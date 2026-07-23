#include <utils.hpp>

// Calcula o minimo entre dois números
//
// Param:
// <uint32_t a>: Primeiro número.
// <uint32_t b>: Segundo número a ser comparado.
//
// Return:
// menor inteiro entre a e b
uint32_t min(const uint32_t a, const uint32_t b)
{
    return (a > b? b: a);
}