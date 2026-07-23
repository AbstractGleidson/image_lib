#include <perl.hpp>

void Perl::set(uint8_t r, uint8_t g, uint8_t b){
    this->red = r;
    this->green = g;
    this->blue = b;
}

uint8_t Perl::getRed() { 
    return this->red; 
}

uint8_t Perl::getGreen() { 
    return this->green; 
}

uint8_t Perl::getBlue() { 
    return this->blue; 
}

void Perl::setRed(uint8_t r) { 
    this->red = r; 
}

void Perl::setGreen(uint8_t g) { 
    this->green = g; 
}

void Perl::setBlue(uint8_t b) { 
    this->blue = b; 
}