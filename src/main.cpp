#include <iostream>
#include <core.hpp>
#include <fstream>

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Uso: %s <path_read.bmp> <path_write.bmp>\n", argv[0]);
        return 1;
    }

    Image lena, polen;

    read_bmp(argv[1], lena, true);
    //read_bmp(argv[2], polen, false);

    // std::vector<double> fdp_esp = lena.hist(0, true);

    // Image einsten_esp = einsten.equalize_esp(0, fdp_esp);

    // Image lena_equalize = lena.equalize(0);

    // lena.show_hist();
    // einsten.show_hist();
    // einsten_esp.show_hist();
    // lena_equalize.show_hist();

    // //Image image_bin = image.equalize(0);

    //write_bmp(argv[3], lena_equalize, false);

    write_bmp(argv[2], lena, true);
    return 0;
}
