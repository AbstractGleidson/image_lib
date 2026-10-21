#include <image.hpp>

// operador de atribuição
Image& Image::operator=(const Image& other) {
    if (this == &other) return *this; // evita auto atribuição

    // libera a memória atual 
    if (this->channels != nullptr)
    {
        int current_channels = get_number_channels(this->color_space);

        for(uint8_t i = 0; i < current_channels; i++)
            delete[] channels[i]; 
        delete[] channels; 
        this->channels = nullptr;
    }

    // copia os valores numéricos da nova imagem
    this->height = other.height;
    this->width  = other.width;
    this->color_space = other.color_space;
    
    uint8_t number_channels = get_number_channels(this->color_space); 

    // aloca e copia os dados se a outra imagem possuir canais válidos
    if(other.channels != nullptr)
    {
        this->channels = new uint8_t*[number_channels]; 
        int elements = this->height * this->width; 

        for(uint8_t i = 0; i < number_channels; i++)
        {
            this->channels[i] = new uint8_t[elements]; 
            memcpy(this->channels[i], other.channels[i], elements * sizeof(uint8_t)); 
        }
    }
    else {
        this->channels = nullptr;
    }

    return *this; 
}

Image Image::operator+(Image& other) {
    Image copy(*this);

    uint16_t height = this->height;
    uint16_t width = this->width;
    uint8_t number_channels = get_number_channels(this->color_space);
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

Image Image::operator-(Image& other) {
    Image copy(*this);

    uint16_t height = this->height;
    uint16_t width = this->width;
    uint8_t number_channels = get_number_channels(this->color_space);
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

Image Image::operator&(Image& other) {
    Image copy(*this);

    uint16_t height = this->height;
    uint16_t width = this->width;
    uint8_t number_channels = get_number_channels(this->color_space);
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

Image Image::operator|(Image& other) {
    Image copy(*this);

    uint16_t height = this->height;
    uint16_t width = this->width;
    uint8_t number_channels = get_number_channels(this->color_space);
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

Image Image::operator~() {
    Image copy(*this);

    int height = this->height;
    int width = this->width;
    uint8_t number_channels = get_number_channels(this->color_space);
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

Image Image::operator*(const double mult) {
    Image copy(*this);

    int height = this->height;
    int width = this->width;
    uint8_t number_channels = get_number_channels(this->color_space);
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

Image Image::operator/(const double div) {
    Image copy(*this);

    int height = this->height;
    int width = this->width;
    uint8_t number_channels = get_number_channels(this->color_space);
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