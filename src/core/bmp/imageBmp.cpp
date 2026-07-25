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

    fread(&file_image.type_file, sizeof(file_image.type_file), 1, bmp_image);

    // verifica se é um arquivo .bmp
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

    // desaloca a memória nos casos necessarios 
    if (image_dst.perls != nullptr) delete[] image_dst.perls;

    image_dst.perls = new Perl[bitmap.height * bitmap.width]; // aloca memória na heap para a matriz de perls 

    image_dst.height = bitmap.height; // cópia altura 
    image_dst.width = bitmap.width; // cópia largura 
    image_dst.channels = bitmap.size_pixel / 8; // calcula a quantidade de canais

    uint8_t rgb[3];

    ColorsPallet *colors_pallet = nullptr;

    // calcula o padding para manter o padrão de multiplos de 4 nas linhas
    int padding; 

    // ler paleta de cores caso seja uma imagem em  escala de cinza
    if(!is_true_color)
    {
        padding = imageBmpPaddingGrayScale(image_dst.get_width()); // calcula o pedding para imagens em escala de cinza

        int pallet_size = bitmap.colors_scale_image == 0?256:bitmap.colors_scale_image; // pode ser 0 ou 256 pra representar a quantidade de cores máxima

        colors_pallet = new ColorsPallet[pallet_size]; // aloca memória na heap para paletas

        // faz a leitura da paleta de cores 
        for(int i = 0; i < bitmap.colors_scale_image; i++)
        {
            fread(&colors_pallet[i], sizeof(uint8_t), 4, bmp_image);
        }
    }
    else{
        // calcula o padding para imagem true color 
        padding = imageBmpPaddingTrueColor(image_dst.get_width());
        // aponta o poteiro do arquivo para a área de dados
        fseek(bmp_image, file_image.offset_data_field, SEEK_SET);
    }

    // lendo pixels
    for(uint32_t i = 0; i < image_dst.height; i++) {
        for(uint32_t j = 0; j < image_dst.width; j++) {
            
            // imagem true color(colorida)
            if(is_true_color){
                // faz a leitura dos bytes para os pixels 
                if(fread(rgb, sizeof(uint8_t), 3, bmp_image) == 3)
                {
                    image_dst.get_perl(j, i).set(rgb[2], rgb[1], rgb[0]); /// leitura na forma BGR
                }
            }
            // imagem em escala de cinza
            else{
                if(fread(rgb, sizeof(uint8_t), 1, bmp_image) == 1)
                {
                    image_dst.set_perl(j, i, 
                        Perl(
                            colors_pallet[rgb[0]].red, 
                            colors_pallet[rgb[0]].green, 
                            colors_pallet[rgb[0]].blue
                        )
                    );
                }
            }
        }
        fseek(bmp_image, padding, SEEK_CUR); // pula os valores de alinhamento 
    }
    fclose(bmp_image);

    delete[] colors_pallet; // desaloca a memoria da paleta de cores da heap
    return ImageAcessStatus::SUCCESS; 
}

ImageAcessStatus write_bmp(const char* path_image, Image& image, const bool is_true_color = true) {

    FILE *bmp_image = fopen(path_image, "wb"); 
    if(bmp_image == NULL) return ImageAcessStatus::FILEOPENERROR; 

    uint32_t height = image.get_height();
    uint32_t width = image.get_width();

    HeadFile file_header;
    HeadBitMap bitmap_header;

    int padding;

    int true_width;

    if(is_true_color)
    {
        true_width = padding + (width * 3);
        padding = imageBmpPaddingTrueColor(image.get_width()); // calcula o padding para imagens true color 
    }
    else {
        int true_width = padding + width;

        padding = imageBmpPaddingGrayScale(image.get_width()); // calcula o padding para imagens em escala de cinza

        bitmap_header.colors_scale_image = 256;
        bitmap_header.colors_scale_image_used = 256;
        bitmap_header.size_pixel = 8;
        file_header.offset_data_field = file_header.size_head_file + bitmap_header.size_head_bitmap + (4 * bitmap_header.colors_scale_image);
    }
    
    

    // Cabeçalho do arquivo
    file_header.size_file_bytes = (height * true_width) + file_header.size_head_file + bitmap_header.size_head_bitmap;

    // Cabeçalho do mapa de bits
    bitmap_header.width = width;
    bitmap_header.height = height;
    bitmap_header.size_image = file_header.size_file_bytes - (sizeof(HeadFile) + bitmap_header.size_head_bitmap);

    // Cabeçalho do arquivo
    fwrite(&file_header.type_file, sizeof(file_header.type_file), 1, bmp_image);
    fwrite(&file_header.size_file_bytes, sizeof(file_header.size_file_bytes), 1, bmp_image);
    fwrite(&file_header.reserved1, sizeof(file_header.reserved1), 1, bmp_image); 
    fwrite(&file_header.reserved2, sizeof(file_header.reserved2), 1, bmp_image); 
    fwrite(&file_header.offset_data_field, sizeof(file_header.offset_data_field), 1, bmp_image);

    // Cabeçalho do bitmap da imagem
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

    // escreve paleta de cores
    if(!is_true_color){
        for(int i = 0; i < 256; i++)
        {
            uint8_t rgbr[4];
            rgbr[0] = i; // Red
            rgbr[1] = i; // Green
            rgbr[2] = i; // Blue
            rgbr[3] = 0; // reserved

            fwrite(rgbr, sizeof(uint8_t), 4, bmp_image);
        }
    }
    else{
        // move o ponteiro do arquivo para a área de dados
        fseek(bmp_image, file_header.offset_data_field, SEEK_SET);
    }
    
    uint8_t padding_byte = 0;

    // bytes de dados da imagem
    for(uint32_t i = 0; i < height; i++) {
        for(uint32_t j = 0; j < width; j++) {

            // escreve os bytes para imagem colocoridas
            if(is_true_color){
                Perl& p = image.get_perl(j, i);
                uint8_t bgr[] = {p.get_blue(), p.get_green(), p.get_red()};

                fwrite(bgr, sizeof(uint8_t), 3, bmp_image); // grava na ordem BGR
            }
            else{ // escreve os bytes para imagens em escala de cinza
                Perl& p = image.get_perl(j, i);
                uint8_t bgr = p.get_blue();

                fwrite(&bgr, sizeof(uint8_t), 1, bmp_image); // grava na ordem BGR
            }
        }
        // colocando zeros para fazer padding de alinhamento em multiplos de 4
        for(int p = 0; p < padding; p++) {
            fwrite(&padding_byte, sizeof(uint8_t), 1, bmp_image); 
        }
    }

    fclose(bmp_image);
    return ImageAcessStatus::SUCCESS; 
}