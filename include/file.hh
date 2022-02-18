#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <igraph.h>

int graph_from_file(igraph_t *graph, const char *ifile_name);
int eccentricities_to_file(igraph_vector_t *eccen, const char *ofile_name);

#endif
