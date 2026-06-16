/*
 * memoria_ops.c — Operacoes de alocacao e liberacao
 *
 * UFMS — FACOM — SO 2026.1
 * Luiz Quirino <luiz.quirino@ufms.br>
 *
 * Baseado em Silberschatz et al. (2018), p. 391-393.
 * First Fit, Best Fit, Worst Fit.
 */

#include <stdio.h>
#include "memoria.h"

void mem_init(Memoria *mem, int tamanho)
{
    mem->tam_total      = tamanho;
    mem->n_blocos       = 1;
    mem->falhas         = 0;
    mem->frag_externas  = 0;

    /* Comeca com um unico bloco livre ocupando tudo */
    mem->blocos[0].inicio  = 0;
    mem->blocos[0].tamanho = tamanho;
    mem->blocos[0].pid     = 0;
}

/*
 * Divide o bloco[idx] em dois: a parte alocada e o resto livre.
 * Retorna 1 se deu certo, 0 se estourou o vetor.
 */
static int dividir_e_alocar(Memoria *mem, int idx, int pid, int tam)
{
    Bloco *b = &mem->blocos[idx];
    int resto = b->tamanho - tam;

    if (resto > 0) {
        /* Precisa inserir um bloco novo depois */
        if (mem->n_blocos >= MAX_BLOCOS)
            return 0;

        /* Desloca tudo pra frente */
        for (int i = mem->n_blocos; i > idx + 1; i--)
            mem->blocos[i] = mem->blocos[i - 1];

        mem->n_blocos++;
        mem->blocos[idx + 1].inicio  = b->inicio + tam;
        mem->blocos[idx + 1].tamanho = resto;
        mem->blocos[idx + 1].pid     = 0;
    }

    b->tamanho = tam;
    b->pid     = pid;
    return 1;
}

/* Primeiro bloco livre que cabe */
int first_fit(Memoria *mem, int pid, int tam)
{
    for (int i = 0; i < mem->n_blocos; i++) {
        if (mem->blocos[i].pid == 0 && mem->blocos[i].tamanho >= tam)
            return dividir_e_alocar(mem, i, pid, tam);
    }
    return 0;
}

/* Menor bloco livre que cabe */
int best_fit(Memoria *mem, int pid, int tam)
{
    int escolhido = -1;
    int menor_resto = mem->tam_total + 1;

    for (int i = 0; i < mem->n_blocos; i++) {
        if (mem->blocos[i].pid == 0 && mem->blocos[i].tamanho >= tam) {
            int r = mem->blocos[i].tamanho - tam;
            if (r < menor_resto) {
                menor_resto = r;
                escolhido = i;
            }
        }
    }
    if (escolhido < 0) return 0;
    return dividir_e_alocar(mem, escolhido, pid, tam);
}

/* Maior bloco livre disponivel */
int worst_fit(Memoria *mem, int pid, int tam)
{
    int escolhido = -1;
    int maior_resto = -1;

    for (int i = 0; i < mem->n_blocos; i++) {
        if (mem->blocos[i].pid == 0 && mem->blocos[i].tamanho >= tam) {
            int r = mem->blocos[i].tamanho - tam;
            if (r > maior_resto) {
                maior_resto = r;
                escolhido = i;
            }
        }
    }
    if (escolhido < 0) return 0;
    return dividir_e_alocar(mem, escolhido, pid, tam);
}

/* Marca bloco do processo como livre */
void mem_liberar(Memoria *mem, int pid)
{
    for (int i = 0; i < mem->n_blocos; i++) {
        if (mem->blocos[i].pid == pid) {
            mem->blocos[i].pid = 0;
            return;
        }
    }
    printf("  [!] P%d nao esta na memoria\n", pid);
}

/* Junta blocos livres adjacentes (coalescing) */
void mem_coalescer(Memoria *mem)
{
    int i = 0;
    while (i < mem->n_blocos - 1) {
        if (mem->blocos[i].pid == 0 && mem->blocos[i + 1].pid == 0) {
            mem->blocos[i].tamanho += mem->blocos[i + 1].tamanho;
            /* Remove bloco i+1 */
            for (int j = i + 1; j < mem->n_blocos - 1; j++)
                mem->blocos[j] = mem->blocos[j + 1];
            mem->n_blocos--;
            /* Nao incrementa i, pode ter outro livre depois */
        } else {
            i++;
        }
    }
}

/* Verifica se ha espaco total suficiente (mas nao contiguo) */
int tem_fragmentacao_externa(Memoria *mem, int tam)
{
    int livre_total = 0;
    for (int i = 0; i < mem->n_blocos; i++) {
        if (mem->blocos[i].pid == 0)
            livre_total += mem->blocos[i].tamanho;
    }
    /* Fragmentacao externa = tem espaco no total, mas nenhum bloco cabe */
    return (livre_total >= tam);
}

/* Verifica se um processo ja esta alocado na memoria */
int mem_pid_existe(Memoria *mem, int pid)
{
    for (int i = 0; i < mem->n_blocos; i++) {
        if (mem->blocos[i].pid == pid)
            return 1;
    }
    return 0;
}
