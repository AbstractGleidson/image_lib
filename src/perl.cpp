#include <perl.hpp>

void Perl::set(uint8_t r, uint8_t g, uint8_t b){
    this->red = r;
    this->green = g;
    this->blue = b;
}

uint8_t Perl::get_red() { 
    return this->red; 
}

uint8_t Perl::get_green() { 
    return this->green; 
}

uint8_t Perl::get_blue() { 
    return this->blue; 
}

void Perl::set_red(uint8_t r) { 
    this->red = r; 
}

void Perl::set_green(uint8_t g) { 
    this->green = g; 
}

void Perl::set_blue(uint8_t b) { 
    this->blue = b; 
}