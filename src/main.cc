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

    int err = graph_from_file(&graph, argv[1]);
//    err = get_wcc(&graph, &wcc);
    void *a = malloc(sizeof(int) * 3);


    auto start = std::chrono::high_resolution_clock::now();
    err &= bounding_eccentricities(&graph, &eccen);
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Time taken by function: "
         << duration.count() << " microseconds" << std::endl;
    err &= eccentricities_to_file(&eccen, ofilename);

    return err;
}
