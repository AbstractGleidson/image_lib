#include <perl.hpp>
#include <stdint.h>

// soma dois perls
void sum_perl(uint8_t *perl_1, uint8_t *perl_2, uint8_t *perl_dst, const uint8_t channels)
{
    if (perl_dst == nullptr)
        perl_dst = perl_1; // soma e coloca o resultado no perl_1

    for(int i = 0; i < channels; i++)
    {
        perl_dst[i] = std::min(std::max(perl_1[i] + perl_2[i], 0), 255);
    }
}

// subtrai dois perls
void sub_perl(uint8_t *perl_1, uint8_t *perl_2, uint8_t *perl_dst, const uint8_t channels)
{
    if (perl_dst == nullptr)
        perl_dst = perl_1; // subtrai e coloca o resultado no perl_1

    for(int i = 0; i < channels; i++)
    {
        perl_dst[i] = std::min(std::max(perl_1[i] - perl_2[i], 0), 255);
    }
}

// soma um perl com um escalar
void sum_perl_number(uint8_t *perl_1, const uint8_t number, uint8_t *perl_dst, const uint8_t channels)
{
    if (perl_dst == nullptr)
        perl_dst = perl_1; // soma e coloca o resultado no perl_1

    for(int i = 0; i < channels; i++)
    {
        perl_dst[i] = std::min(std::max(perl_1[i] + number, 0), 255);
    }
}

// subtrai um perl com um escalar
void sub_perl_number(uint8_t *perl_1, const uint8_t number, uint8_t *perl_dst, const uint8_t channels)
{
    if (perl_dst == nullptr)
        perl_dst = perl_1; // soma e coloca o resultado no perl_1

    for(int i = 0; i < channels; i++)
    {
        perl_dst[i] = std::min(std::max(perl_1[i] - number, 0), 255);
    }
}

// multiplica um perl por um escalar 
void mul_perl(uint8_t *perl_1, uint8_t *perl_dst, const double number, const uint8_t channels)
{
    if (perl_dst == nullptr)
        perl_dst = perl_1; // multiplica e coloca o resultado no perl_1

    for(int i = 0; i < channels; i++)
    {
        perl_dst[i] = std::min(std::max((int) (perl_1[i] * number), 0), 255);
    }
}

void div_perl(uint8_t *perl_1, uint8_t *perl_dst, const double number, const uint8_t channels)
{
    if (perl_dst == nullptr)
        perl_dst = perl_1; // divide e coloca o resultado no perl_1

    if (number == 0.0) return; // divisão por zero

    for(int i = 0; i < channels; i++)
    {
        perl_dst[i] = std::min(std::max((int) (perl_1[i] / number), 0), 255);
    }
}

void bitwise_and_perl(uint8_t *perl_1, uint8_t *perl_2, uint8_t *perl_dst, const uint8_t channels)
{
    if (perl_dst == nullptr)
        perl_dst = perl_1; // bitwise and e coloca o resultado no perl_1

    for(int i = 0; i < channels; i++)
    {
        perl_dst[i] = perl_1[i] & perl_2[i];
    }
}

void bitwise_or_perl(uint8_t *perl_1, uint8_t *perl_2, uint8_t *perl_dst, const uint8_t channels)
{
    if (perl_dst == nullptr)
        perl_dst = perl_1; // bitwise or e coloca o resultado no perl_1

    for(int i = 0; i < channels; i++)
    {
        perl_dst[i] = perl_1[i] | perl_2[i];
    }
}

void bitwise_not_perl(uint8_t *perl_1, uint8_t *perl_dst, const uint8_t channels)
{
    if (perl_dst == nullptr)
        perl_dst = perl_1; // bitwise not e coloca o resultado no perl_1

    for(int i = 0; i < channels; i++)
    {
        perl_dst[i] = ~perl_1[i];
    }
}