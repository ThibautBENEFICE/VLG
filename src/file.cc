#include "file.hh"

int graph_from_file(igraph_t *graph, const char* ifile_name)
{
    FILE *file = fopen(ifile_name, "r");
    int check = 0;
    check = igraph_read_graph_edgelist(graph, file, 0, false);
    fclose(file);
    return check;
}

int eccentricities_to_file(igraph_vector_t *eccen,
        igraph_real_t count_bfs,
        const char *ofile_name)
{
    auto nb_v = igraph_vector_size(eccen);
    std::ofstream ofile(ofile_name);
    ofile << "number of BFS: " << count_bfs << std::endl;
    for (igraph_integer_t i = 0; i < nb_v; i++)
    {
        ofile << i << " " << igraph_vector_e(eccen, i) << std::endl;
    }
    ofile.flush();
    ofile.close();
    return 0;
}
