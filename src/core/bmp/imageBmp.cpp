#include <core.hpp>
#include <bmpImageHeaders.hpp>

// calcula o pedding de final de linha para imagens bmp
int imageBmpPaddingTrueColor(uint32_t width)
{
    return (4 - (width * 3) % 4) % 4;
}

// calcula o pedding de final de linha para imagens bmp
int imageBmpPaddingGrayScale(uint32_t width)
{
    return (4 - (width % 4)) % 4;
}

ImageAcessStatus read_bmp(const char* path_image, Image& image_dst, const bool is_true_color = true) {
    FILE *bmp_image = fopen(path_image, "rb"); 
    if(bmp_image == NULL) return ImageAcessStatus::FILENOTFOUND; 

    HeadFile file_image;
    HeadBitMap bitmap;

    // Leitura do tipo de arquivo
    if(fread(&file_image.type_file, sizeof(file_image.type_file), 1, bmp_image) != 1) {
        fclose(bmp_image);
        return ImageAcessStatus::FORMATNOTBMP;
    }

    // Verifica se é um arquivo .bmp ("BM")
    if(file_image.type_file != 0x4D42) {
        fclose(bmp_image);
        return ImageAcessStatus::FORMATNOTBMP; 
    }

    fread(&file_image.size_file_bytes, sizeof(file_image.size_file_bytes), 1, bmp_image);
    fseek(bmp_image, 4, SEEK_CUR); // Pula reserved1 e reserved2
    fread(&file_image.offset_data_field, sizeof(file_image.offset_data_field), 1, bmp_image);
    fseek(bmp_image, 4, SEEK_CUR); // Pula tamanho do cabeçalho DIB
    
    fread(&bitmap.width, sizeof(bitmap.width), 1, bmp_image);
    fread(&bitmap.height, sizeof(bitmap.height), 1, bmp_image);
    fseek(bmp_image, 2, SEEK_CUR); // Pula planos
    fread(&bitmap.size_pixel, sizeof(bitmap.size_pixel), 1, bmp_image);
    fread(&bitmap.compression_image, sizeof(uint32_t), 1, bmp_image); 
    fread(&bitmap.size_image, sizeof(bitmap.size_image), 1, bmp_image);
    fread(&bitmap.resolution_horizontal_image, sizeof(bitmap.resolution_horizontal_image), 1, bmp_image);
    fread(&bitmap.resolution_vertical_image, sizeof(bitmap.resolution_vertical_image), 1, bmp_image);
    fread(&bitmap.colors_scale_image, sizeof(bitmap.colors_scale_image), 1, bmp_image);
    fread(&bitmap.colors_scale_image_used, sizeof(bitmap.colors_scale_image_used), 1, bmp_image);

    ColorsPallet *colors_pallet = nullptr;
    int padding = 0; 
    uint8_t *perl = new uint8_t[is_true_color ? 3 : 1]{0};

    // Leitura de paleta ou ajuste de ponteiro para dados
    if(!is_true_color) {
        image_dst = Image(bitmap.height, bitmap.width, 1, nullptr, GRAY); 
        padding = imageBmpPaddingGrayScale(image_dst.get_width()); 

        int pallet_size = (bitmap.colors_scale_image == 0) ? 256 : bitmap.colors_scale_image; 
        colors_pallet = new ColorsPallet[pallet_size]; 

        for(int i = 0; i < pallet_size; i++) {
            fread(&colors_pallet[i], sizeof(uint8_t), 4, bmp_image);
        }
    }
    else {
        image_dst = Image(bitmap.height, bitmap.width, 3, nullptr, RGB); 
        padding = imageBmpPaddingTrueColor(image_dst.get_width());
        
        // Garante que o ponteiro vai exatamente onde começam os dados de pixel
        fseek(bmp_image, file_image.offset_data_field, SEEK_SET);
    }

    for(uint32_t i = 0; i < image_dst.height; i++) {
        for(uint32_t j = 0; j < image_dst.width; j++) {
            
            if(is_true_color) {
                if(fread(perl, sizeof(uint8_t), 3, bmp_image) == 3) {
                    image_dst.set_perl(i, j, perl); // Leitura BGR
                }
            }
            else {
                if(fread(perl, sizeof(uint8_t), 1, bmp_image) == 1) {
                    image_dst.set_perl(i, j, perl);
                }
            }
        }
        // Pula os bytes de padding de alinhamento de 4 bytes ao final de cada linha
        fseek(bmp_image, padding, SEEK_CUR); 
    }

    // Limpeza segura de recursos
    if (colors_pallet != nullptr) {
        delete[] colors_pallet;
    }
    delete[] perl;
    fclose(bmp_image);

    return ImageAcessStatus::SUCCESS; 
}

ImageAcessStatus write_bmp(const char* path_image, Image& image, const bool is_true_color = true) {

    FILE *bmp_image = fopen(path_image, "wb"); 
    if(bmp_image == NULL) return ImageAcessStatus::FILEOPENERROR; 

    uint32_t height = image.get_height();
    uint32_t width = image.get_width();

    HeadFile file_header;
    HeadBitMap bitmap_header;

    int padding = 0;
    int true_width = 0;

    // cálculo de padding e largura da linha alinhada a 4 bytes
    if(is_true_color) {
        padding = imageBmpPaddingTrueColor(width);
        true_width = (width * 3) + padding;
        
        bitmap_header.colors_scale_image = 0;
        bitmap_header.colors_scale_image_used = 0;
        bitmap_header.size_pixel = 24; // 24 bits para True Color
        file_header.offset_data_field = file_header.size_head_file + bitmap_header.size_head_bitmap;
    }
    else {
        padding = imageBmpPaddingGrayScale(width);
        true_width = width + padding;

        bitmap_header.colors_scale_image = 256;
        bitmap_header.colors_scale_image_used = 256;
        bitmap_header.size_pixel = 8; // 8 bits para Escala de Cinza
        file_header.offset_data_field = file_header.size_head_file + bitmap_header.size_head_bitmap + (4 * 256);
    }
    
    // Cabeçalho do arquivo
    file_header.size_file_bytes = file_header.offset_data_field + (height * true_width);

    // Cabeçalho do mapa de bits
    bitmap_header.width = width;
    bitmap_header.height = height;
    bitmap_header.size_image = height * true_width;

    // Escrevendo Cabeçalho do arquivo
    fwrite(&file_header.type_file, sizeof(file_header.type_file), 1, bmp_image);
    fwrite(&file_header.size_file_bytes, sizeof(file_header.size_file_bytes), 1, bmp_image);
    fwrite(&file_header.reserved1, sizeof(file_header.reserved1), 1, bmp_image); 
    fwrite(&file_header.reserved2, sizeof(file_header.reserved2), 1, bmp_image); 
    fwrite(&file_header.offset_data_field, sizeof(file_header.offset_data_field), 1, bmp_image);

    // Escrevendo Cabeçalho do bitmap
    fwrite(&bitmap_header.size_head_bitmap, sizeof(bitmap_header.size_head_bitmap), 1, bmp_image);
    fwrite(&bitmap_header.width, sizeof(bitmap_header.width), 1, bmp_image);
    fwrite(&bitmap_header.height, sizeof(bitmap_header.height), 1, bmp_image);
    fwrite(&bitmap_header.planes, sizeof(bitmap_header.planes), 1, bmp_image); 
    fwrite(&bitmap_header.size_pixel, sizeof(bitmap_header.size_pixel), 1, bmp_image);
    fwrite(&bitmap_header.compression_image, sizeof(bitmap_header.compression_image), 1, bmp_image);
    fwrite(&bitmap_header.size_image, sizeof(bitmap_header.size_image), 1, bmp_image);
    fwrite(&bitmap_header.resolution_horizontal_image, sizeof(bitmap_header.resolution_horizontal_image), 1, bmp_image);
    fwrite(&bitmap_header.resolution_vertical_image, sizeof(bitmap_header.resolution_vertical_image), 1, bmp_image);
    fwrite(&bitmap_header.colors_scale_image, sizeof(bitmap_header.colors_scale_image), 1, bmp_image);
    fwrite(&bitmap_header.colors_scale_image_used, sizeof(bitmap_header.colors_scale_image_used), 1, bmp_image);

    // Escreve paleta de cores apenas se for escala de cinza
    if(!is_true_color) {
        for(int i = 0; i < 256; i++) {
            uint8_t rgbr[4] = {(uint8_t)i, (uint8_t)i, (uint8_t)i, 0}; // B, G, R, Reserved
            fwrite(rgbr, sizeof(uint8_t), 4, bmp_image);
        }
    }
    
    uint8_t padding_byte = 0;
    int num_channels = image.get_number_channels();
    uint8_t *perl = new uint8_t[num_channels];

    for(uint32_t i = 0; i < height; i++) {
        for(uint32_t j = 0; j < width; j++) {
            image.get_perl(i, j, perl);

            if(is_true_color){
                uint8_t bgr[] = {perl[0], perl[1], perl[2]}; // ordem BGR do BMP
                fwrite(bgr, sizeof(uint8_t), 3, bmp_image);
            }
            else { 
                fwrite(perl, sizeof(uint8_t), 1, bmp_image);
            }
        }
        // Preenchendo o padding com zeros para alinhar em múltiplos de 4 bytes
        for(int p = 0; p < padding; p++) {
            fwrite(&padding_byte, sizeof(uint8_t), 1, bmp_image); 
        }
    }

    // Limpeza de memória
    delete[] perl;
    fclose(bmp_image);
    return ImageAcessStatus::SUCCESS; 
}