/*
 * main.c — Simulador de alocacao de memoria (particoes variaveis)
 *
 * UFMS — FACOM — SO 2026.1
 * Luiz Quirino <luiz.quirino@ufms.br>
 *
 * Uso: ./simulador <arquivo.txt> [-a ALGORITMO]
 *      Algoritmos: FIRST_FIT, BEST_FIT, WORST_FIT
 */

#include <stdio.h>
#include <string.h>
#include "memoria.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_entrada.txt> [-a ALGORITMO]\n", argv[0]);
        fprintf(stderr, "Algoritmos: FIRST_FIT, BEST_FIT, WORST_FIT\n");
        return 1;
    }

    Memoria mem;
    Evento eventos[MAX_EVENTOS];
    int n_eventos = 0;
    char alg[20];

    carregar_entrada(argv[1], &mem, eventos, &n_eventos, alg);

    /* Flag -a sobrescreve o algoritmo do arquivo */
    for (int i = 2; i < argc - 1; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            strncpy(alg, argv[i + 1], 19);
            alg[19] = '\0';
            break;
        }
    }

    /* Seleciona a funcao de alocacao pelo nome do algoritmo */
    int (*fn_alocar)(Memoria *, int, int) = NULL;

    if (strcmp(alg, "FIRST_FIT") == 0)
        fn_alocar = first_fit;
    else if (strcmp(alg, "BEST_FIT") == 0)
        fn_alocar = best_fit;
    else if (strcmp(alg, "WORST_FIT") == 0)
        fn_alocar = worst_fit;
    else {
        fprintf(stderr, "Algoritmo '%s' invalido.\n", alg);
        fprintf(stderr, "Use: FIRST_FIT, BEST_FIT ou WORST_FIT\n");
        return 1;
    }

    printf("\n=== Simulador de Alocacao de Memoria ===\n");
    printf("Algoritmo: %s\n", alg);
    printf("Memoria total: %d unidades\n\n", mem.tam_total);

    mostrar_memoria(&mem, 0);

    /* Loop principal: processa cada evento */
    for (int i = 0; i < n_eventos; i++) {
        Evento *ev = &eventos[i];
        int status = ST_OK;

        if (ev->tipo == 0) {
            /* Validacoes de entrada antes de tentar alocar */
            if (ev->tamanho <= 0) {
                status = ST_TAM_INVALIDO;
            } else if (mem_pid_existe(&mem, ev->pid)) {
                status = ST_PID_DUPLICADO;
            } else if (fn_alocar(&mem, ev->pid, ev->tamanho)) {
                status = ST_OK;
            } else {
                /* Falhou por espaco: distingue a causa */
                if (tem_fragmentacao_externa(&mem, ev->tamanho)) {
                    status = ST_FRAG_EXTERNA;
                    mem.frag_externas++;
                } else {
                    status = ST_SEM_MEMORIA;
                }
                mem.falhas++;
            }
        } else {
            mem_liberar(&mem, ev->pid);
            mem_coalescer(&mem);
        }

        mostrar_evento(i + 1, ev, status);
        mostrar_memoria(&mem, i + 1);
    }

    mostrar_relatorio(&mem, n_eventos);
    return 0;
}
