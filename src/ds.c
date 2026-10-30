#include "ds.h"
#include "utils.h"
#include <string.h>

void insert_edge(node_t **graph, int u, int v, int weight) {
  node_t *n = (node_t *)malloc(sizeof(node_t));
  n->weight = weight;
  n->destination = v;
  n->next = graph[u];
  graph[u] = n;
}

void remove_edge(node_t **graph, int u, int v) {
  if (graph == NULL || graph[u] == NULL) {
    return;
  }

  node_t *current = graph[u];
  node_t *previous = NULL;

  while (current != NULL) {
    if (current->destination == v) {
      if (previous == NULL) {
        graph[u] = current->next;
      } else {
        previous->next = current->next;
      }

      free(current);
      return;
    }

    previous = current;
    current = current->next;
  }
}

edge_t *get_all_edges(node_t **graph, size_t n, size_t m) {
  if (graph == NULL || m == 0) {
    return NULL;
  }

  size_t total_entries = 2 * m;

  edge_t *edges = (edge_t *)malloc(sizeof(edge_t) * total_entries);
  if (edges == NULL) {
    return NULL;
  }

  size_t idx = 0;

  for (size_t u = 0; u < n; u++) {
    node_t *curr = graph[u];
    while (curr != NULL) {
      if (idx < total_entries) {
        edges[idx++] = (edge_t){
            .src = (int)u, .dest = curr->destination, .weight = curr->weight};
      }
      curr = curr->next;
    }
  }

  return edges;
}

bool dfs(node_t **graph, int current, int target, int *visited,
         size_t current_weight, size_t normalized_weight, size_t B, size_t M) {
  if (max(current_weight, normalized_weight) > B) {
    return false;
  }
  if (current == target) {
    return true;
  }

  visited[current] = 1;
  node_t *node = graph[current];

  while (node != NULL) {
    if (!visited[node->destination]) {
      if (dfs(graph, node->destination, target, visited,
              current_weight + node->weight,
              normalized_weight + (M - node->weight), B, M)) {
        return true;
      }
    }
    node = node->next;
  }

  return false;
}
