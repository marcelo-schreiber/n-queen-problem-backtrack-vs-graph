#include "rainhas.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 1024

typedef struct Graph {
    unsigned int num_vertices;
    unsigned int adjacency_matrix[MAX_VERTICES][MAX_VERTICES];
} Graph;

// * Backtracking solution
bool is_safe(unsigned int row, unsigned int col, unsigned int *board, casa *proibido, unsigned int k) {
    for (unsigned int i = 0; i < row; i++) { 
        // column
        if (board[i] == col) return false;

        // diagonal 
        if (abs((int)(board[i] - col)) == (int)(row - i)) return false;
    }
    
    // prohibited positions
    for (unsigned int i = 0; i < k; i++) {
        if (proibido[i].linha == row + 1 && proibido[i].coluna == col + 1) {
            return false;
        }
    }
    
    return true;
}


bool solve_nqueens(unsigned int row, unsigned int n, unsigned int *board, casa *proibido, unsigned int k) {
    if (row == n) return true;

    for (unsigned int col = 1; col <= n; col++) {
        if (is_safe(row, col, board, proibido, k)) {
            board[row] = col;
            if (solve_nqueens(row + 1, n, board, proibido, k)) return true;
            board[row] = 0; // backtrack
        }
    }

    return false;
}

//------------------------------------------------------------------------------
// computa uma resposta para a instância (n,c) do problema das n
// rainhas com casas proibidas usando a modelagem do problema como
// conjunto independente de um grafo
//
// n, c e r são como em rainhas_bt()
unsigned int *rainhas_bt(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
    if (solve_nqueens(0, n, r, c, k)) {
        return r;
    } else {
        for (unsigned int i = 0; i < n; i++) {
            r[i] = 0; // No solution found
        }
        return r;
    }
}

// * Graph solution
void initialize_graph(Graph *g, unsigned int num_vertices) {
    g->num_vertices = num_vertices;
}

void add_edge(Graph *g, unsigned int src, unsigned int dest) {
    g->adjacency_matrix[src][dest] = 1;
    g->adjacency_matrix[dest][src] = 1;
}

int is_independent(Graph *g, unsigned int *I, unsigned int I_size, unsigned int v) {
    for (unsigned int i = 0; i < I_size; i++) {
        if (g->adjacency_matrix[I[i]][v]) {
            return 0;
        }
    }
    return 1;
}

int ConjIndep(Graph *g, unsigned int *I, unsigned int I_size, unsigned int *C, unsigned int C_size, unsigned int n, unsigned int *result) {
    if (I_size == n) {
        memcpy(result, I, n * sizeof(unsigned int));
        return 1;
    }

    if (C_size == 0) {
        return 0;
    }

    for (unsigned int i = 0; i < C_size; i++) {
        unsigned int v = C[i];

        if (is_independent(g, I, I_size, v)) {
            I[I_size] = v;
            unsigned int new_C[MAX_VERTICES];
            unsigned int new_C_size = 0;

            for (unsigned int j = i + 1; j < C_size; j++) {
                if (g->adjacency_matrix[v][C[j]] == 0) {
                    new_C[new_C_size++] = C[j];
                }
            }

            if (ConjIndep(g, I, I_size + 1, new_C, new_C_size, n, result)) {
                return 1;
            }
        }
    }

    return 0;
}

// Function to find an independent set of a given size
int find_independent_set(Graph *g, unsigned int n, unsigned int *result) {
    unsigned int I[MAX_VERTICES], C[MAX_VERTICES];
    unsigned int I_size = 0, C_size = 0;

    for (unsigned int i = 0; i < g->num_vertices; i++) {
        C[C_size++] = i;
    }

    return ConjIndep(g, I, I_size, C, C_size, n, result);
}

// Function to convert 2D board position to graph vertex index
unsigned int position_to_vertex(unsigned int row, unsigned int col, unsigned int n) {
    return row * n + col;
}

// Function to initialize the graph for the N-Queens problem
void initialize_nqueens_graph(Graph *g, unsigned int n, casa *c, unsigned int k) {
    initialize_graph(g, n * n);

    // Add edges for attack positions (same row, column, or diagonal)
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            for (unsigned int x = 0; x < n; x++) {
                for (unsigned int y = 0; y < n; y++) {
                    if (i == x || j == y || abs((int) (i - x)) == abs((int)(j - y))) {
                        if (!(i == x && j == y)) {
                            add_edge(g, position_to_vertex(i, j, n), position_to_vertex(x, y, n));
                        }
                    }
                }
            }
        }
    }

    // Remove forbidden positions
    for (unsigned int i = 0; i < k; i++) {
        unsigned int v = position_to_vertex(c[i].linha, c[i].coluna, n);
        for (unsigned int j = 0; j < g->num_vertices; j++) {
            g->adjacency_matrix[v][j] = 1;
            g->adjacency_matrix[j][v] = 1;
        }
    }
}

// Function to solve the N-Queens problem using independent sets
unsigned int *rainhas_ci(unsigned int n, unsigned int k, casa *c, unsigned int *r) {
    Graph g;
    
    initialize_nqueens_graph(&g, n, c, k);
    find_independent_set(&g, n, r);

    for (unsigned int i = 0; i < n; i++) {
        r[i] = r[i] % n + 1;
    }

    return r;
}
