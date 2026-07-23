#include <core.hpp>

// leitura de imagens bmp
int8_t read_bmp(const char* path_image, Image& image_dst) {
    FILE *bmp_image = fopen(path_image, "rb"); 
    if(bmp_image == NULL) return 0; 

    HeadFile file_image;
    HeadBitMap bitmap;

    fread(&file_image.type_file, sizeof(file_image.type_file), 1, bmp_image);
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

    if(file_image.type_file != 0x4D42) {
        fclose(bmp_image);
        return -1; 
    }

    if (image_dst.perls != nullptr) delete[] image_dst.perls;
    image_dst.perls = new Perl[bitmap.height * bitmap.width];

    image_dst.height = bitmap.height;
    image_dst.width = bitmap.width;
    image_dst.channels = bitmap.size_pixel / 8;
    
    fseek(bmp_image, file_image.offset_data_field, SEEK_SET);

    int padding = (4 - (image_dst.get_width() * 3) % 4) % 4; // padding para alinha as linhas em multiplos de 4
    
    for(uint32_t i = 0; i < bitmap.height; i++) {
        for(uint32_t j = 0; j < bitmap.width; j++) {
            
            uint8_t rgb[3];

            if(fread(rgb, sizeof(uint8_t), 3, bmp_image) == 3)
            {
                image_dst.get_perl(j, i).set(rgb[2], rgb[1], rgb[0]); /// leitura na forma BGR
            }
        }
        fseek(bmp_image, padding, SEEK_CUR);
    }

    fclose(bmp_image);
    return 1; 
}

// escrita de imagens bmp
int8_t write_bmp(const char* path_image, Image& image) {

    uint32_t height = image.get_height();
    uint32_t width = image.get_width();

    int padding = (4 - (width * 3) % 4) % 4; // preenchimento para alinhar em bytes multiplos de 4 de final de linha 
    int true_width = padding + (width * 3);

    uint16_t HeadBitMapSize = 40;
    uint16_t HeadFileSize = 14;

    // Cabeçalho do arquivo
    uint16_t BFTYPE = 0x4D42;
    uint32_t BFSIZE = (height * true_width) + 54;
    uint16_t BFRESER1 = 0;
    uint16_t BFRESER2 = 0;
    uint32_t BFOFFSETBITS = 54;

    // Cabeçalho do mapa de bits
    uint32_t BITSIZE = 40;
    uint32_t BIWIDTH = width;
    uint32_t BIHEIGHT = height;
    uint16_t BIPLANES = 1;
    uint16_t BIBITCOUNT = 24;
    uint32_t BICOMPRESS = 0;
    uint32_t BISIZEIMG = BFSIZE - (HeadBitMapSize + HeadFileSize);
    uint32_t BIXPPMETER = 0;
    uint32_t BIYPPMETER = 0;
    uint32_t BICLRMETER = 0;  
    uint32_t BICLRIMPOR = 0;

    FILE *bmp_image = fopen(path_image, "wb"); 
    if(bmp_image == NULL) return 0; 

    fwrite(&BFTYPE, sizeof(BFTYPE), 1, bmp_image);
    fwrite(&BFSIZE, sizeof(BFSIZE), 1, bmp_image);
    fwrite(&BFRESER1, sizeof(BFRESER1), 1, bmp_image); 
    fwrite(&BFRESER2, sizeof(BFRESER2), 1, bmp_image); 
    fwrite(&BFOFFSETBITS, sizeof(BFOFFSETBITS), 1, bmp_image);

    fwrite(&BITSIZE, sizeof(BITSIZE), 1, bmp_image);
    fwrite(&BIWIDTH, sizeof(BIWIDTH), 1, bmp_image);
    fwrite(&BIHEIGHT, sizeof(BIHEIGHT), 1, bmp_image);
    fwrite(&BIPLANES, sizeof(BIPLANES), 1, bmp_image); 
    fwrite(&BIBITCOUNT, sizeof(BIBITCOUNT), 1, bmp_image);
    fwrite(&BICOMPRESS, sizeof(BICOMPRESS), 1, bmp_image);
    fwrite(&BISIZEIMG, sizeof(BISIZEIMG), 1, bmp_image);
    fwrite(&BIXPPMETER, sizeof(BIXPPMETER), 1, bmp_image);
    fwrite(&BIYPPMETER, sizeof(BIYPPMETER), 1, bmp_image);
    fwrite(&BICLRMETER, sizeof(BICLRMETER), 1, bmp_image);
    fwrite(&BICLRIMPOR, sizeof(BICLRIMPOR), 1, bmp_image);

    fseek(bmp_image, BFOFFSETBITS, SEEK_SET);

    uint8_t padding_byte = 0;

    for(uint32_t i = 0; i < height; i++) {
        for(uint32_t j = 0; j < width; j++) {
            Perl& p = image.get_perl(j, i);
            uint8_t bgr[] = {p.getBlue(), p.getGreen(), p.getRed()};

            fwrite(bgr, sizeof(uint8_t), 3, bmp_image); // grava na ordem BGR
        }
        for(int p = 0; p < padding; p++) {
            fwrite(&padding_byte, sizeof(uint8_t), 1, bmp_image); 
        }
    }

    fclose(bmp_image);
    return 1; 
}