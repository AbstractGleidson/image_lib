#include <image.hpp>

// construtor padrão
Image::Image()
{
    this->height = 0; 
    this->width = 0; 
    this->channels = 0; 
    this->perls = nullptr; // inicializa o ponteiro da matriz de pixels como nulo 
}

// construtor de cópia, memótodo copy
Image::Image(const Image& other)
{
    // cópia valores numericos
    this->height = other.height;
    this->width  = other.width;
    this->channels = other.channels;

    if(other.perls != nullptr)
    {
        int size_perls = this->height * this->width;

        perls = new Perl[size_perls]; // aloca memória para a matriz de pixels na heap
        
        memcpy(perls, other.perls, sizeof(Perl) * size_perls); // cópia o bloco de memória 
    }
    else{
        this->perls = nullptr;
    }
}

// destrutor 
Image::~Image()
{
    if(perls != nullptr)
    {
        delete[] perls; // libera memória da heap
    }
}


// operador de atribuição
Image& Image::operator=(const Image& other) {
    if (this == &other) return *this; // evita auto atribuição

    // evita atribuição para referência null
    if (perls != nullptr) delete[] perls;

    // copia os valores numericos
    this->height = other.height;
    this->width = other.width;
    this->channels = other.channels;

    // cópia matriz de pixels
    if (other.perls != nullptr) {
        int total_perls = this->width * this->height;
        
        perls = new Perl[total_perls]; // aloca memória na heap para matriz de pixels
        
        memcpy(perls, other.perls, sizeof(Perl) * total_perls); // cópia o bloco de memoria da matriz de pixels
    } else {
        perls = nullptr;
    }

    return *this; // retorna a referẽncia 
}

void Image::print_matriz_image(const uint32_t head) {
    uint32_t show_head = head > 0 ? min(this->height, head) : this->height;

    for(uint32_t i = 0; i < show_head; i++) {
        printf("\n[");
        for(uint32_t j = 0; j < this->width; j++) {
            Perl& p = get_perl(j, i);
            printf("(%d, %d, %d)", p.get_red(), p.get_green(), p.get_blue());
            if (j < this->width - 1) printf(", ");
        }
        printf("]\n");
    }
}

void Image::print_head_image() {
    printf("--------------------- Cabeçalho da Image ---------------------\n");
    printf("Largura da imagem: %d\n", this->get_width());
    printf("Altura da imagem: %d\n", this->get_height());
    printf("Quantidade canais da imagem: %hu\n", this->get_channels());
    printf("---------------------------------------------------------------\n");
}

Image Image::get_red_image() {
    Image copy(*this); 
    for(int i = 0; i < copy.get_height(); i++) {
        for(int j = 0; j < copy.get_width(); j++) {
            copy.get_perl(j, i).set_green(0);
            copy.get_perl(j, i).set_blue(0);
        }
    }
    return copy;
}

Image Image::get_green_image() {
    Image copy(*this); 
    for(int i = 0; i < copy.get_height(); i++) {
        for(int j = 0; j < copy.get_width(); j++) {
            copy.get_perl(j, i).set_red(0);
            copy.get_perl(j, i).set_blue(0);
        }
    }
    return copy;
}

Image Image::get_blue_image() {
    Image copy(*this); 
    for(int i = 0; i < copy.get_height(); i++) {
        for(int j = 0; j < copy.get_width(); j++) {
            copy.get_perl(j, i).set_red(0);
            copy.get_perl(j, i).set_green(0);
        }
    }
    return copy;
}

Image Image::negative()
{
    Image copy(*this); // cria cópia 

    for(int i = 0; i < copy.get_height(); i++)
    {
        for(int j = 0; j < copy.get_width(); j++)
        {
            Perl perl = copy.get_perl(j, i);
            copy.set_perl(j, i, ~perl);
        }
    }

    return copy;
}

Image Image::gray_scale_mean()
{
    Image copy(*this);

    for(int i = 0; i < copy.get_height(); i++)
    {
        for(int j = 0; j < copy.get_width(); j++)
        {
            Perl perl = copy.get_perl(j, i);

            int mean = (perl.get_red() + perl.get_green() + perl.get_blue()) / 3;

            copy.set_perl(
                j, i, 
                Perl(
                    mean,
                    mean,
                    mean
                )
            );
        }
    }

    return copy;
}

Image Image::gray_scale()
{
    Image copy(*this);

    for(int i = 0; i < copy.get_height(); i++)
    {
        for(int j = 0; j < copy.get_width(); j++)
        {
            Perl perl = copy.get_perl(j, i);

            // pesos de acordo com a sensibilidade do olho humano 
            double weight_red = 0.2126, weight_green = 0.7152, weight_blue = 0.0722; 

            int mean = ((perl.get_red() * weight_red) + (perl.get_green() * weight_green) + (perl.get_blue() * weight_blue)) / 3;

            copy.set_perl(
                j, i, 
                Perl(
                    mean,
                    mean,
                    mean
                )
            );
        }
    }

    return copy;
}

// converte o para o limiar: channel > thres => 255
Image Image::binary(const uint8_t thres)
{
    Image copy(*this);
    
    for(int i = 0; i < copy.get_height(); i++)
    {
        for(int j = 0; j < copy.get_width(); j++)
        {
            Perl perl = copy.get_perl(j, i);

            copy.set_perl(
                j, i, 
                Perl(
                    perl.get_red() > thres? 255 : 0,
                    perl.get_red() > thres? 255 : 0,
                    perl.get_red() > thres? 255 : 0
                )
            );
        }
    }

    return copy;
}