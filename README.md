# VLG S8 2022 - Thibaut BENEFICE

This project is an implementation of the Bounding Eccentricities algorithm in
c/c++.

Given an input graph file (undirected and in edgelist format) we try to find
the eccentricity of all nodes while reducing as much as possible the number of
bfs required.

There are 2 main sections to this project: the bounding eccentricities algorithm
and the method of selection of the
successive nodes for which we compute their eccentricity (the select_next_root
function).

## How to build the program:

- release build: `./run.sh`
- testsuite + leak_checks + debugmode: `./run_tests.sh`

#### Warning

Some include locations may differ depending on where you installed
igraph. Please adapt path values in CMakelists.txt for include_directories
and link_directories.

## How to execute the program:
`./be <input_file> [<output_file>]`

if output_file is not specified out.txt will be chosen be chosen by default. The
output file contains:
    - the number of BFS (each time eccentricity is called)
    - the eccentricity of each node

#### Warning

This program only computes the eccentricity of the largest WCC.
