#include <stdint.h>

// Cabeçalho do arquivo da imagem BMP (14 bytes)
typedef struct 
{
    uint16_t type_file = 0x4D42; // extensão do arquivo 
    uint32_t size_file_bytes = 0; // tamanho total do arquivo em bytes
    uint16_t reserved1 = 0; // reservado (sempre 0)
    uint16_t reserved2 = 0; // reservado (sempre 0)
    uint32_t offset_data_field = 54; // deslocamento para o início da área de pixels (14 + 40 = 54)
    uint8_t size_head_file = 14; // tamanho do cabeçalho do arquivo
} HeadFile;

// Cabeçalho do mapa de bits da imagem BMP (40 bytes - BITMAPINFOHEADER)
typedef struct 
{
    uint32_t size_head_bitmap = 40; // tamanho do cabeçalho do bitmap
    uint32_t width = 0; // largura da imagem em pixels
    uint32_t height = 0; // altura da imagem em pixels
    uint16_t planes = 1;  // número de planos de cor (deve ser 1)
    uint16_t size_pixel = 24; // tamanho do pixel em bits (24 bits = RGB/TrueColor)
    uint32_t compression_image = 0; // tipo de compressão (0 = sem compressão)
    uint32_t size_image = 0; // tamanho dos dados de imagem (largura_com_padding * altura)
    uint32_t resolution_horizontal_image = 0; //  resolução horizontal da imagem
    uint32_t resolution_vertical_image = 0; // resolução vertical da imagem
    uint32_t colors_scale_image = 0; // número de cores na paleta (0 para 24 bits)
    uint32_t colors_scale_image_used = 0;// cores importantes (0 = todas)
} HeadBitMap;

// Formato RGBA (usado em imagens com paleta/menos de 24 bits por pixel)
typedef struct 
{
    uint8_t blue = 0;
    uint8_t green = 0;
    uint8_t red = 0;
    uint8_t reserved = 0;
} ColorsPallet;