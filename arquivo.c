/*
 * arquivo.c — Leitura do arquivo de entrada
 *
 * UFMS — FACOM — SO 2026.1
 * Luiz Quirino <luiz.quirino@ufms.br>
 *
 * Formato esperado:
 *   # Algoritmo: FIRST_FIT
 *   # Memoria: 1000
 *   ALOCAR  P1  200
 *   LIBERAR P2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoria.h"

void carregar_entrada(const char *caminho, Memoria *mem,
                      Evento eventos[], int *n_eventos,
                      char *algoritmo)
{
    FILE *arq = fopen(caminho, "r");
    if (!arq) {
        fprintf(stderr, "Erro ao abrir '%s'\n", caminho);
        exit(1);
    }

    char buf[256];
    int tam_mem = 1024;  /* padrao se nao especificado */
    *n_eventos = 0;
    strcpy(algoritmo, "FIRST_FIT");

    while (fgets(buf, sizeof(buf), arq)) {
        /* Pula linhas vazias */
        if (buf[0] == '\n' || buf[0] == '\r')
            continue;

        /* Linhas de configuracao */
        if (buf[0] == '#') {
            if (strstr(buf, "Algoritmo:"))
                sscanf(buf, "# Algoritmo: %19s", algoritmo);
            else if (strstr(buf, "Memoria:"))
                sscanf(buf, "# Memoria: %d", &tam_mem);
            continue;
        }

        if (*n_eventos >= MAX_EVENTOS)
            break;

        char cmd[16], id_str[16];
        Evento *ev = &eventos[*n_eventos];
        int n = sscanf(buf, "%15s %15s %d", cmd, id_str, &ev->tamanho);

        if (n < 2) continue;

        /* Extrai numero do id (P1 -> 1, P12 -> 12) */
        ev->pid = atoi(id_str + 1);

        if (strcmp(cmd, "ALOCAR") == 0 && n >= 3) {
            ev->tipo = 0;
        } else if (strcmp(cmd, "LIBERAR") == 0) {
            ev->tipo = 1;
            ev->tamanho = 0;
        } else {
            continue;
        }

        (*n_eventos)++;
    }

    fclose(arq);
    mem_init(mem, tam_mem);

    printf("--- Entrada carregada ---\n");
    printf("Arquivo   : %s\n", caminho);
    printf("Algoritmo : %s\n", algoritmo);
    printf("Memoria   : %d\n", tam_mem);
    printf("Eventos   : %d\n", *n_eventos);
    printf("-------------------------\n");
}
