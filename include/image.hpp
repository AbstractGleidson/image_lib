#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <utils.hpp>
#include <perl.hpp>

// Formatos de imagem 
enum ImageFormat{BMP, JPG, PNG};

// status para leitura de imagens
enum ImageAcessStatus{FILENOTFOUND, FILEOPENERROR, FORMATNOTBMP, SUCCESS};

// Imagem formato bmp
class Image 
{
    private: 
        uint32_t width; // largura da imagem
        uint32_t height; // altura da imagem
        uint32_t channels; // quantidade de canais 
        Perl *perls; // inicio da matriz de pixels

    public:

        Image(); // construtor padrão 
        Image(const Image& other); // construtor de cópias 
        ~Image(); // destrutor 
        Image& operator=(const Image& other); // sobrecrita de operador de atribuição
        
        void print_head_image(); // printa informaçoes básicas da imagem 
        void print_matriz_image(const uint32_t head); // printa o inicio da matriz de perls

        // retorna a largura
        uint32_t get_width() {
            return this->width;
        }

        // retorna altura 
        uint32_t get_height()
        {
            return this->height;
        }

        // retorna a quantidade de canais
        uint8_t get_channels()
        {
            return this->channels;
        }

        // retorna o perls na posição adequada
        Perl& get_perl(int x, int y)
        {
            return this->perls[(y * this->width) + x];
        }

        // retorna o canal vermelho
        Image get_red_image();

        // retorna o canal verde
        Image get_green_image();

        // retorna o canal azul
        Image get_blue_image();

        friend ImageAcessStatus read_bmp(const char* path_image, Image& image_dst);
        friend ImageAcessStatus write_bmp(const char* path_image, Image& image);
};