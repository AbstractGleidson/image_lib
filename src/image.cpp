#include <image.hpp>
#include <cmath>
#include <fstream>
#include <iostream>

// construtor padrão
Image::Image()
{
    this->height = 0; 
    this->width = 0; 
    this->channels = nullptr; // ponteiro para os canais
    this->color_space = RGB; 
}

// construtor personalizado
Image::Image(int height, int width, uint8_t number_channels, uint8_t *perl, ColorSpace color_space)
{
    this->height = height; 
    this->width = width; 
    this->color_space = color_space; 
    this->channels = new uint8_t*[number_channels];

    bool alloc_local = false;

    uint8_t *default_perl = perl;
    if(perl == nullptr)
        default_perl = new uint8_t[number_channels]{0}; // oloca um array tamanho channel inicializado com 0
    

    int elements = this->height * this->width;

    for(int i = 0; i < number_channels; i++)
    {
        this->channels[i] = new uint8_t[elements];
        memset(this->channels[i], default_perl[i], elements * sizeof(uint8_t));
    }

    if(perl == nullptr)
        delete[] default_perl;
}

// construtor de cópia, metodo copy
Image::Image(const Image& other)
{
    // cópia valores numericos
    this->height = other.height;
    this->width  = other.width;
    this->color_space = other.color_space;
    int number_channels = this->get_number_channels();
    this->channels = new uint8_t*[number_channels]; // cria um ponteiro para cada canal

    if(other.channels != nullptr)
    {
        int elements = this->height * this->width; // quantidade de elementos em cada canal

        for(int i = 0; i < number_channels; i++)
        {
            this->channels[i] = new uint8_t[elements]; // aloca memoria para cada canal
            memcpy(this->channels[i], other.channels[i], elements * sizeof(uint8_t)); // cópia o bloco de memória 
        }
    }
    else{
        this->channels  = nullptr;
    }
}

// destrutor 
Image::~Image()
{
    if(channels != nullptr)
    {
        for(int i = 0; i < this->get_number_channels(); i++)
            delete[] channels[i]; // libera memória da heap para cada canal
        delete[] channels; // libera os ponteiros duplos
    }
}


// operador de atribuição
Image& Image::operator=(const Image& other) {
    if (this == &other) return *this; // evita auto atribuição

    // libera a memória atual 
    if (this->channels != nullptr)
    {
        int current_channels = this->get_number_channels();
        for(int i = 0; i < current_channels; i++)
            delete[] channels[i]; 
        delete[] channels; 
        this->channels = nullptr;
    }

    // copia os valores numéricos da nova imagem
    this->height = other.height;
    this->width  = other.width;
    this->color_space = other.color_space;
    
    int number_channels = this->get_number_channels(); 

    // aloca e copia os dados se a outra imagem possuir canais válidos
    if(other.channels != nullptr)
    {
        this->channels = new uint8_t*[number_channels]; 
        int elements = this->height * this->width; 

        for(int i = 0; i < number_channels; i++)
        {
            this->channels[i] = new uint8_t[elements]; 
            memcpy(this->channels[i], other.channels[i], elements * sizeof(uint8_t)); 
        }
    }
    else {
        this->channels = nullptr;
    }

    return *this; 
}

Image Image::get_channel(uint8_t channel) {
    Image copy = Image(this->height, this->width, 1, nullptr, GRAY); 
    
    memcpy(copy.channels[0], this->channels[channel], (this->height * this->width) * sizeof(uint8_t));
    return copy;
}

Image Image::negative()
{
    Image copy(*this); // cria cópia 
    return ~copy;
}

Image Image::mean_gray_scale()
{
    Image copy = Image(this->height, this->width, 1, nullptr, GRAY);

    int number_channels = this->get_number_channels();
    int height = this->height;
    int width = this->width;
            
    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < height; i++)
    {
        // Alocado dentro do loop para ser thread-safe no OpenMP sem data races
        uint8_t *perl = new uint8_t[number_channels];

        for(int j = 0; j < width; j++)
        {
            this->get_perl(i, j, perl);

            int sum = 0; // Usar int para evitar overflow antes da divisão
            for(int c = 0; c < number_channels; c++)
            {
                sum += perl[c]; 
            }

            uint8_t mean = static_cast<uint8_t>(sum / number_channels);

            copy.set_perl(i, j, &mean);
        }

        delete[] perl; // Libera o buffer da thread atual
    }

    return copy;
}

Image Image::RGB_TO_GRAY()
{
    Image copy = Image(this->height, this->width, 1, nullptr, GRAY);

    int height = this->height;
    int width = this->width;
    int number_channels = this->get_number_channels();

    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < height; i++)
    {
        uint8_t *perl = new uint8_t[number_channels];

        for(int j = 0; j < width; j++)
        {
            this->get_perl(i, j, perl);

            // Pesos de acordo com a sensibilidade do olho humano 
            double weight_red = 0.2126, weight_green = 0.7152, weight_blue = 0.0722; 

            double gray_val = (perl[0] * weight_red) + (perl[1] * weight_green) + (perl[2] * weight_blue);
            
            uint8_t gray = (uint8_t) (std::min(std::max((int) (gray_val), 0), 255));

            copy.set_perl(i, j, &gray);
        }

        delete[] perl;
    }

    return copy;
}

Image Image::binary(const uint8_t thres)
{
    Image copy(*this);

    int height = this->height;
    int width = this->width;
    int number_channels = this->get_number_channels();

    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < height; i++)
    {
        uint8_t *perl = new uint8_t[number_channels];

        for(int j = 0; j < width; j++)
        {
            this->get_perl(i, j, perl);

            for(int c = 0; c < number_channels; c++)
            {
                perl[c] = (perl[c] > thres) ? 255 : 0;
            }

            copy.set_perl(i, j, perl);
        }

        delete[] perl; 
    }

    return copy;
}

Image Image::mean_blur(int size_kernel) {
    Image copy(*this);

    // Dimensões da imagem
    int height = this->get_height();
    int width = this->get_width();
    uint8_t number_channels = this->get_number_channels();

    #pragma omp parallel for schedule(dynamic)
    for(int i = 0; i < height; i++)
    {
        uint8_t *neighbor_perl = new uint8_t[number_channels];
        uint8_t *mean_p = new uint8_t[number_channels];
        
        double *means = new double[number_channels];

        for(int j = 0; j < width; j++)
        {
            // Reseta o acumulador e contadores para o pixel atual
            for(int c = 0; c < number_channels; c++) {
                means[c] = 0.0;
            }
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
                        for(int c = 0; c < number_channels; c++)
                        {
                            means[c] += neighbor_perl[c];
                        }

                        valid_elements++;
                    }
                }
            }

            // Calcula a média final e converte para uint8_t 
            if(valid_elements > 0)
            {
                for(int c = 0; c < number_channels; c++)
                {
                    mean_p[c] = static_cast<uint8_t>(means[c] / valid_elements);
                }
            }

            // Substitui os valores na imagem 
            copy.set_perl(i, j, mean_p);
        }

        delete[] neighbor_perl;
        delete[] mean_p;
        delete[] means;
    }

    return copy;
}

Image Image::median_blur(int size_kernel) {
    Image copy(*this);

    int height = this->get_height();
    int width = this->get_width();
    uint8_t number_channels = this->get_number_channels();

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
                        for(int c = 0; c < number_channels; c++)
                        {
                            medians[c].push_back(neighbor_perl[c]);
                        }
                    }
                }
            }

            // Calcula a mediana para cada canal
            for(int c = 0; c < number_channels; c++)
            {
                if(!medians[c].empty())
                {
                    // Ordena os valores de intensidade
                    std::sort(medians[c].begin(), medians[c].end());
                    
                    // Pega o elemento do meio (mediana)
                    int median_idx = medians[c].size() / 2;
                    median_p[c] = medians[c][median_idx];  
                }
                else {
                    median_p[c] = 0;
                }
            }

            copy.set_perl(i, j, median_p);
        }

        // Libera os buffers temporários da thread atual
        delete[] neighbor_perl;
        delete[] median_p;
    }

    return copy;
}

std::vector<int> Image::hist(const uint8_t channel)
{
    std::vector<int> frequency(256, 0);
    uint8_t *perl = new uint8_t[this->get_number_channels()];

    for(int i = 0; i < this->height; i++)
    {
        for(int j = 0; j < this->width; j++)
        {
            this->get_perl(i, j, perl);
            frequency[perl[channel]]++;
        }   
    }

    delete [] perl;
    return frequency;
}

std::vector<double> Image::hist(const uint8_t channel, const bool is_norm)
{
    std::vector<double> frequency(256, 0);
    uint8_t *perl = new uint8_t[this->get_number_channels()];

    for(int i = 0; i < this->height; i++)
    {
        for(int j = 0; j < this->width; j++)
        {
            this->get_perl(i, j, perl);
            frequency[perl[channel]]++;
        }   
    }

    delete [] perl;

    if(is_norm)
        for(int i = 0; i < 256; i++)
            frequency[i] = frequency[i] /  (this->height * this->width);

    return frequency;
}

Image Image::equalize(const uint8_t channel)
{
    Image copy = this->get_channel(channel);
    std::vector<int> frequency = copy.hist(0);
    std::vector<double> accumulated(256, 0);
    std::vector<int> transform_function(256, 0);

    int height = copy.get_height();
    int width = copy.get_width();

    int full_perls = height * width;

    for(int i = 0; i < 256; i++){
        if(i > 0)
            accumulated[i] = (accumulated[i - 1]) + (frequency[i] / (double) full_perls);
        else
            accumulated[i] = (frequency[i] / (double) full_perls);

        transform_function[i] = std::round(accumulated[i] * 255);  
    }

    uint8_t *perl = new uint8_t[copy.get_number_channels()];

    for(int i = 0; i < copy.get_height(); i++)
    {
        for(int j = 0; j < copy.get_width(); j++)
        {
            copy.get_perl(i, j, perl);
            *perl = transform_function[*perl]; // aplica a função de transformação
            copy.set_perl(i, j, perl);
        }
    }

    delete [] perl;

    return copy;
}

Image Image::equalize_esp(const uint8_t channel, std::vector<double> hist_esp)
{
    Image copy = this->get_channel(channel);

    std::vector<int> hist_origem  = copy.hist(0);
    std::vector<double> accumulated_origem(256, 0);
    std::vector<double> accumulated_esp(256, 0);
    std::vector<double> transform_origem(256, 0);
    std::vector<double> transform_esp(256, 0);
    std::vector<double> transform_origem_to_esp(256, 0);

    int height = copy.get_height();
    int width = copy.get_width();

    int full_perls = height * width;

    for(int i = 0; i < 256; i++){
        if(i > 0){
            accumulated_origem[i] = (accumulated_origem[i - 1]) + (hist_origem[i] / (double) full_perls);
            accumulated_esp[i] = (accumulated_esp[i - 1]) + hist_esp[i];
        }    
        else{
            accumulated_origem[i] = (hist_origem[i] / (double) full_perls);
            accumulated_esp[i] = hist_esp[i];

        }
        transform_origem[i] = accumulated_origem[i] * 255; // Função de transformação da original
        transform_esp[i] = accumulated_esp[i] * 255; // Função de transformação da especificada
    }

    // Faz o emparelhamento entre as funções de transformaçõeos
    for(int i = 0; i < 256; i++)
    {
        int dist = std::abs(transform_origem[i] - transform_esp[0]);
        int index_min = 0;

        for(int j = 0; j < 256; j++)
        {
            if(dist > std::abs(transform_origem[i] - transform_esp[j])){
                index_min = j;
                dist = std::abs(transform_origem[i] - transform_esp[j]);
            }
        }

        transform_origem_to_esp[i] = index_min;
    }

    uint8_t *perl = new uint8_t[copy.get_number_channels()];

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            copy.get_perl(i, j, perl);
            *perl = transform_origem_to_esp[*perl]; // aplica a função de transformação
            copy.set_perl(i, j, perl);
        }
    }

    delete [] perl;

    return copy;
}

void Image::write_hist(const char path[])
{
    std::ofstream file(path); // cria o arquivo

    std::vector<int> frequency = this->hist(0); // gera o histograma

    if(file.is_open())
    {
        for(int i = 0; i < 256; i++)
        {
            file << "Intensidade: " << i << "  Frequencia: " << frequency[i] << std::endl;
        }
        file.close();
    }
}

void Image::show_hist()
{

    this->write_hist("hist.txt"); // escreve o histograma em arquivo temp
    int resultado = std::system("python ../src/aux_python/hist.py");
    resultado = std::system("rm hist.txt");
}