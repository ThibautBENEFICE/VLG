#ifndef GRAPH_TREATMENT_H
#define GRAPH_TREATMENT_H

#include <queue>
#include <vector>
#include <igraph.h>
#include <climits>
#include <cstdlib>
#include <unordered_set>

int bounding_eccentricities(igraph_t *graph, igraph_vector_t *eccen);
int get_wcc(igraph_t *graph, igraph_vector_ptr_t *wcc);

#endif
