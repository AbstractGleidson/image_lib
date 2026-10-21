import argparse
import matplotlib.pyplot as plt

def ler_histograma(filename):
    frequency = []
    with open(filename, 'r', encoding='utf-8') as file:
        for line in file:
            parts = line.split(' ')
            if len(parts) > 3:
                frequency.append(float(parts[4]))
    return frequency

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Gera histograma de frequência.")
    parser.add_argument('--inputs', nargs='+', required=True, help='Caminho para um ou mais arquivos de histograma')
    parser.add_argument('--labels', nargs='+', default=[], help='Legendas para cada histograma')
    parser.add_argument('--title', type=str, default='Histograma de Frequência', help='Título do gráfico')
    parser.add_argument('--output', type=str, default=None, help='Caminho completo para salvar a imagem (se omitido, exibe na tela)')
    parser.add_argument("--normalized", action="store_true", help='Indica se os dados estão normalizados entre 0 e 1')
    
    args = parser.parse_args()

    plt.figure(figsize=(10, 5))

    # Plota cada histograma fornecido
    for i, file_path in enumerate(args.inputs):
        frequency = ler_histograma(file_path)
        intensities = list(range(len(frequency)))
        
        # Define um rótulo padrão caso não seja fornecido
        label = args.labels[i] if i < len(args.labels) else f'Hist {i+1}'
        
        plt.bar(intensities, frequency, alpha=0.6, label=label, edgecolor='black')

    plt.title(args.title)
    plt.xlabel('Intensidade')
    plt.xlim(0,255)
    
    if args.normalized:
        plt.ylabel('Frequência Relativa (Probabilidade)')
    else:
        plt.ylabel('Frequência (Contagem de Pixels)')
    
    # Se houver mais de uma série ou labels explicitos, exibe a legenda
    if len(args.inputs) > 1 or args.labels:
        plt.legend()
        
    plt.grid(axis='y', linestyle='--', alpha=0.7)

    # Salva ou exibe o gráfico
    if args.output:
        plt.savefig(args.output, bbox_inches='tight')
        print(f"Gráfico salvo em: {args.output}")
    else:
        plt.show()