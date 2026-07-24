#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <image.hpp>
#include <utils.hpp>

// Realiza a leitura de uma imagem .bmp.
//
// Param:
// <char* path_image>: Caminho da imagem à ser lida.
// <Image* image>:  Onde salvar o resuldado da leitura.
//
// return:
//  FILENOTFOUND  - Caso não consiga abrir o arquivo.
//  FORMATNOTBMP - Caso o formato da imagem não seja .bmp.
//  SUCCESS  - Caso ocorra corretamente a leitura.  
ImageAcessStatus read_bmp(const char*, Image&, const bool);

// Salva uma imagem .bmp
//
// Param:
// <char *path_image>: Caminho para salvar a imagem.
// <Image *image>: Imagem á ser salva.
//
// Return: 
// FILEOPENERROR - Caso não consiga criar o arquivo.
// SUCCESS - Caso ocorra com sucesso o salvamento da imagem.
ImageAcessStatus write_bmp(const char*, Image&, const bool);