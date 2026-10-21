#include <iostream>
#include <core.hpp>
#include <fstream>

int main(int argc, char **argv)
{
    Image ponto_g, einstein, sankara; 

    // Realiza a leitura das imagens 
    read_bmp("../assets/bmp/ponto_g.bmp", ponto_g, true);
    read_bmp("../assets/bmp/einstein.bmp", einstein, true);
    read_bmp("../assets/bmp/sankara_h.bmp", sankara, true);

    // Converte as imagens para escala de cinza
    Image ponto_g_gray = ponto_g.RGB_TO_GRAY();
    Image sankara_gray = sankara.RGB_TO_GRAY();
    Image einstein_gray = einstein.RGB_TO_GRAY();
    
    // Equaliza o histograma da imagem ponto_g.bmp 
    Image ponto_g_gray_equalize = ponto_g_gray.equalize(0);

    // Especifica o histograma da imagem ponto_g.bmp utilizando a FDP da imagem einstein.bmp
    Image ponto_g_gray_esp_einstein = ponto_g_gray.equalize_esp(
        0,
        einstein_gray.hist(0, true)
    );

    // Especifica o histograma da imagem ponto_g.bmp utilizando a FDP da imagem sankara.bmp
    Image ponto_g_gray_esp_sankara = ponto_g_gray.equalize_esp(
        0,
        sankara_gray.hist(0, true)
    );
   
    // cria um vector para as labels dos gráficos
    std::vector<std::string> labels= {"Original", "Equalizada"};

    // Gera os gráficos de comparação da imagem original com a equalizada
    ponto_g_gray.show_hist("Histograma da Imagem Original", "", true);
    ponto_g_gray_equalize.show_hist("Histograma da Imagem Equalizada", "", true);
    ponto_g_gray.show_mult_hist(ponto_g_gray_equalize, "Histograma Original x Equalizado", labels, "", true);

    // Troca as labels 
    labels[0] = "Original";
    labels[1] = "Especificado";
    
    // Gera os gráficos de comparação entre a imagem original com a especificada com histograma da imagem einstein.bmp
    ponto_g_gray.show_hist("Histograma da Imagem Original", "", true);
    einstein.show_hist("Histograma da Imagem Einstein", "", true);
    ponto_g_gray_esp_einstein.show_hist("Histograma da Imagem Especificada com Einstein", "", true);
    einstein.show_mult_hist(ponto_g_gray_esp_einstein, "Histograma Einstein x Especificado Einstein", labels, "", true);

    // Gera os gráficos de comparação entre a imagem original com a especificada com histograma da imagem sankara.bmp
    ponto_g_gray.show_hist("Histograma da Imagem Original", "", true);
    sankara.show_hist("Histograma da Imagem Sankara", "", true);
    ponto_g_gray_esp_sankara.show_hist("Histograma da Imagem Especificada com Sankara", "", true);
    sankara.show_mult_hist(ponto_g_gray_esp_sankara, "Histograma Sankara x Especificado Sankara", labels, "", true);

    // Salva resultados
    write_bmp("../assets/results/bmp/ponto_g_gray.bmp", ponto_g_gray, false);
    write_bmp("../assets/results/bmp/ponto_g_equalized.bmp", ponto_g_gray_equalize, false);
    write_bmp("../assets/results/bmp/sankara_gray.bmp", sankara_gray, false);
    write_bmp("../assets/results/bmp/einstein_gray.bmp", einstein_gray, false);
    write_bmp("../assets/results/bmp/ponto_g_esp_sankara.bmp", ponto_g_gray_esp_sankara, false);
    write_bmp("../assets/results/bmp/ponto_g_esp_einstein.bmp", ponto_g_gray_esp_einstein, false);

    return 0;
}
