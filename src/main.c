#include "ds.h"
#include "utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool has_path(node_t **graph, size_t n, int src, int target, size_t B,
              size_t M) {
  int *visited = (int *)calloc(n, sizeof(int));
  if (visited == NULL) {
    return false;
  }

  bool path_exists = dfs(graph, src, target, visited, 0, 0, B, M);

  free(visited);
  return path_exists;
}

int find_boundary_value(node_t **graph, size_t n, size_t M) {
  int low = 0;
  int high = (n - 1) * M;
  int optimal_B = -1;

  if (!has_path(graph, n, 0, n - 1, high, M)) {
    return -1;
  }

  while (low <= high) {
    int mid = low + (high - low) / 2;

    if (has_path(graph, n, 0, n - 1, mid, M)) {
      optimal_B = mid;
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }

  return optimal_B;
}

int main() {
  char *line = NULL;
  size_t len = 0;

  size_t line_number = 0;
  size_t n = 0;
  size_t m = 0;
  size_t M = 0;
  node_t **graph = NULL;

  while ((line = fgetln(stdin, &len)) != NULL) {
    line_number++;

    if (line_number == 1) {
      n = (size_t)atoi(line);
      if (graph == NULL) {
        graph = (node_t **)malloc(sizeof(node_t *) * n);
        for (size_t i = 0; i < n; i++) {
          graph[i] = NULL;
        }
      }
    } else if (line_number == 2) {
      m = (size_t)atoi(line);
    } else if (line_number == 3) {
      M = (size_t)atoi(line);
    } else {
      char buf[1024];
      snprintf(buf, sizeof(buf), "%.*s", (int)len, line);
      char *tok = strtok(buf, " ");

      u_int32_t j = (u_int32_t)atoi(tok);

      u_int32_t k = 0;
      u_int32_t w = 0;
      u_int8_t i = 0;

      while (tok != NULL) {
        tok = strtok(NULL, " ");
        if (i == 0) {
          k = (u_int32_t)atoi(tok);
        } else if (i == 1) {
          w = (u_int32_t)atoi(tok);
          insert_edge(graph, j - 1, k - 1, w);
          insert_edge(graph, k - 1, j - 1, w);
        }
        i++;
      }
    }
  }

  // ------------ Optimization part ------------
  int B = find_boundary_value(graph, n, M);

  if (B < 0) {
    fprintf(stdout, "NO\n");
    exit(EXIT_SUCCESS);
  }

  edge_t *edges = get_all_edges(graph, n, m);

  edge_t **path = (edge_t **)malloc(sizeof(edge_t *) * (n - 1));
  for (int i = 0; i < (int)n - 1; i++) {
    path[i] = NULL;
  }

  size_t path_count = 0;

  for (size_t i = 0; i < 2 * m; i++) {
    // Skip the reverse representation of undirected edges
    if (edges[i].src > edges[i].dest) {
      continue;
    }

    remove_edge(graph, edges[i].src, edges[i].dest);
    remove_edge(graph, edges[i].dest, edges[i].src);

    bool path_exists = has_path(graph, n, 0, n - 1, B, M);

    insert_edge(graph, edges[i].src, edges[i].dest, edges[i].weight);
    insert_edge(graph, edges[i].dest, edges[i].src, edges[i].weight);

    if (!path_exists && path_count < n - 1) {
      path[path_count++] = &edges[i];
    }
  }

  // ------------ Output the results ------------
  for (size_t i = 0; i < path_count; i++) {
    fprintf(stdout, "%d %d\n", path[i]->src + 1, path[i]->dest + 1);
  }

  size_t path_sum = 0;
  size_t path_sum_M = 0;
  for (size_t i = 0; i < path_count; i++) {
    path_sum += path[i]->weight;
    path_sum_M += M - path[i]->weight;
  }

  size_t solution = max(path_sum, path_sum_M);

  fprintf(stdout, "%ld\n", solution);

  free(edges);
  free(path);

  for (size_t i = 0; i < n; i++) {
    node_t *curr = graph[i];
    while (curr != NULL) {
      node_t *tmp = curr;
      curr = curr->next;
      free(tmp);
    }
  }
  free(graph);
  exit(EXIT_SUCCESS);
}
