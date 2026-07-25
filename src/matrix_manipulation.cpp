#include <matrix_manipulation.hpp>

const double mean(double *matrix, const int height, const int width) {
    double mean = 0;

    for(int i; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            mean += matrix[(i * width) + j];
        }
    }

    return (mean / (height * width));
}