import math
import random
import time
import statistics
import csv
import matplotlib.pyplot as plt


# comparação de tempo entre prim e kruskal
# Problemas:
# 2522 - Rede do DINF
# 1552 - Resgate em Queda Livre


def gerar_pontos(quantidade, limite_coordenada=10000):
    pontos = []

    for _ in range(quantidade):
        x = random.randint(0, limite_coordenada)
        y = random.randint(0, limite_coordenada)
        pontos.append((x, y))

    return pontos


def calcular_distancia(ponto_a, ponto_b, fator_unidade):
    diferenca_x = ponto_a[0] - ponto_b[0]
    diferenca_y = ponto_a[1] - ponto_b[1]

    return math.hypot(diferenca_x, diferenca_y) * fator_unidade

def prim(pontos, fator_unidade):
    quantidade = len(pontos)

    visitado = [False] * quantidade
    menor_distancia = [float("inf")] * quantidade

    menor_distancia[0] = 0.0
    custo_total = 0.0

    for _ in range(quantidade):
        vertice_atual = -1

        for i in range(quantidade):
            if not visitado[i]:
                if vertice_atual == -1 or menor_distancia[i] < menor_distancia[vertice_atual]:
                    vertice_atual = i

        visitado[vertice_atual] = True
        custo_total += menor_distancia[vertice_atual]

        for vizinho in range(quantidade):
            if not visitado[vizinho]:
                distancia = calcular_distancia(
                    pontos[vertice_atual],
                    pontos[vizinho],
                    fator_unidade
                )

                if distancia < menor_distancia[vizinho]:
                    menor_distancia[vizinho] = distancia

    return custo_total


def criar_conjuntos(quantidade):
    pai = list(range(quantidade))
    altura = [0] * quantidade

    return pai, altura


def encontrar_representante(pai, vertice):
    if pai[vertice] != vertice:
        pai[vertice] = encontrar_representante(pai, pai[vertice])

    return pai[vertice]


def unir_conjuntos(pai, altura, vertice_a, vertice_b):
    representante_a = encontrar_representante(pai, vertice_a)
    representante_b = encontrar_representante(pai, vertice_b)

    if representante_a == representante_b:
        return False

    if altura[representante_a] < altura[representante_b]:
        pai[representante_a] = representante_b
    elif altura[representante_a] > altura[representante_b]:
        pai[representante_b] = representante_a
    else:
        pai[representante_b] = representante_a
        altura[representante_a] += 1

    return True


def gerar_arestas(pontos, fator_unidade):
    quantidade = len(pontos)
    arestas = []

    for i in range(quantidade):
        for j in range(i + 1, quantidade):
            peso = calcular_distancia(pontos[i], pontos[j], fator_unidade)
            arestas.append((peso, i, j))

    return arestas


def kruskal(pontos, fator_unidade):
    quantidade = len(pontos)

    arestas = gerar_arestas(pontos, fator_unidade)
    arestas.sort()

    pai, altura = criar_conjuntos(quantidade)

    custo_total = 0.0
    arestas_usadas = 0

    for peso, origem, destino in arestas:
        if unir_conjuntos(pai, altura, origem, destino):
            custo_total += peso
            arestas_usadas += 1

            if arestas_usadas == quantidade - 1:
                break

    return custo_total


def medir_tempo(funcao_algoritmo, pontos, fator_unidade, repeticoes=5):
    tempos = []

    funcao_algoritmo(pontos, fator_unidade)

    for _ in range(repeticoes):
        inicio = time.perf_counter()
        funcao_algoritmo(pontos, fator_unidade)
        fim = time.perf_counter()

        tempos.append(fim - inicio)

    return statistics.median(tempos)


def executar_benchmark(nome_problema, fator_unidade, quantidades):
    resultados = []

    for quantidade in quantidades:
        pontos = gerar_pontos(quantidade)

        tempo_prim = medir_tempo(prim, pontos, fator_unidade)
        tempo_kruskal = medir_tempo(kruskal, pontos, fator_unidade)

        custo_prim = prim(pontos, fator_unidade)
        custo_kruskal = kruskal(pontos, fator_unidade)

        resultados.append({
            "problema": nome_problema,
            "quantidade_pontos": quantidade,
            "tempo_prim": tempo_prim,
            "tempo_kruskal": tempo_kruskal,
            "custo_prim": custo_prim,
            "custo_kruskal": custo_kruskal
        })

        print(f"{nome_problema} | {quantidade} pontos")
        print(f"  Prim    -> tempo: {tempo_prim:.6f}s | custo: {custo_prim:.2f}")
        print(f"  Kruskal -> tempo: {tempo_kruskal:.6f}s | custo: {custo_kruskal:.2f}")
        print()

    return resultados

def gerar_grafico_tempo(resultados, nome_problema, nome_arquivo):
    quantidades = [linha["quantidade_pontos"] for linha in resultados]
    tempos_prim = [linha["tempo_prim"] for linha in resultados]
    tempos_kruskal = [linha["tempo_kruskal"] for linha in resultados]

    plt.figure()

    plt.plot(quantidades, tempos_prim, marker="o", label="Prim")
    plt.plot(quantidades, tempos_kruskal, marker="o", label="Kruskal")

    plt.title(f"Comparação de tempo - {nome_problema}")
    plt.xlabel("Quantidade de pontos")
    plt.ylabel("Tempo de execução (segundos)")

    plt.legend()
    plt.grid(True)

    plt.savefig(nome_arquivo, dpi=300, bbox_inches="tight")
    plt.close()


def salvar_resultados_csv(nome_arquivo, resultados):
    campos = [
        "problema",
        "quantidade_pontos",
        "tempo_prim",
        "tempo_kruskal",
        "custo_prim",
        "custo_kruskal"
    ]

    with open(nome_arquivo, "w", newline="", encoding="utf-8") as arquivo:
        escritor = csv.DictWriter(arquivo, fieldnames=campos)
        escritor.writeheader()
        escritor.writerows(resultados)



def main():
    random.seed(42)

    quantidades = [50, 100, 150, 200, 250, 300]

    resultados_2522 = executar_benchmark(
        nome_problema="2522 - Rede do DINF",
        fator_unidade=1.0,
        quantidades=quantidades
    )

    resultados_1552 = executar_benchmark(
        nome_problema="1552 - Resgate em Queda Livre",
        fator_unidade=0.01,
        quantidades=quantidades
    )

    todos_resultados = resultados_2522 + resultados_1552

    salvar_resultados_csv(
        nome_arquivo="resultados_tempo_prim_kruskal.csv",
        resultados=todos_resultados
    )

    gerar_grafico_tempo(
        resultados=resultados_2522,
        nome_problema="2522 - Rede do DINF",
        nome_arquivo="grafico_tempo_2522.png"
    )

    gerar_grafico_tempo(
        resultados=resultados_1552,
        nome_problema="1552 - Resgate em Queda Livre",
        nome_arquivo="grafico_tempo_1552.png"
    )

    print("Arquivos gerados:")
    print("- resultados_tempo_prim_kruskal.csv")
    print("- grafico_tempo_2522.png")
    print("- grafico_tempo_1552.png")

main()