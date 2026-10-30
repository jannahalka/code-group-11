#include "utils.h"
#include <stdio.h>

void print_graph(node_t **graph, size_t n) {
  for (u_int8_t i = 0; i < n; i++) {
    printf("------- NODE %d -------\n", i + 1);
    node_t *node = graph[i];
    while (node != NULL) {
      printf("dest: %d, weight: %d\n", node->destination + 1, node->weight);
      node = node->next;
    }
  }
}

size_t max(size_t a, size_t b) {
  if (a > b) {
    return a;
  } else if (b > a) {
    return b;
  }

  return a; // a == b
}
