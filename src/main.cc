#include "file.hh"
#include "graph_treatment.hh"
#include <chrono>

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        std::cout << "ERROR: execution format is ./be <in_filei>" << std::endl;
        return -1;
    }
    const char *ofilename = "out.txt";
    const char *ifilename = argv[1];

    if (argc > 2)
    {
        ofilename = argv[2];
    }

    igraph_t graph;
    igraph_vector_t eccen;
    igraph_vector_ptr_t wcc;
    igraph_real_t count_bfs = 0;

    int err = graph_from_file(&graph, argv[1]);
    err = get_wcc(&graph, &wcc);

    auto start = std::chrono::high_resolution_clock::now();
    err &= bounding_eccentricities((igraph_t *)VECTOR(wcc)[0], &eccen, &count_bfs);
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Time to compute eccentricity for " << argv[1] << " was "
        << duration.count() << " microseconds" << std::endl;
    err &= eccentricities_to_file(&eccen, count_bfs, ofilename);
    igraph_vector_destroy(&eccen);
    igraph_destroy(&graph);
    igraph_decompose_destroy(&wcc);
    igraph_vector_ptr_destroy(&wcc);

    return err;
}
