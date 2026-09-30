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
        uint16_t width; // largura da imagem
        uint16_t height; // altura da imagem
        uint8_t **channels = nullptr; // Ponteiro para os canais
        ColorSpace color_space; // Espaço de cor da imagem

    public:
        
        // Construtor padrão criar o objeto vazio
        Image(); 

        // Construtor padrão com parâmetros para a imagem
        Image(const uint16_t height, const uint16_t width, uint8_t *perl=nullptr, const ColorSpace color_space=RGB);
        
        // Construtor de cópias 
        Image(const Image& other);
        
        // Destrutor 
        ~Image();

        // Operador de atribuição
        Image& operator=(const Image& other); 

        Image operator+(Image& other);
    
        Image operator-(Image& other);

        Image operator&(Image& other);

        Image operator|(Image& other);

        Image operator~();

        Image operator*(const double mult);

        Image operator/(const double div);
        
        // retorna a largura
        uint16_t get_width() {
            return this->width;
        }

        // retorna altura 
        uint16_t get_height()
        {
            return this->height;
        }

        // retorna o espaço de cor 
        ColorSpace get_color_space()
        {
            return this->color_space;
        }

        // retorna a quantidade de canais de acordo com o espaço de cores
        static uint8_t get_number_channels(ColorSpace color_space)
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

        // Retorna os valores e um dado perl
        std::vector<uint8_t> get_perl(int row, int column)
        {
            uint8_t number_channels = get_number_channels(this->color_space);
            std::vector<uint8_t> perl(number_channels);

            for(uint8_t i = 0; i < number_channels; i++)
                perl[i] = this->channels[i][index_image(row, column, this->width)];
            

            return perl;
        }

        // copia o valor de um perl para um vetor uint8_t passado por parâmetro
        void get_perl(const int row, const int column, uint8_t *perl_dst)
        {
            uint8_t number_channels = get_number_channels(this->color_space);

            for(uint8_t i = 0; i < number_channels; i++)
                perl_dst[i] = this->channels[i][index_image(row, column, this->width)];
            
        }

        // muda o valor de um perl da imagem pelo vector passado por parâmetro
        void set_perl(const int row, const int column, std::vector<uint8_t> perl)
        {
            uint8_t number_channels = get_number_channels(this->color_space);

            for(uint8_t i = 0; i < number_channels; i++)
                this->channels[i][index_image(row, column, this->width)] = perl[i];
            
        }

        // muda o valor de um perl da imagem pelo vetor uint8_t passado por parâmetro
        void set_perl(const int row, const int column, uint8_t *perl)
        {
            uint8_t number_channels = get_number_channels(this->color_space);

            for(uint8_t i = 0; i < number_channels; i++)
                this->channels[i][index_image(row, column, this->width)] = perl[i];
            
        }

        // retorna o canal vermelho
        Image get_channel(uint8_t channel);

        // retorna a imagem negativa
        Image negative();

        // retorna a imagem em escala de cinza por media dos canais
        Image mean_gray_scale();

        // retorna a imagem em escala de cinza por media ponderada
        Image RGB_TO_GRAY();

        // retorna a imagem binaria
        Image binary(const uint8_t thres = 125);

        // blur por media 
        Image mean_blur(const uint16_t size_kernel);

        // blur por mediana
        Image median_blur(const uint16_t size_kernel);

        // retorna o histograma da imagem
        std::vector<int> hist(const uint8_t channel);

        // retorna o histograma normalizado
        std::vector<double> hist(const uint8_t channel, const bool is_norm);

        // equaliza o canal passado por parâmetro
        Image equalize(const uint8_t channel);

        // especifica o canal passado por parâmetro utilizando o histograma passado por parâmtro
        Image equalize_esp(const uint8_t channel, std::vector<double> hist);

        void write_hist(const char path[]);

        void show_hist();

        friend ImageAcessStatus read_bmp(const char* path_image, Image& image_dst, const bool is_true_color);
        friend ImageAcessStatus write_bmp(const char* path_image, Image& image_src, const bool is_true_color);
};