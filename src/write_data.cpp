#include <fstream>
#include <vector>

void write_hist(std::vector<int> frequency)
{
    std::ofstream file("hist.txt"); // cria o arquivo

    if(file.is_open())
    {
        for(int i = 0; i < 256; i++)
        {
            file << "Intensidade: " << i << "  Frequencia: " << frequency[i] << std::endl;
        }
        
        file.close();
    }
}