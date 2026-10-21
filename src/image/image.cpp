#include <image.hpp>

// construtor padrão
Image::Image()
{
    this->height = 0; 
    this->width = 0; 
    this->channels = nullptr; // ponteiro para os canais
    this->color_space = RGB; 
}

// construtor personalizado
Image::Image(const uint16_t height, const uint16_t width, uint8_t *perl, const ColorSpace color_space)
{
    this->height = height; 
    this->width = width; 
    this->color_space = color_space; 

    uint8_t number_channels = get_number_channels(color_space); // converte o numero de canais em função do espaço de cor

    this->channels = new uint8_t*[number_channels]; // alloca memória para os ponteiros

    uint8_t *default_perl = perl; // perls padrão que irá preencher a imagem

    if(perl == nullptr)
        default_perl = new uint8_t[number_channels]{0}; // aloca um array inicializado com 0

    int elements = this->height * this->width;

    for(uint8_t i = 0; i < number_channels; i++)
    {
        this->channels[i] = new uint8_t[elements]; // aloca memoria para cada canal
        memset(this->channels[i], default_perl[i], elements * sizeof(uint8_t)); // preenche com o valor do perl 
    }

    if(perl == nullptr)
        delete[] default_perl; 
}

// construtor de cópia, metodo copy
Image::Image(const Image& other)
{
    // cópia valores numericos
    this->height = other.height;
    this->width  = other.width;
    this->color_space = other.color_space;

    int number_channels = get_number_channels(color_space);

    this->channels = new uint8_t*[number_channels]; // cria um ponteiro para cada canal

    if(other.channels != nullptr)
    {
        int elements = this->height * this->width; // quantidade de elementos em cada canal

        for(uint8_t i = 0; i < number_channels; i++)
        {
            this->channels[i] = new uint8_t[elements]; // aloca memoria para cada canal
            memcpy(this->channels[i], other.channels[i], elements * sizeof(uint8_t)); // cópia o bloco de memória 
        }
    }
    else
        this->channels  = nullptr;
}

// destrutor 
Image::~Image()
{
    if(channels != nullptr)
    {
        uint8_t number_channels = get_number_channels(this->color_space);
        
        for(uint8_t i = 0; i < number_channels; i++)
            delete[] channels[i]; // libera memória da heap para cada canal
        delete[] channels; // libera os ponteiros duplos
    }
}

Image Image::get_channel(uint8_t channel) {
    Image copy = Image(this->height, this->width, nullptr, GRAY); 
    
    memcpy(copy.channels[0], this->channels[channel], (this->height * this->width) * sizeof(uint8_t));
    return copy;
}

Image Image::mean_gray_scale()
{
    Image copy = Image(this->height, this->width, nullptr, GRAY);

    uint8_t number_channels = get_number_channels(this->color_space);
    uint16_t height = this->height;
    uint16_t width = this->width;
            
    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < height; i++)
    {
        // Alocado dentro do loop para ser thread-safe no OpenMP sem data races
        uint8_t *perl = new uint8_t[number_channels];

        for(int j = 0; j < width; j++)
        {
            this->get_perl(i, j, perl);

            int sum = 0; 
            for(uint8_t c = 0; c < number_channels; c++)
                sum += perl[c]; 

            uint8_t mean = (uint8_t) (sum / number_channels); // calcula a media dos canais

            copy.set_perl(i, j, &mean);
        }

        delete[] perl; // Libera o buffer da thread atual
    }

    return copy;
}

Image Image::RGB_TO_GRAY()
{
    Image copy = Image(this->height, this->width, nullptr, GRAY);

    uint16_t height = this->height;
    uint16_t width = this->width;
    uint8_t number_channels = get_number_channels(this->color_space);

    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < height; i++)
    {
        uint8_t *perl = new uint8_t[number_channels];

        for(int j = 0; j < width; j++)
        {
            this->get_perl(i, j, perl);

            // Pesos de acordo com a sensibilidade do olho humano 
            double weight_red = 0.2126, weight_green = 0.7152, weight_blue = 0.0722; 

            double gray_val = (perl[0] * weight_red) + (perl[1] * weight_green) + (perl[2] * weight_blue);
            
            uint8_t gray = (uint8_t) (std::min(std::max((int) (gray_val), 0), 255));

            copy.set_perl(i, j, &gray);
        }

        delete[] perl;
    }

    return copy;
}