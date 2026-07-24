#include <stdio.h>
#include <core.hpp>

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Uso: %s <path_read.bmp> <path_write.bmp>\n", argv[0]);
        return 1;
    }

    Image image;

    int response_read = read_bmp(argv[1], image);

    if (response_read == ImageAcessStatus::SUCCESS) {
        write_bmp(argv[2], image);

        printf("Altura: %d\nLargura: %d\nCanais: %d\n", image.get_height(), image.get_width(), image.get_channels());
        
        Perl perl = image.get_perl(10, 10); 

        printf("pixel (10, 10): (%d, %d, %d)\n", perl.getRed(), perl.getGreen(), perl.getBlue());
    } else {
        printf("Erro ao ler o arquivo BMP (Erro codigo: %d).\n", response_read);
    }

    return 0;
}
