#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <utils.hpp>
#include <perl.hpp>
#include <vector>
#include <algorithm>

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

        Perl& get_perl(int x, int y)
        {
            return this->perls[(y * this->width) + x];
        }

        void set_perl(int x, int y, const Perl perl)
        {
            this->perls[(y * this->width) + x] = perl;
        }

        // retorna o canal vermelho
        Image get_red_image();

        // retorna o canal verde
        Image get_green_image();

        // retorna o canal azul
        Image get_blue_image();

        // retorna a imagem negativa
        Image negative();

        // retorna a imagem binaria
        Image binary(const uint8_t thres = 125);

        // retorna a imagem em escala de cinza por media 
        Image mean_gray_scale();

        // retorna a imagem em escala de cinza por media ponderada
        Image gray_scale();

        // blur por media 
        Image mean_blur(const int size_kernel);

        // blur por mediana
        Image median_blur(const int size_kernel);

        friend ImageAcessStatus read_bmp(const char* path_image, Image& image_dst, const bool is_true_color);
        friend ImageAcessStatus write_bmp(const char* path_image, Image& image_src, const bool is_true_color);

        Image operator+(Image& other) {
            Image copy(*this);

            int height = this->height;
            int width = this->width;

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    copy.set_perl(j, i, this->get_perl(j, i) + other.get_perl(j, i));
                }
            }

            return copy;
        }

        Image operator-(Image& other) {
            Image copy(*this);

            int height = this->height;
            int width = this->width;

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    copy.set_perl(j, i, this->get_perl(j, i) - other.get_perl(j, i));
                }
            }

            return copy;
        }

        Image operator&(Image& other) {
            Image copy(*this);

            int height = this->height;
            int width = this->width;

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    copy.set_perl(j, i, this->get_perl(j, i) & other.get_perl(j, i));
                }
            }

            return copy;
        }

        Image operator|(Image& other) {
            Image copy(*this);

            int height = this->height;
            int width = this->width;

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    copy.set_perl(j, i, this->get_perl(j, i) | other.get_perl(j, i));
                }
            }

            return copy;
        }

        Image operator*(const int number) {
            Image copy(*this);

            int height = this->height;
            int width = this->width;

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    copy.set_perl(j, i, this->get_perl(j, i) * number);
                }
            }

            return copy;
        }
};