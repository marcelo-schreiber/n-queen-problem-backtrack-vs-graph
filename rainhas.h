#ifndef RAINHAS_H
#define RAINHAS_H
#include <stdbool.h>

//------------------------------------------------------------------------------
// representa uma casa do tabuleiro
//

typedef struct casa {
  unsigned int linha, coluna;
} casa;


#define MAX_VERTICES 1024

typedef struct Graph {
    unsigned int num_vertices;
    unsigned int adjacency_matrix[MAX_VERTICES][MAX_VERTICES];
} Graph;
//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n rainhas 
// com casas proibidas usando backtracking
//
//    n é o tamanho (número de linhas/colunas) do tabuleiro
//
//    c é um vetor de k 'struct casa' indicando as casas proibidas
//
//    r é um vetor de n posições (já alocado) a ser preenchido com a resposta:
//      r[i] = j > 0 indica que a rainha da linha i+1 fica na coluna j;
//      r[i] = 0     indica que não há rainha nenhuma na linha i+1
//
// devolve r

unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r);

//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n rainhas 
// com casas proibidas usando backtracking
//
// n, c, r e o valor de retorno são como em rainhas_bt

unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r);
void initialize_nqueens_graph(Graph *g, unsigned int n, casa *c, unsigned int k);

bool is_safe(unsigned int row, unsigned int col, unsigned int *board, casa *proibido, unsigned int k);
bool solve_nqueens(unsigned int row, unsigned int n, unsigned int *board, casa *proibido, unsigned int k);
void initialize_graph(Graph *g, unsigned int num_vertices);
unsigned int position_to_vertex(unsigned int row, unsigned int col, unsigned int n);
int find_independent_set(Graph *g, unsigned int n, unsigned int *result);
void add_edge(Graph *g, unsigned int src, unsigned int dest);
int is_independent(Graph *g, unsigned int *I, unsigned int I_size, unsigned int v);
int ConjIndep(Graph *g, unsigned int *I, unsigned int I_size, unsigned int *C, unsigned int C_size, unsigned int n, unsigned int *result);

#endif
