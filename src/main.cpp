#include <iostream>
#include <core.hpp>
#include <fstream>

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Uso: %s <path_read.bmp> <path_write.bmp>\n", argv[0]);
        return 1;
    }

    Image image;

    read_bmp(argv[1], image, false);

    uint8_t *perl = new uint8_t[image.get_number_channels()];

    //std::vector<int> frequency = image.hist(0);

    //image.show_hist();

    Image image_bin = image.equalize(0);

    image_bin.show_hist();
    image.show_hist();

    write_bmp(argv[2], image_bin, false);

    return 0;
}
