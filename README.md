# Simulador de Alocação Dinâmica de Memória

Trabalho prático 2 da disciplina de Sistemas Operacionais — UFMS/FACOM 2026.1

**Autor:** Luiz Quirino — luiz.quirino@ufms.br

---

## Introdução

Simulador de alocação dinâmica de memória contígua com partições variáveis. Lê um arquivo de entrada com o tamanho da memória e uma sequência de eventos (alocações e liberações), executa a simulação passo a passo e exibe o estado da memória após cada operação.

Implementa três estratégias: First Fit, Best Fit e Worst Fit, conforme Silberschatz, Galvin e Gagne (2018), capítulo 9.

---

## Estrutura do Projeto

```
├── memoria.h           # Structs e protótipos (header único)
├── main.c              # Ponto de entrada e dispatch por function pointer
├── arquivo.c           # Parser do arquivo de entrada
├── memoria_ops.c       # Algoritmos de alocação e liberação
├── visualizacao.c      # Interface textual e relatório final
├── Makefile            # Compilação e atalhos de execução
├── entrada_first_fit.txt   # Cenário de teste
├── entrada_comparativo.txt # Cenário para análise comparativa
└── relatorio.tex       # Relatório LaTeX
```

---

## Compilação e Execução

### Pré-requisitos

* GCC
* GNU Make

### Compilar

```bash
make
```

### Executar

```bash
./simulador <arquivo_de_entrada.txt>
./simulador <arquivo_de_entrada.txt> -a ALGORITMO
```

A flag `-a` sobrescreve o algoritmo definido no cabeçalho do arquivo, sem precisar editá-lo.

### Atalhos

```bash
make first          # First Fit
make best           # Best Fit
make worst          # Worst Fit
make robustez       # Cenário de casos de borda
make comparativo    # Três algoritmos na mesma entrada
make help           # Exibe as opções disponíveis
```

---

## Formato do Arquivo de Entrada

```
# Algoritmo: FIRST_FIT
# Memoria: 1000
#
# evento   id   tamanho
ALOCAR  P1  200
ALOCAR  P2  300
LIBERAR P1
ALOCAR  P3  250
```

Algoritmos aceitos: `FIRST_FIT`, `BEST_FIT`, `WORST_FIT`.

---

## Saída

Após cada evento, o simulador exibe:

1. Resultado da operação, com um de cinco desfechos:
   - `OK`
   - `FALHOU (fragmentacao externa)` — há espaço total, mas não contíguo
   - `FALHOU (memoria livre insuficiente)` — não há espaço total
   - `REJEITADO (tamanho invalido)` — tamanho ≤ 0
   - `REJEITADO (processo ja alocado)` — ID duplicado
2. Tabela com blocos ocupados e livres (endereço, tamanho)
3. Mapa visual compacto da memória

Ao final, imprime relatório com: utilização, brechas, alocações que falharam e fragmentações externas detectadas.

---

## Referências

* SILBERSCHATZ, A.; GALVIN, P. B.; GAGNE, G. **Operating System Concepts**. 10. ed. Wiley, 2018.
