#include <image.hpp>

// Realiza a leitura de uma imagem .bmp.
//
// Param:
// <char* path_image>: Caminho da imagem à ser lida.
// <Image* image>: Caminho onde salvar o resuldado da leitura.
//
// return:
//  0  - Caso não consiga abrir o arquivo.
//  -1 - Caso o formato da imagem não seja .bmp.
//  1  - Caso ocorra corretamente a leitura.  
int read_bmp(char* path_image, Image *image)
{
    FILE *bmp_image = fopen(path_image, "rb"); 
    if(bmp_image == NULL) return 0; // caso não consiga abrir o aquivo 

    // litura da estrutura de uma imagem no formato .bmp
    fread(&image->file_image.type_file, sizeof(image->file_image.type_file), 1, bmp_image);
    fread(&image->file_image.size_file_bytes, sizeof(image->file_image.size_file_bytes), 1, bmp_image);
    fseek(bmp_image, sizeof(uint32_t), SEEK_CUR); // pula reserved1 e reserved2, cada com 16 bits
    fread(&image->file_image.offset_data_field, sizeof(image->file_image.offset_data_field), 1, bmp_image);
    fseek(bmp_image, sizeof(uint32_t), SEEK_CUR); // pula o tamanho do cabeçalho do bitmap, sempre é 40 bits
    fread(&image->bitmap.width, sizeof(image->bitmap.width  ), 1, bmp_image);
    fread(&image->bitmap.height, sizeof(image->bitmap.height), 1, bmp_image);
    fseek(bmp_image, sizeof(uint16_t), SEEK_CUR); // pula quantidade de planos na imagem, sempre 1
    fread(&image->bitmap.size_pixel, sizeof(image->bitmap.size_pixel), 1, bmp_image);
    fread(&image->bitmap.compression_image, sizeof(uint32_t), 1, bmp_image); // tipo de compressão da imagem pode ser RLE
    fread(&image->bitmap.size_image, sizeof(image->bitmap.size_image), 1, bmp_image);
    fread(&image->bitmap.resolution_horizontal_image, sizeof(image->bitmap.resolution_horizontal_image), 1, bmp_image);
    fread(&image->bitmap.resolution_vertical_image, sizeof(image->bitmap.resolution_vertical_image), 1, bmp_image);
    fread(&image->bitmap.colors_scale_image, sizeof(image->bitmap.colors_scale_image), 1, bmp_image);
    fread(&image->bitmap.colors_scale_image_used, sizeof(image->bitmap.colors_scale_image_used), 1, bmp_image);

    // verifica extensão "BM" para formato do arquivo
    if(image->file_image.type_file != 0x4D42) {
        fclose(bmp_image);
        return -1; // caso não seja uma imagem .bmp
    }

    // aloca memoria para a matriz de pixels da imagem
    image->perls = (Perl *) malloc(sizeof(Perl) * image->bitmap.height * image->bitmap.width);
    
    // move ponteiro do arquivo para área de dados do arquivo
    fseek(bmp_image, image->file_image.offset_data_field, SEEK_SET);

    // padding (ajuste de 4 bytes por linha do BMP), para alinhas em multiplos de 4
    int padding = (4 - (image->bitmap.width    * 3) % 4) % 4;
    Perl *matriz = image->perls; // Endereço base da matriz de pixels        

    for(int i = 0; i < image->bitmap.height; i++)
    {
        for(int j = 0; j < image->bitmap.width; j++)
        {
            int offset =  ((i * image->bitmap.width) + j);// deslocamento para cada perl 

            fread(&(matriz + offset)->red, sizeof(uint8_t), 1, bmp_image);
            fread(&(matriz + offset)->green, sizeof(uint8_t), 1, bmp_image);
            fread(&(matriz + offset)->blue, sizeof(uint8_t), 1, bmp_image);
        }
        // pula os bytes de preenchimento (padding) no fim da linha, se existirem
        fseek(bmp_image, padding, SEEK_CUR);
    }

    fclose(bmp_image);
    return 1; // conseguiu ler a imagem
}

// Salva uma imagem .bmp
//
// Param:
// <char *path_image>: Caminho para salvar a imagem.
// <Image *image>: Imagem á ser salva.
//
// Return: 
// 0 - Caso não consiga criar o arquivo.
// 1 - Caso ocorra com sucesso o salvamento da imagem.
int write_bmp(char* path_image, Image *image)
{
    FILE *bmp_image = fopen(path_image, "wb"); 
    if(bmp_image == NULL) return 0; // Não conseguiu criar o arquivo

    // cabeçalho do arquivo (HeadFile)
    fwrite(&image->file_image.type_file, sizeof(image->file_image.type_file), 1, bmp_image);
    fwrite(&image->file_image.size_file_bytes, sizeof(image->file_image.size_file_bytes), 1, bmp_image);
    
    uint16_t reserved = 0;
    fwrite(&reserved, sizeof(uint16_t), 1, bmp_image); // reserved1
    fwrite(&reserved, sizeof(uint16_t), 1, bmp_image); // reserved2
    
    fwrite(&image->file_image.offset_data_field, sizeof(image->file_image.offset_data_field), 1, bmp_image);

    // cabeçalho do bitmap (HeadBitMap)
    uint32_t dib_header_size = 40; // Tamanho padrão do cabeçalho 
    fwrite(&dib_header_size, sizeof(uint32_t), 1, bmp_image);
    
    fwrite(&image->bitmap.width, sizeof(image->bitmap.width), 1, bmp_image);
    fwrite(&image->bitmap.height, sizeof(image->bitmap.height), 1, bmp_image);
    
    uint16_t planes = 1;
    fwrite(&planes, sizeof(uint16_t), 1, bmp_image); // planes sempre 1
    
    fwrite(&image->bitmap.size_pixel, sizeof(image->bitmap.size_pixel), 1, bmp_image);
    
    uint32_t compression = 0; // sem compressão
    fwrite(&compression, sizeof(uint32_t), 1, bmp_image);
    
    fwrite(&image->bitmap.size_image, sizeof(image->bitmap.size_image), 1, bmp_image);
    fwrite(&image->bitmap.resolution_horizontal_image, sizeof(image->bitmap.resolution_horizontal_image), 1, bmp_image);
    fwrite(&image->bitmap.resolution_vertical_image, sizeof(image->bitmap.resolution_vertical_image), 1, bmp_image);
    fwrite(&image->bitmap.colors_scale_image, sizeof(image->bitmap.colors_scale_image), 1, bmp_image);
    fwrite(&image->bitmap.colors_scale_image_used, sizeof(image->bitmap.colors_scale_image_used), 1, bmp_image);

    // posiciona o ponteiro do arquivo onde deve ser salvo a matriz de pixels
    fseek(bmp_image, image->file_image.offset_data_field, SEEK_SET);

    // padding do final da linha 
    int padding = (4 - (image->bitmap.width * 3) % 4) % 4;
    uint8_t padding_byte = 0;
    Perl *matriz = image->perls;

    for(int i = 0; i < image->bitmap.height; i++)
    {
        for(int j = 0; j < image->bitmap.width; j++)
        {
            int offset = ((i * image->bitmap.width) + j); // deslocamento em relação a cada perl

            fwrite(&(matriz + offset)->red, sizeof(uint8_t), 1, bmp_image);
            fwrite(&(matriz + offset)->green, sizeof(uint8_t), 1, bmp_image);
            fwrite(&(matriz + offset)->blue, sizeof(uint8_t), 1, bmp_image);
        }
        
        // alinha o arquivo para multiplos de 4
        for(int p = 0; p < padding; p++) {
            fwrite(&padding_byte, sizeof(uint8_t), 1, bmp_image); // completa com 0 para alinhar 
        }
    }

    fclose(bmp_image);
    return 1; // escreveu a imagem corretamente 
}

// Calcula o minimo entre dois números
//
// Param:
// <uint32_t a>: Primeiro número.
// <uint32_t b>: Segundo número a ser comparado.
//
// Return:
// menor inteiro entre a e b
uint32_t min(uint32_t a, uint32_t b)
{
    return (a > b? b: a);
}

// Mostra a matriz de pixels da imagem
//
// Param:
// <Image *image>: Image que deve ser exibida a matriz
// <uint32_t head>: Número minimo de linhas a ser exibido 
void print_matriz_image(Image *image, uint32_t head){
    Perl *matriz = image->perls;
    int show_head = head > 0? min(image->bitmap.height, head): image->bitmap.height;

    for(int i = 0; i < show_head; i++)
    {
        printf("\n[");
        for(int j = 0; j < image->bitmap.width; j++)
        {
            int offset =  sizeof(Perl) * (i + (j * image->bitmap.width));  
            printf("(%d, %d, %d)\n", (matriz + offset)->red, (matriz + offset)->green, (matriz + offset)->blue);
        }
        printf("]\n");
    }
}

// Exibi o cabeçalho da imagem 
// 
// Param:
// <Image *image>: Imagem que se deve exibir o cabeçalho
void print_head_image(Image *image)
{
    printf("--------------------- Cabeçalho da Image ---------------------\n");
    printf("Tamanho do arquivo em bytes: %d\n", image->file_image.size_file_bytes);
    printf("Largura da imagem: %d\n", image->bitmap.width);
    printf("Altura da imagem: %d\n", image->bitmap.height);
    printf("Tamanho do pixel em bits: %hu\n", image->bitmap.size_pixel);
    printf("Escala de cores da imagem: %d\n", image->bitmap.colors_scale_image_used);
    printf("Tipo de compressão da imagem: %hu\n", image->bitmap.compression_image);
    printf("---------------------------------------------------------------\n");
}