#include "graph_treatment.hh"
#include <iostream>


//FIXME consider changing calls to igraph_vector_e(v, i) to VECTOR(v)[i]

int get_wcc(igraph_t *graph,
            igraph_vector_ptr_t *wcc)
{
    igraph_real_t maxcompno = 1;
    igraph_real_t minelts = 2;
    igraph_connectedness_t mode = IGRAPH_WEAK;
    return igraph_decompose(graph, wcc, mode, maxcompno, minelts);
}

int compute_eccentricity(igraph_t *graph,
        igraph_integer_t root,
        igraph_vector_t *eccen,
        igraph_vector_t *dist,
        igraph_vector_t *cand)
{
    int err;
    igraph_integer_t cur_v, ecc, nb_neighbors = 0;
    igraph_vector_t cur_neighbors;
    std::queue<igraph_integer_t> q;

    igraph_vector_init(&cur_neighbors, 0);
    igraph_vector_fill(dist, -1);
    VECTOR(*dist)[root] = 0;
    q.push(root);

    while (!q.empty())
    {
        cur_v = q.front();
        q.pop();

        err = igraph_neighbors(graph, &cur_neighbors, cur_v, IGRAPH_OUT);
        nb_neighbors= igraph_vector_size(&cur_neighbors);
        for (igraph_real_t j = 0; j < nb_neighbors; j++)
        {
            igraph_real_t neighbor_j = igraph_vector_e(&cur_neighbors, j);
            if (igraph_vector_e(dist, neighbor_j) == -1)
            {
                igraph_integer_t dist_cur = igraph_vector_e(dist, cur_v);
                igraph_vector_set(dist, neighbor_j, dist_cur + 1);
                q.push(neighbor_j);
                ecc = std::max(ecc, dist_cur + 1);
            }
        }
    }
    VECTOR(*eccen)[root] = ecc;
    return 0;
}

igraph_integer_t select_next_root(igraph_t *graph,
        igraph_vector_t *eccen,
        igraph_vector_t *cand)
{
    //FIXME risk of repeatedly selecting the same root and not updating it.
    igraph_integer_t selected;
    selected = igraph_vector_which_max(cand); // igraph_vector_which_min(eccen)
    return selected;
}

int bounding_eccentricities(igraph_t *graph, igraph_vector_t *eccen)
{
    // FIXME we suppose graph is already the wcc of the initial graph.

    igraph_real_t nb_v = igraph_vcount(graph);

    igraph_vector_t ecc_l;
    igraph_vector_t ecc_u;
    igraph_vector_t cand;
    igraph_vector_t dist;

    igraph_vector_init(eccen, nb_v);
    igraph_vector_init(&ecc_l, nb_v);
    igraph_vector_init(&ecc_u, nb_v);
    igraph_vector_init(&cand, nb_v);
    igraph_vector_init(&dist, nb_v);

    igraph_vector_fill(&ecc_l, LONG_MIN);
    igraph_vector_fill(&ecc_u, LONG_MAX);
    igraph_vector_fill(&cand, 1);
    while (igraph_vector_contains(&cand, 1))
    {
        igraph_integer_t root = select_next_root(graph, eccen, &cand);
        compute_eccentricity(graph, root, eccen, &dist, &cand);
        for (igraph_integer_t w = 0; w < nb_v; w++)
        {
            if (igraph_vector_e(&cand, w) == 0)
                continue; //we only consider candidate vertices
            igraph_real_t d_rw = igraph_vector_e(&dist, w);
            igraph_real_t eccr = igraph_vector_e(eccen, root);
            igraph_real_t loww = igraph_vector_e(&ecc_l, w);
            igraph_real_t uppw = igraph_vector_e(&ecc_u, w);
            VECTOR(ecc_l)[w] = std::max(loww, std::max(eccr - d_rw, d_rw));
            VECTOR(ecc_u)[w] = std::min(uppw, eccr + d_rw);

            if (igraph_vector_e(&ecc_l, w) == igraph_vector_e(&ecc_u, w))
            {
                VECTOR(*eccen)[w] =igraph_vector_e(&ecc_l, w);
                VECTOR(cand)[w] = 0;
            }
        }
    }

    return 0;
}
