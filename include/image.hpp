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

// Espaços de cores 
enum ColorSpace{RGB, GRAY};

// status para leitura de imagens
enum ImageAcessStatus{FILENOTFOUND, FILEOPENERROR, FORMATNOTBMP, SUCCESS};

// Imagem formato bmp
class Image 
{
    private: 
        uint32_t width; // largura da imagem
        uint32_t height; // altura da imagem
        uint8_t **channels = nullptr; // Ponteiro para os canais
        ColorSpace color_space; // Espaço de cor da imagem

    public:

        Image(); // construtor padrão 
        Image(int height, int width, uint8_t channels=3, uint8_t *perl=nullptr, ColorSpace color_space=RGB);
        Image(const Image& other); // construtor de cópias 
        ~Image(); // destrutor 
        Image& operator=(const Image& other); // sobrecrita de operador de atribuição
        
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
        const uint8_t get_number_channels()
        {
            switch (color_space)
            {
            case RGB:
                return 3;
            case GRAY:
                return 1;
            default:
                return 0;
            }
        }

        std::vector<uint8_t> get_perl(int row, int column)
        {
            uint8_t channels = this->get_number_channels();
            std::vector<uint8_t> perl(channels);

            for(int i = 0; i < channels; i++)
            {
                perl[i] = this->channels[i][index_image(row, column, this->width)];
            }

            return perl;
        }

        void get_perl(int row, int column, uint8_t *perl_dst)
        {
            uint8_t number_channels = this->get_number_channels();

            for(int i = 0; i < number_channels; i++)
                perl_dst[i] = this->channels[i][index_image(row, column, this->width)];
            
        }

        void set_perl(int row, int column, std::vector<uint8_t> perl)
        {
            uint8_t number_channels = this->get_number_channels();

            for(int i = 0; i < number_channels; i++)
            {
                this->channels[i][index_image(row, column, this->width)] = perl[i];
            }
        }

        void set_perl(int row, int column, uint8_t *perl)
        {
            uint8_t number_channels = this->get_number_channels();

            for(int i = 0; i < number_channels; i++)
            {
                this->channels[i][index_image(row, column, this->width)] = perl[i];
            }
        }

        // retorna o canal vermelho
        Image get_channel(uint8_t channel);

        // retorna a imagem negativa
        Image negative();

        // retorna a imagem binaria
        Image binary(const uint8_t thres = 125);

        // retorna a imagem em escala de cinza por media 
        Image mean_gray_scale();

        // retorna a imagem em escala de cinza por media ponderada
        Image RGB_TO_GRAY();

        // blur por media 
        Image mean_blur(const int size_kernel);

        // blur por mediana
        Image median_blur(const int size_kernel);

        // retorna o histograma da imagem
        std::vector<int> hist(const uint8_t channel);

        // retorna o histograma normalizado
        std::vector<double> hist(const uint8_t channel, const bool is_norm);

        // equaliza o histrograma do canal 0
        Image equalize(const uint8_t channel);

        Image equalize_esp(const uint8_t channel, std::vector<double> hist);

        void write_hist(const char path[]);

        void show_hist();

        friend ImageAcessStatus read_bmp(const char* path_image, Image& image_dst, const bool is_true_color);
        friend ImageAcessStatus write_bmp(const char* path_image, Image& image_src, const bool is_true_color);

        Image operator+(Image& other) {
            Image copy(*this);

            int height = this->height;
            int width = this->width;
            uint8_t number_channels = this->get_number_channels();
            uint8_t *perl_1 = new uint8_t[number_channels]{0}; 
            uint8_t *perl_2 = new uint8_t[number_channels]{0}; 

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    this->get_perl(i, j, perl_1);
                    other.get_perl(i, j, perl_2);

                    sum_perl(perl_1, perl_2, nullptr, number_channels);

                    copy.set_perl(i, j, perl_1);
                }
            }

            delete[] perl_1;
            delete[] perl_2;

            return copy;
        }

        Image operator-(Image& other) {
            Image copy(*this);

            int height = this->height;
            int width = this->width;
            uint8_t number_channels = this->get_number_channels();
            uint8_t *perl_1 = new uint8_t[number_channels]{0}; 
            uint8_t *perl_2 = new uint8_t[number_channels]{0}; 

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    this->get_perl(i, j, perl_1);
                    other.get_perl(i, j, perl_2);

                    sub_perl(perl_1, perl_2, nullptr, number_channels);

                    copy.set_perl(i, j, perl_1);
                }
            }

            delete[] perl_1;
            delete[] perl_2;

            return copy;
        }

        Image operator&(Image& other) {
            Image copy(*this);

            int height = this->height;
            int width = this->width;
            uint8_t number_channels = this->get_number_channels();
            uint8_t *perl_1 = new uint8_t[number_channels]{0}; 
            uint8_t *perl_2 = new uint8_t[number_channels]{0}; 

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    this->get_perl(i, j, perl_1);
                    other.get_perl(i, j, perl_2);

                    bitwise_and_perl(perl_1, perl_2, nullptr, number_channels);

                    copy.set_perl(i, j, perl_1);
                }
            }

            delete[] perl_1;
            delete[] perl_2;

            return copy;
        }

        Image operator|(Image& other) {
            Image copy(*this);

            int height = this->height;
            int width = this->width;
            uint8_t number_channels = this->get_number_channels();
            uint8_t *perl_1 = new uint8_t[number_channels]{0}; 
            uint8_t *perl_2 = new uint8_t[number_channels]{0}; 

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    this->get_perl(i, j, perl_1);
                    other.get_perl(i, j, perl_2);

                    bitwise_or_perl(perl_1, perl_2, nullptr, number_channels);

                    copy.set_perl(i, j, perl_1);
                }
            }

            delete[] perl_1;
            delete[] perl_2;

            return copy;
        }

        Image operator~() {
            Image copy(*this);

            int height = this->height;
            int width = this->width;
            uint8_t number_channels = this->get_number_channels();
            uint8_t *perl_1 = new uint8_t[number_channels]{0}; 

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    this->get_perl(i, j, perl_1);

                    bitwise_not_perl(perl_1, nullptr, number_channels);

                    copy.set_perl(i, j, perl_1);
                }
            }

            delete[] perl_1;

            return copy;
        }

        Image operator*(const double mult) {
            Image copy(*this);

            int height = this->height;
            int width = this->width;
            uint8_t number_channels = this->get_number_channels();
            uint8_t *perl_1 = new uint8_t[number_channels]{0}; 

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    this->get_perl(i, j, perl_1);

                    mul_perl(perl_1, nullptr, mult, number_channels);

                    copy.set_perl(i, j, perl_1);
                }
            }

            delete[] perl_1;

            return copy;
        }

        Image operator/(const double div) {
            Image copy(*this);

            int height = this->height;
            int width = this->width;
            uint8_t number_channels = this->get_number_channels();
            uint8_t *perl_1 = new uint8_t[number_channels]{0}; 

            for(int i = 0; i < height; i++)
            {
                for(int j = 0; j < width; j++)
                {
                    this->get_perl(i, j, perl_1);

                    div_perl(perl_1, nullptr, div, number_channels);

                    copy.set_perl(i, j, perl_1);
                }
            }

            delete[] perl_1;

            return copy;
        }
};