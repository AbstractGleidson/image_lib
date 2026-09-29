import matplotlib.pyplot as plt

if __name__ == "__main__":
    frequency = []
    
    # Leitura do arquivo gerado
    with open("hist.txt", 'r', encoding='utf-8') as file:
        for line in file:
            parts = line.split(' ')
            if len(parts) > 3:
                frequency.append(int(parts[4]))
    
    intensities = list(range(len(frequency)))
    
    plt.figure(figsize=(10, 5))
    plt.bar(intensities, frequency, color='skyblue', edgecolor='black')
    plt.title('Histograma de Frequência por Intensidade')
    plt.xlabel('Intensidade')
    plt.ylabel('Frequência')
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    
    plt.show()