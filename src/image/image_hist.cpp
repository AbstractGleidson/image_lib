#include <image.hpp>

// Histograma de quantidade de perls
std::vector<int> Image::hist(const uint8_t channel)
{
    std::vector<int> frequency(256, 0);
    uint8_t *perl = new uint8_t[get_number_channels(this->color_space)];

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

// Histograma de frequencia 
std::vector<double> Image::hist(const uint8_t channel, const bool is_norm)
{
    std::vector<double> frequency(256, 0);
    std::vector<int> perls = this->hist(channel); // calcula a quantidade de perl por intensida    

    if(is_norm)
        for(uint16_t i = 0; i < 256; i++)
            frequency[i] = perls[i] /  (double) (this->height * this->width);

    return frequency;
}

// Retorna o canal da imagem equalizado
Image Image::equalize(const uint8_t channel)
{
    Image copy = this->get_channel(channel); // Pega o canal selecionado

    std::vector<int> frequency = copy.hist(0); // histograma da imagem
    double accumulated[256] = {0}; // vetor para a acumulada
    uint8_t transform_function[256] = {0}; // vetor para a função de mapeamento

    uint16_t height = copy.get_height();
    uint16_t width = copy.get_width();

    int full_perls = height * width;

    // calcula a acumulada e mapeamento inicial
    accumulated[0] = (frequency[0] / (double) full_perls); 
    transform_function[0] = std::round(accumulated[0] * 255);

    for(uint16_t i = 1; i < 256; i++){
        accumulated[i] = (accumulated[i - 1]) + (frequency[i] / (double) full_perls);
        transform_function[i] = std::round(accumulated[i] * 255);  
    }

    uint8_t *perl = new uint8_t[get_number_channels(copy.get_color_space())];

    // Aplica a função de transformação
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            copy.get_perl(i, j, perl);
            *perl = transform_function[*perl]; // aplica a função de transformação
            copy.set_perl(i, j, perl);
        }
    }

    delete [] perl;

    return copy;
}

// Especifica o canal com a FDP passada como parâmetro
Image Image::equalize_esp(const uint8_t channel, std::vector<double> hist_esp)
{
    Image copy = this->get_channel(channel);

    std::vector<int> hist_origem  = copy.hist(0);
    double *accumulated_origem = new double[256]{0};
    double *accumulated_esp = new double[256]{0};
    double *transform_origem = new double[256]{0};
    double *transform_esp = new double[256]{0};
    uint8_t *transform_origem_to_esp = new uint8_t[256]{};

    uint16_t height = copy.get_height();
    uint16_t width = copy.get_width();

    int full_perls = height * width;

    for(uint16_t i = 0; i < 256; i++){
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

    // libera a memoria das acumuladas
    delete[] accumulated_origem;
    delete[] accumulated_esp;

    // Faz o emparelhamento entre as funções de transformaçõeos
    for(uint16_t i = 0; i < 256; i++)
    {
        uint8_t dist = std::abs(transform_origem[i] - transform_esp[0]);
        uint8_t index_min = 0;

        for(uint16_t j = 0; j < 256; j++)
        {
            if(dist > std::abs(transform_origem[i] - transform_esp[j])){
                index_min = j;
                dist = std::abs(transform_origem[i] - transform_esp[j]);
            }
        }

        transform_origem_to_esp[i] = index_min;
    }

    // libera memoria das funcoes de transformacao
    delete [] transform_origem;
    delete [] transform_esp;

    uint8_t *perl = new uint8_t[get_number_channels(copy.get_color_space())];

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
    delete [] transform_origem_to_esp;

    return copy;
}

// Escreve o histograma da imagem em um arquivo .txt
void Image::write_hist(const char path[])
{
    std::ofstream file(path); // cria o arquivo

    std::vector<int> frequency = this->hist(0); // gera o histograma

    if(file.is_open())
    {
        for(uint16_t i = 0; i < 256; i++)
        {
            file << "Intensidade: " << i << "  Frequencia: " << frequency[i] << std::endl;
        }
        file.close();
    }
}

// Escreve o histograma normalizado da imagem em um arquivo .txt
void Image::write_hist(const char path[], const bool is_norm)
{
    std::ofstream file(path); // cria o arquivo

    std::vector<double> frequency = this->hist(0, is_norm); // gera o histograma

    if(file.is_open())
    {
        for(uint16_t i = 0; i < 256; i++)
        {
            file << "Intensidade: " << i << "  Frequencia: " << frequency[i] << std::endl;
        }
        file.close();
    }
}

// Exibe o histograma da imagem
void Image::show_hist(const std::string& title, const std::string& save_path, const bool is_norm)
{

    // Escreve o histograma 
    if(is_norm)
        this->write_hist("hist.txt", true); // Normalizado [1,0]
    else
        this->write_hist("hist.txt"); // Não normalizado, contagem de intensidades
    
    // Monta o comando base
    std::string cmd = "python ../src/aux_python/hist.py --inputs hist.txt --title \"" + title + "\"" + (is_norm?" --normalized":"");
    
    // Se um caminho de salvamento foi fornecido, adiciona o argumento --output
    if (!save_path.empty()) {
        cmd += " --output \"" + save_path + "\"";
    }
    
    // Executa o comando
    int resultado = std::system(cmd.c_str());
    resultado = std::system("rm hist.txt"); // Limpeza do arquivo temp
}

// Exibe o histograma da imagem junto do histograma de outra imagem passada como parâmetro
void Image::show_mult_hist(Image& other, const std::string& title, std::vector<std::string>& labels, const std::string& save_path, const bool is_norm)
{

    // Escreve os histogramas 
    if(is_norm)
    {   // Histrogramas normalizados 
        this->write_hist("hist1.txt", true);
        other.write_hist("hist2.txt", true);
    }
    else{// Histogramas de contagem
        this->write_hist("hist1.txt");
        other.write_hist("hist2.txt");
    }
    
    // Monta o comando base
    std::string cmd = "python ../src/aux_python/hist.py --inputs hist1.txt hist2.txt --title \"" + title + "\"" + " --labels " + labels[0] + " " + labels[1] + (is_norm?" --normalized":"");
    
    // Se um caminho de salvamento foi fornecido, adiciona o argumento --output
    if (!save_path.empty()) {
        cmd += " --output \"" + save_path + "\"";
    }
    
    // Executa o comando
    int resultado = std::system(cmd.c_str());
    resultado = std::system("rm hist1.txt"); // Limpeza do arquivo temp
    resultado = std::system("rm hist2.txt"); // Limpeza do arquivo temp
}