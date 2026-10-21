#include <image.hpp>

Image Image::negative()
{
    Image copy = ~(*this); // aplica o filtro negativo no objeto e atribui em copy  
    return copy;
}

Image Image::binary(const uint8_t thres)
{
    Image copy(*this);

    uint16_t height = this->height;
    uint16_t width = this->width;
    uint8_t number_channels = get_number_channels(this->color_space);

    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < height; i++)
    {
        uint8_t *perl = new uint8_t[number_channels];

        for(int j = 0; j < width; j++)
        {
            this->get_perl(i, j, perl);

            for(uint8_t c = 0; c < number_channels; c++)
                perl[c] = (perl[c] > thres) ? 255 : 0;
            
            copy.set_perl(i, j, perl);
        }

        delete[] perl; 
    }

    return copy;
}

Image Image::mean_blur(const uint16_t size_kernel) {
    Image copy(*this);

    // Dimensões da imagem
    uint16_t height = this->get_height();
    uint16_t width = this->get_width();
    uint8_t number_channels = get_number_channels(this->color_space);

    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < height; i++)
    {
        uint8_t *neighbor_perl = new uint8_t[number_channels];
        uint8_t *mean_p = new uint8_t[number_channels];
        
        double *means = new double[number_channels];

        for(int j = 0; j < width; j++)
        {
            // Reseta o acumulador e contadores para o pixel atual
            for(uint8_t c = 0; c < number_channels; c++) 
                means[c] = 0.0;
            
            int valid_elements = 0;

            // Varredura da vizinhança (Kernel)
            for(int offset_i = -size_kernel; offset_i <= size_kernel; offset_i++)
            {
                for(int offset_j = -size_kernel; offset_j <= size_kernel; offset_j++)
                {
                    // Calcula as posições da vizinhança (y = linha/altura, x = coluna/largura)
                    int neighbor_y = i + offset_i;
                    int neighbor_x = j + offset_j;

                    // Verifica se é uma posição válida dentro da imagem
                    if(neighbor_y >= 0 && neighbor_y < height && neighbor_x >= 0 && neighbor_x < width)
                    {
                        this->get_perl(neighbor_y, neighbor_x, neighbor_perl);

                        // Incrementa as somas para cada canal
                        for(uint8_t c = 0; c < number_channels; c++)
                            means[c] += neighbor_perl[c];
                        
                        valid_elements++;
                    }
                }
            }

            // Calcula a média final e converte para uint8_t 
            if(valid_elements > 0)
                for(uint8_t c = 0; c < number_channels; c++)
                    mean_p[c] = (uint8_t)(means[c] / valid_elements);
                

            // Substitui os valores na imagem 
            copy.set_perl(i, j, mean_p);
        }

        delete[] neighbor_perl;
        delete[] mean_p;
        delete[] means;
    }

    return copy;
}

Image Image::median_blur(const uint16_t size_kernel) {
    Image copy(*this);

    uint16_t height = this->get_height();
    uint16_t width = this->get_width();
    uint8_t number_channels = get_number_channels(this->color_space);

    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < height; i++)
    {
        uint8_t *neighbor_perl = new uint8_t[number_channels];
        uint8_t *median_p = new uint8_t[number_channels];

        for(int j = 0; j < width; j++)
        {
            // Cria um vetor para armazenar os valores de intensidade da vizinhança de cada canal
            std::vector<std::vector<uint8_t>> medians(number_channels);

            for(int offset_i = -size_kernel; offset_i <= size_kernel; offset_i++)
            {
                for(int offset_j = -size_kernel; offset_j <= size_kernel; offset_j++)
                {
                    // Calcula as posições da vizinhança (y = linha/altura, x = coluna/largura)
                    int neighbor_y = i + offset_i;
                    int neighbor_x = j + offset_j;

                    // Verifica se é uma posição válida
                    if(neighbor_y >= 0 && neighbor_y < height && neighbor_x >= 0 && neighbor_x < width)
                    {
                        // Pega o perl da vizinhança usando a ordem correta (row, col)
                        this->get_perl(neighbor_y, neighbor_x, neighbor_perl);

                        // Insere a intensidade da vizinhança no canal correspondente
                        for(uint8_t c = 0; c < number_channels; c++)
                            medians[c].push_back(neighbor_perl[c]);
                        
                    }
                }
            }

            // Calcula a mediana para cada canal
            for(uint8_t c = 0; c < number_channels; c++)
            {
                if(!medians[c].empty())
                {
                    // Ordena os valores de intensidade
                    std::sort(medians[c].begin(), medians[c].end());
                    
                    // Pega o elemento do meio (mediana)
                    int median_idx = medians[c].size() / 2;
                    median_p[c] = medians[c][median_idx];  
                }
                else
                    median_p[c] = 0;
                
            }

            copy.set_perl(i, j, median_p);
        }

        // Libera os buffers temporários da thread atual
        delete[] neighbor_perl;
        delete[] median_p;
    }

    return copy;
}