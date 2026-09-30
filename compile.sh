#!/bin/zsh
g++ -I  include src/core/bmp/*.cpp src/*.cpp -o bin/app.exe 
cd bin
./app.exe ../assets/lena_gray.bmp ../assets/polen.bmp ../assets/einsten.bmp ../assets/equalize_lena.bmp ../assets/equalize_polen.bmp ../assets/esp_polen.bmp ../assets/esp_lena.bmp