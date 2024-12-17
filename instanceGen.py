import csv
import random

def gerador_instancia_sudoku():
    # Perguntas iniciais ao usuário
    csv_path = input("Digite o caminho do arquivo CSV com os jogos de Sudoku: ").strip()
    instancia_path = input("Digite onde a nova instância deve ser armazenada: ").strip()
    instancia_nome = input("Digite o nome do arquivo da nova instância (com extensão .txt): ").strip()
    num_jogos = int(input("Quantos jogos devem ser colocados na nova instância? "))
    max_repeticoes = int(input("Qual a quantidade máxima de repetições de um determinado jogo? "))

    # Lendo o CSV e descartando a primeira linha
    jogos = []
    try:
        with open(csv_path, "r") as csvfile:
            reader = csv.reader(csvfile)
            next(reader)  # Descartar a primeira linha
            for linha in reader:
                jogo, _ = linha  # Ignorar a solução
                jogos.append(jogo)
    except FileNotFoundError:
        print("Arquivo CSV não encontrado. Verifique o caminho e tente novamente.")
        return
    except Exception as e:
        print(f"Ocorreu um erro ao processar o arquivo CSV: {e}")
        return

    # Selecionando e repetindo jogos
    instancias = []
    while len(instancias) < num_jogos:
        jogo = random.choice(jogos)
        repeticoes = random.randint(1, min(max_repeticoes, num_jogos - len(instancias)))
        instancias.extend([jogo] * repeticoes)

    # Embaralhar os jogos
    random.shuffle(instancias)

    # Salvando a nova instância no arquivo TXT
    try:
        with open(f"{instancia_path}/{instancia_nome}", "w") as file:
            file.write(f"{num_jogos}\n")  # Quantidade de jogos na instância
            for jogo in instancias:
                file.write(f"{jogo}\n")
        print(f"Instância gerada e salva em {instancia_path}/{instancia_nome}")
    except Exception as e:
        print(f"Ocorreu um erro ao salvar a instância: {e}")

# Executar o gerador
gerador_instancia_sudoku()
