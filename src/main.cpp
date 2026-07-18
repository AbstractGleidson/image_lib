#include <stdio.h>
#include <image.hpp>

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Uso: %s <path_read.bmp> <path_write.bmp>\n", argv[0]);
        return 1;
    }

    Image image;
    image.perls = NULL; // Inicializa o ponteiro como nulo

    int resultado = read_bmp(argv[1], &image);
    
    if (resultado == 1) {
        print_head_image(&image);
        print_matriz_image(&image, 3);
        write_bmp(argv[2], &image);
    } else {
        printf("Erro ao ler o arquivo BMP (Erro codigo: %d).\n", resultado);
    }

    return 0;
}
