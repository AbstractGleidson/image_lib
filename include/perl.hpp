#include <stdint.h>
#include <vector>

void sum_perl(uint8_t *perl_1, uint8_t *perl_2, uint8_t *perl_dst, const uint8_t channels);

void sub_perl(uint8_t *perl_1, uint8_t *perl_2, uint8_t *perl_dst, const uint8_t channels);

void div_perl(uint8_t *perl_1,  uint8_t *perl_dst, const double div, const uint8_t channels);

void mul_perl(uint8_t *perl_1, uint8_t *perl_dst, const double mult, const uint8_t channels);

void bitwise_and_perl(uint8_t *perl_1, uint8_t *perl_2, uint8_t *perl_dst, const uint8_t channels);

void bitwise_or_perl(uint8_t *perl_1, uint8_t *perl_2, uint8_t *perl_dst, const uint8_t channels);

void bitwise_not_perl(uint8_t *perl_1, uint8_t *perl_dst, const uint8_t channels);


std::vector<uint8_t> sum_perl(uint8_t *perl_1, uint8_t *perl_2);

std::vector<uint8_t> sub_perl(uint8_t *perl_1, uint8_t *perl_2);

std::vector<uint8_t> div_perl(uint8_t *perl_1, const double div);

std::vector<uint8_t> mul_perl(uint8_t *perl_1, const double mult);

std::vector<uint8_t> bitwise_and_perl(uint8_t *perl_1, uint8_t *perl_2);

std::vector<uint8_t> bitwise_or_perl(uint8_t *perl_1, uint8_t *perl_2);

std::vector<uint8_t> bitwise_not_perl(uint8_t *perl_1);