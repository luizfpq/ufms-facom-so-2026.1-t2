/*
 * memoria.h — Structs e prototipos do simulador
 *
 * UFMS — FACOM — Sistemas Operacionais 2026.1
 * Luiz Quirino <luiz.quirino@ufms.br>
 *
 * Ref: Silberschatz, Galvin, Gagne. Operating System Concepts.
 *      10 ed. Wiley, 2018. Cap. 9 (particoes variaveis).
 */

#ifndef MEMORIA_H
#define MEMORIA_H

#define MAX_BLOCOS    200
#define MAX_EVENTOS   100

/* Um bloco contiguo de memoria (ocupado ou livre) */
typedef struct {
    int inicio;        /* endereco inicial */
    int tamanho;
    int pid;           /* id do processo, 0 = livre */
} Bloco;

/* Evento lido do arquivo de entrada */
typedef struct {
    int tipo;          /* 0 = alocar, 1 = liberar */
    int pid;
    int tamanho;       /* so usado em alocacao */
} Evento;

/* Estado completo da simulacao */
typedef struct {
    Bloco blocos[MAX_BLOCOS];
    int n_blocos;
    int tam_total;
    int falhas;        /* alocacoes que nao couberam */
    int frag_externas; /* vezes que havia espaco total mas nao contiguo */
} Memoria;

/* arquivo.c */
void carregar_entrada(const char *caminho, Memoria *mem,
                      Evento eventos[], int *n_eventos,
                      char *algoritmo);

/* memoria_ops.c */
void mem_init(Memoria *mem, int tamanho);
int  first_fit(Memoria *mem, int pid, int tam);
int  best_fit(Memoria *mem, int pid, int tam);
int  worst_fit(Memoria *mem, int pid, int tam);
void mem_liberar(Memoria *mem, int pid);
void mem_coalescer(Memoria *mem);
int  tem_fragmentacao_externa(Memoria *mem, int tam);

/* visualizacao.c */
void mostrar_memoria(Memoria *mem, int passo);
void mostrar_evento(int passo, Evento *ev, int ok);
void mostrar_relatorio(Memoria *mem, int n_eventos);

#endif
