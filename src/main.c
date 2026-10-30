#include "ds.h"
#include "utils.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool has_path(node_t **graph, size_t n, int src, int target, int B, int M) {
  int *visited = (int *)calloc(n, sizeof(int));
  if (visited == NULL) {
    return false;
  }

  bool path_exists = dfs(graph, src, target, visited, 0, 0, B, M);

  free(visited);
  return path_exists;
}

int find_boundary_value(node_t **graph, size_t n, int M) {
  int low = 0;
  int high = ((int)n - 1) * M;
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

int main(int argc, char **argv) {
  FILE *fp = fopen("./data/test01.uwg", "r");
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  if (fp == NULL) {
    exit(EXIT_FAILURE);
  }

  size_t line_number = 0;
  size_t n = 0;
  size_t m = 0;
  size_t M = 0;
  node_t **graph = NULL;

  while ((read = getline(&line, &len, fp)) != -1) {
    line_number++;

    if (line_number == 1) {
      n = (u_int8_t)atoi(line);
      if (graph == NULL) {
        graph = (node_t **)malloc(sizeof(node_t *) * n);
        for (u_int8_t i = 0; i < n; i++) {
          graph[i] = NULL;
        }
      }
    } else if (line_number == 2) {
      m = (u_int8_t)atoi(line);
    } else if (line_number == 3) {
      M = (u_int8_t)atoi(line);
    } else {
      char *tok = strtok(line, " ");
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
  int B = find_boundary_value(graph, n, 10);

  if (B < 0) {
    printf("no path found\n");
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

    if (has_path(graph, n, 0, n - 1, B, 10)) {
      insert_edge(graph, edges[i].src, edges[i].dest, edges[i].weight);
      insert_edge(graph, edges[i].dest, edges[i].src, edges[i].weight);
    } else {
      insert_edge(graph, edges[i].src, edges[i].dest, edges[i].weight);
      insert_edge(graph, edges[i].dest, edges[i].src, edges[i].weight);

      if (path_count < n - 1) {
        path[path_count++] = &edges[i];
      }
    }
  }

  for (size_t i = 0; i < path_count; i++) {
    printf("src: %d, dest: %d\n", path[i]->src, path[i]->dest);
  }

  fclose(fp);
  if (line)
    free(line);

  free(edges);
  free(path);
  free(graph);
  exit(EXIT_SUCCESS);
}
