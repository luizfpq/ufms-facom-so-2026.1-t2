/*
 * visualizacao.c — Saida textual do simulador
 *
 * UFMS — FACOM — SO 2026.1
 * Luiz Quirino <luiz.quirino@ufms.br>
 */

#include <stdio.h>
#include "memoria.h"

void mostrar_memoria(Memoria *mem, int passo)
{
    printf("\n  [Passo %d] Memoria (%d unidades):\n", passo, mem->tam_total);
    printf("  +");
    for (int i = 0; i < 56; i++) printf("-");
    printf("+\n");

    for (int i = 0; i < mem->n_blocos; i++) {
        Bloco *b = &mem->blocos[i];
        if (b->pid == 0)
            printf("  | LIVRE   ");
        else
            printf("  | P%-6d ", b->pid);

        printf("inicio=%-5d tam=%-5d", b->inicio, b->tamanho);

        /* Preenche ate fechar a caixa */
        int usado = 10 + 20;  /* chars ja impressos apos "  | " */
        for (int s = usado; s < 52; s++) printf(" ");
        printf("|\n");
    }

    printf("  +");
    for (int i = 0; i < 56; i++) printf("-");
    printf("+\n");

    /* Barra visual: cada char = ~2% da memoria */
    printf("  ");
    for (int i = 0; i < mem->n_blocos; i++) {
        Bloco *b = &mem->blocos[i];
        int larg = (b->tamanho * 50) / mem->tam_total;
        if (larg < 1) larg = 1;
        char ch = (b->pid == 0) ? '.' : ('0' + (b->pid % 10));
        for (int j = 0; j < larg; j++)
            putchar(ch);
    }
    printf("\n");
}

void mostrar_evento(int passo, Evento *ev, int status)
{
    printf("\n");
    if (ev->tipo == 0) {
        const char *msg;
        switch (status) {
            case ST_OK:
                msg = "OK";
                break;
            case ST_FRAG_EXTERNA:
                msg = "FALHOU (fragmentacao externa: ha espaco total, mas nao contiguo)";
                break;
            case ST_SEM_MEMORIA:
                msg = "FALHOU (memoria livre insuficiente)";
                break;
            case ST_TAM_INVALIDO:
                msg = "REJEITADO (tamanho invalido, deve ser > 0)";
                break;
            case ST_PID_DUPLICADO:
                msg = "REJEITADO (processo ja alocado na memoria)";
                break;
            default:
                msg = "FALHOU";
        }
        printf("  [%d] ALOCAR P%d (%d un.) -> %s\n",
               passo, ev->pid, ev->tamanho, msg);
    } else {
        printf("  [%d] LIBERAR P%d\n", passo, ev->pid);
    }
}

void mostrar_relatorio(Memoria *mem, int n_eventos)
{
    int ocupado = 0, livre = 0, brechas = 0;

    for (int i = 0; i < mem->n_blocos; i++) {
        if (mem->blocos[i].pid == 0) {
            livre += mem->blocos[i].tamanho;
            brechas++;
        } else {
            ocupado += mem->blocos[i].tamanho;
        }
    }

    float uso = 100.0f * ocupado / mem->tam_total;

    printf("\n");
    printf("=========================================\n");
    printf(" RELATORIO FINAL\n");
    printf("=========================================\n");
    printf(" Memoria total          : %d\n", mem->tam_total);
    printf(" Ocupado                : %d\n", ocupado);
    printf(" Livre                  : %d\n", livre);
    printf(" Brechas                : %d\n", brechas);
    printf(" Utilizacao             : %.1f%%\n", uso);
    printf(" Eventos processados    : %d\n", n_eventos);
    printf(" Alocacoes que falharam : %d\n", mem->falhas);
    printf(" Fragmentacao externa   : %d ocorrencia(s)\n", mem->frag_externas);
    printf("=========================================\n\n");
}
