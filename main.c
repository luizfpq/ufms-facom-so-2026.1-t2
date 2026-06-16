/*
 * main.c — Simulador de alocacao de memoria (particoes variaveis)
 *
 * UFMS — FACOM — SO 2026.1
 * Luiz Quirino <luiz.quirino@ufms.br>
 *
 * Uso: ./simulador <arquivo.txt>
 */

#include <stdio.h>
#include <string.h>
#include "memoria.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo_entrada.txt>\n", argv[0]);
        return 1;
    }

    Memoria mem;
    Evento eventos[MAX_EVENTOS];
    int n_eventos = 0;
    char alg[20];

    carregar_entrada(argv[1], &mem, eventos, &n_eventos, alg);

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
        int ok = 1;

        if (ev->tipo == 0) {
            /* Tenta alocar */
            ok = fn_alocar(&mem, ev->pid, ev->tamanho);
            if (!ok) {
                if (tem_fragmentacao_externa(&mem, ev->tamanho))
                    mem.frag_externas++;
                mem.falhas++;
            }
        } else {
            /* Libera e junta brechas adjacentes */
            mem_liberar(&mem, ev->pid);
            mem_coalescer(&mem);
        }

        mostrar_evento(i + 1, ev, ok);
        mostrar_memoria(&mem, i + 1);
    }

    mostrar_relatorio(&mem, n_eventos);
    return 0;
}
