#!/bin/zsh
g++ -I  include src/core/bmp/*.cpp src/*.cpp -o bin/app.exe 
cd bin
./app.exe ../assets/blue_channel.bmp ../assets/equalize_blue_channel.bmp
cd ..