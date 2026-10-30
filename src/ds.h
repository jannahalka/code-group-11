#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct node {
  int destination;
  int weight;
  struct node *next;
} node_t;

typedef struct {
  int src;
  int dest;
  int weight;
} edge_t;

// Data structures
void insert_edge(node_t **graph, int u, int v, int weight);
void remove_edge(node_t **graph, int u, int v);
edge_t *get_all_edges(node_t **graph, size_t n, size_t m);

// Algorithms
bool dfs(node_t **graph, int current, int target, int *visited,
         int current_weight, int normalized_weight, int B, int M);
