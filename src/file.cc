#include "file.hh"

int file_check(std::ifstream instream)
{
    // Use this function if different file types
    return 0;
}

//FIXME first iteration for undirected edge list in facebook.txt.

int graph_from_file(igraph_t *graph, const char* ifile_name)
{
    FILE *file = fopen(ifile_name, "r");
    int check = 0;
    check = igraph_read_graph_edgelist(graph, file, 0, false);
    fclose(file);
    return check;
}

int eccentricities_to_file(igraph_vector_t *eccen, const char *ofile_name)
{
    auto nb_v = igraph_vector_size(eccen);
    std::ofstream ofile(ofile_name);
    for (igraph_integer_t i = 0; i < nb_v; i++)
    {
        ofile << i << " " << igraph_vector_e(eccen, i) << std::endl;
    }
    ofile.flush();
    ofile.close();
    return 0;
}
