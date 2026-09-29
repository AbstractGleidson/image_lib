#include <utils.hpp>

int min(const int a, const int b)
{
    return (a > b? b: a);
}

int max(const int a, const int b)
{
    return (a < b? b: a);
}

const int index_image(const int x, const int y, const int width) {
    return (width * x + y); 
}