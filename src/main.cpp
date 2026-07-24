#include <stdio.h>
#include <core.hpp>

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Uso: %s <path_read.bmp> <path_write.bmp>\n", argv[0]);
        return 1;
    }

    Image image;

    int response_read = read_bmp(argv[1], image, true);
    
    if (response_read == ImageAcessStatus::SUCCESS) {
       
        //Image negative = image.negative();

        Image gray = image.gray_scale_mean();
        Image binary = image.binary(125);

        // salva resultados
        //write_bmp("../assets/negativer_image.bmp", negative, true);
        write_bmp("../assets/gray_image.bmp", gray, false);
        write_bmp("../assets/binary_image.bmp", binary, false);
    } else {
        printf("Erro ao ler o arquivo BMP (Erro codigo: %d).\n", response_read);
    }

    return 0;
}
