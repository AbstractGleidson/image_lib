#include <iostream>
#include <core.hpp>
#include <fstream>

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Uso: %s <path_read.bmp> <path_write.bmp>\n", argv[0]);
        return 1;
    }

    Image lena, sankara, eins;

    read_bmp("../assets/lena_gray.bmp", lena, false);
    read_bmp("../assets/teste.bmp", sankara, true);
    read_bmp("../assets/einstein.bmp", eins, true);

    Image lena_equalize = lena.equalize(0);
    Image sankara_equalize = sankara.equalize(0);

    std::vector<double> eins_hist = eins.hist(0, true);

    Image lena_esp = lena.equalize_esp(0, eins_hist);
    Image sankara_esp = sankara.equalize_esp(0, eins_hist);


    lena.show_hist();
    sankara.show_hist();
    eins.show_hist();

    lena_equalize.show_hist();
    sankara_equalize.show_hist();

    lena_esp.show_hist();
    sankara_esp.show_hist();

    // //Image image_bin = image.equalize(0);

    write_bmp("../assets/equalize_lena.bmp", lena_equalize, false);
    write_bmp("../assets/equalize_sankara.bmp", sankara_equalize, false);
    write_bmp("../assets/esp_lena.bmp", lena_esp, false);
    write_bmp("../assets/esp_sankara.bmp", sankara_esp, false);

    return 0;
}
