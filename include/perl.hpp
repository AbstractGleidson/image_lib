#include <stdint.h>
#include <utils.hpp>

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
        uint8_t get_red();
        uint8_t get_green();
        uint8_t get_blue();

        // define cores personalizadas
        void set(uint8_t r, uint8_t g, uint8_t b);

        // sets para as cores
        void set_green(uint8_t g);
        void set_red(uint8_t r);
        void set_blue(uint8_t b);

        Perl& operator=(const Perl& other)
        {
            if(this == &other) return *this;
            
            this->red = other.red;
            this->green = other.green;
            this->blue = other.blue;

            return *this;
        }

        // soma de dois perls
        Perl operator+(const Perl& other) const {
            Perl result = Perl();

            result.red = min((int) this->red + other.red, 255);
            result.green = min((int) this->green + other.green, 255);
            result.blue = min((int) this->blue + other.blue, 255);
            
            return result;
        }
        
        // soma de dois perls
        Perl& operator+=(const Perl& other)
        {
            this->red = max(min((int) this->red + other.red, 255), 0);
            this->green = max(min((int) this->green + other.green, 255), 0);
            this->blue = max(min((int) this->blue + other.blue, 255), 0);
            
            return *this;
        }

        // subtração de dois perls
        Perl operator-(const Perl& other) const {
            Perl result = Perl();

            result.red = min(max((int) this->red - other.red, 0), 255);
            result.green = min(max((int) this->green - other.green, 0), 255);
            result.blue = min(max((int) this->blue - other.blue, 0), 255);
            
            return result;
        }

        // Decremento 
        Perl& operator-=(const Perl& other)
        {
            this->red = min(max((int) this->red - other.red, 0), 255);
            this->green = min(max((int) this->green - other.green, 0), 255);
            this->blue = min(max((int) this->blue - other.blue, 0), 255);
            
            return *this;
        }

        // multiplicação de um perl por inteiro 
        Perl operator*(const uint8_t number) const {
            Perl result = Perl();

            result.red = max(min((int) this->red * number, 255), 0);
            result.green = max(min((int) this->green * number, 255), 0);
            result.blue = max(min((int) this->blue * number, 255), 0);

            return result;
        }

        // Incremento multiplicativo
        Perl& operator*=(const uint8_t number)
        {
            this->red = max(min((int) this->red * number, 255), 0);
            this->green = max(min((int) this->green * number, 255), 0);
            this->blue = max(min((int) this->blue * number, 255), 0);
            
            return *this;
        }

        // multiplicação de um perl por inteiro 
        Perl operator/(const uint8_t number) const {
            Perl result = Perl();
            
            result.red = max(min((int) this->red / number, 255), 0);
            result.green = max(min((int) this->green / number, 255), 0);
            result.blue = max(min((int) this->blue / number, 255), 0);
            
            return result;
        }

        // Incremento multiplicativo
        Perl& operator/=(const uint8_t number)
        {
            this->red = max(min((int) this->red / number, 255), 0);
            this->green = max(min((int) this->green / number, 255), 0);
            this->blue = max(min((int) this->blue / number, 255), 0);
            
            return *this;
        }

        // not bitwise 
        Perl operator~() const {
            return Perl(255, 255, 255) - *this;
        }

        // and bitwise  
        Perl operator&(const Perl& other) const {
            Perl result; 

            result.red = this->red & other.red;
            result.green = this->green & other.green;
            result.blue = this->blue & other.blue; 
        
            return result;
        }

        // or bitewise 
        Perl operator|(const Perl& other) const {
            Perl result; 

            result.red = this->red | other.red;
            result.green = this->green | other.green;
            result.blue = this->blue | other.blue; 

            return result;
        }
};