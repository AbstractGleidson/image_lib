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
            printf("(%d, %d, %d)", p.getRed(), p.getGreen(), p.getBlue());
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
            copy.get_perl(j, i).setGreen(0);
            copy.get_perl(j, i).setBlue(0);
        }
    }
    return copy;
}

Image Image::get_green_image() {
    Image copy(*this); 
    for(int i = 0; i < copy.get_height(); i++) {
        for(int j = 0; j < copy.get_width(); j++) {
            copy.get_perl(j, i).setRed(0);
            copy.get_perl(j, i).setBlue(0);
        }
    }
    return copy;
}

Image Image::get_blue_image() {
    Image copy(*this); 
    for(int i = 0; i < copy.get_height(); i++) {
        for(int j = 0; j < copy.get_width(); j++) {
            copy.get_perl(j, i).setRed(0);
            copy.get_perl(j, i).setGreen(0);
        }
    }
    return copy;
}