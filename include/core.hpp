#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <image.hpp>
#include <utils.hpp>

enum ImageFormat{BMP, JPG, PNG};

// Cabeçalho do arquivo da imagem
typedef struct 
{
    uint16_t type_file; // extensão do arquivo
    uint32_t size_file_bytes; // tamanho do arquivo em bytes
    uint32_t offset_data_field; // deslocamento no arquivo em bytes para o inicio da area de dados 
} HeadFile;

// Cabeçalho do mapa de bits da imagem 
typedef struct 
{
    uint32_t size_head_bitmap; // tamanho do cabeçalho do bitmap
    uint32_t width; // largura da imagem em pixels
    uint32_t height; // altura da imagem em pixels
    uint16_t size_pixel; // tamanho dos pixels em bits
    uint32_t compression_image; // tipo de compressão da imagem
    uint32_t size_image; // tamanho da área de dados da imagem em bytes
    uint32_t resolution_horizontal_image; //  em pixels por metro
    uint32_t resolution_vertical_image; // em pixels por metro
    uint32_t colors_scale_image; // Número de tons de cada cor 
    uint32_t colors_scale_image_used; // Tons de cada cor realmente utilizado
} HeadBitMap;

// Formato RGBA, só existe em imagem com menos de 24 bits por pixel
typedef struct 
{
    uint8_t blue, green, red, reserved; // Paleta de cores da imagem
} ColorsPallet;

int8_t read_bmp(const char* path_image, Image& image_dst);
int8_t write_bmp(const char* path_image, Image& image_dst);