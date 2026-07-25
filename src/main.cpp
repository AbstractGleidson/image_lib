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

        Image gray = image.mean_gray_scale();

        Image blur = image.median_blur(10);
        //Image blur = gray.median_blur(5);

        Image edges = (gray - blur) * 3;

        // salva resultados
        write_bmp("../assets/edges_gray_2.bmp", edges, false);
        // write_bmp("../assets/blur_color_median.bmp", blur_color, true);
        // write_bmp("../assets/blur_gray_median.bmp", blur_gray, false);
    } else {
        printf("Erro ao ler o arquivo BMP (Erro codigo: %d).\n", response_read);
    }

    return 0;
}
