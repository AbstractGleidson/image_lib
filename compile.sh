#!/bin/zsh
g++ -I  include src/core/*.cpp src/*.cpp -o bin/app.exe 
cd bin
./app.exe ../assets/teste.bmp ../assets/teste_write.bmp
cd ..