#include <stdint.h>

// Elemento minimo de imagem
class Perl 
{
    private:
        uint8_t red; 
        uint8_t green;
        uint8_t blue;

    public:
        // construtor para perls pretos
        Perl(){
            this->red = 0;
            this->green = 0;
            this->blue = 0;
        }

        // construtor personalizado, com parâmetros
        Perl(uint8_t r, uint8_t g, uint8_t b) {
            this->red = r;
            this->green = g;
            this->blue = b;
        }

        // gets para as cores
        uint8_t getRed();
        uint8_t getGreen();
        uint8_t getBlue();

        // define cores personalizadas
        void set(uint8_t r, uint8_t g, uint8_t b);

        // sets para as cores
        void setGreen(uint8_t g);
        void setRed(uint8_t r);
        void setBlue(uint8_t b);
};
