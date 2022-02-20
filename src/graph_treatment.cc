#include "graph_treatment.hh"
#include <iostream>


int get_wcc(igraph_t *graph,
        igraph_vector_ptr_t *wcc)
{
    igraph_real_t maxcompno = 1;
    igraph_real_t minelts = 2;
    igraph_connectedness_t mode = IGRAPH_WEAK;
    igraph_vector_ptr_init(wcc, maxcompno);
    return igraph_decompose(graph, wcc, mode, maxcompno, minelts);
}

int get_commu(igraph_t *graph,
        igraph_vector_t *membership)
{
    igraph_community_multilevel(graph, NULL, 1, membership, NULL, NULL);
    return 0;
}


int community_mask_init(igraph_vector_t *membership,
        igraph_real_t nb_commu,
        igraph_real_t nb_v,
        std::vector<std::vector<bool>> &comm_mask)
{
    comm_mask.resize(nb_commu);
    for (auto it = comm_mask.begin(); it != comm_mask.end(); it++)
    {
        it->resize(nb_v);
        std::fill(it->begin(), it->end(), false);
    }
    for (auto i = 0; i < nb_v; i++)
    {
        igraph_real_t commu = igraph_vector_e(membership, i);
        comm_mask[commu][i] = true;
    }
    return 0;
}

void mask(std::vector<bool> comm_mask,
        std::vector<igraph_real_t> in,
        std::vector<igraph_real_t> &out)
{
    auto size = comm_mask.size();
    for (auto i = 0; i < size; i++)
    {
        if (comm_mask[i])
            out[i] = in[i];
        else
            out[i] = -1;
    }
}

igraph_integer_t commu_max_delta_sum(
        std::vector<igraph_real_t> &delta,
        std::vector<std::vector<bool>> &comm_mask)
{
    igraph_integer_t max_commu, nb_commu, nb_v;
    std::vector<igraph_real_t> sum_delta_commu, mask_i;
    nb_commu = comm_mask.size();
    nb_v = delta.size();
    sum_delta_commu.resize(nb_commu);
    mask_i.resize(nb_v);
    for (auto i = 0; i < nb_commu; i++)
    {
        sum_delta_commu[i] = 0;
        mask(comm_mask[i], delta, mask_i);
        for (auto j = 0; j < nb_v; j++)
            sum_delta_commu[i] += mask_i[j];
    }
    return std::max_element(sum_delta_commu.begin(), sum_delta_commu.end()) -
        sum_delta_commu.begin();
}

igraph_real_t random_select(igraph_vector_t *cand)
{
    igraph_integer_t nb_v = igraph_vector_size(cand);
    igraph_real_t selected = rand() % nb_v;
    while (igraph_vector_e(cand, selected) == false)
        selected = rand() % nb_v;
    return selected;
}

igraph_real_t select_commu_max_delta(
        std::vector<igraph_real_t> &delta,
        igraph_vector_t *cand,
        std::vector<std::vector<bool>> &comm_mask)
{
    igraph_real_t selected;
    igraph_integer_t nb_commu, nb_v, commu_selected;
    nb_v = delta.size();
    nb_commu = comm_mask.size();
    std::vector<igraph_real_t> tmp;
    tmp.resize(nb_v);
    commu_selected = commu_max_delta_sum(delta, comm_mask);
    mask(comm_mask[commu_selected], delta, tmp);
    selected = std::max_element(tmp.begin(), tmp.end()) - tmp.begin();

    return selected;
}

igraph_real_t select_next_root(
        std::vector<igraph_real_t> &delta,
        igraph_vector_t *cand,
        std::vector<std::vector<bool>> &comm_mask,
        igraph_real_t &count_bfs)
{
    igraph_real_t selected;
    auto rand_select_method = rand() / RAND_MAX;

    if (rand_select_method <= (1 / count_bfs) + 0.05)
        selected = random_select(cand);
    else
        selected = select_commu_max_delta(delta, cand, comm_mask);
    return selected;
}

int eccentricity(igraph_t *graph,
        igraph_integer_t root,
        igraph_vector_t *eccen,
        igraph_vector_t *dist,
        igraph_vector_t *cand)
{
    int err = 0;
    igraph_real_t cur_v, nb_neighbors;
    igraph_real_t ecc = 0;
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
        for (igraph_real_t i = 0; i < nb_neighbors; i++)
        {
            igraph_real_t neighbor_i = igraph_vector_e(&cur_neighbors, i);
            if (igraph_vector_e(dist, neighbor_i) == -1)
            {
                igraph_real_t dist_cur = igraph_vector_e(dist, cur_v);
                igraph_vector_set(dist, neighbor_i, dist_cur + 1);
                q.push(neighbor_i);
                ecc = std::max(ecc, dist_cur + 1);
            }
        }
    }
    igraph_vector_destroy(&cur_neighbors);
    VECTOR(*eccen)[root] = ecc;
    return 0;
}

int bounding_eccentricities(igraph_t *graph,
        igraph_vector_t *eccen,
        igraph_real_t &count_bfs)
{
    srand(time(NULL));

    igraph_real_t nb_v = igraph_vcount(graph);
    igraph_real_t root;
    igraph_integer_t nb_commu;
    igraph_vector_t ecc_l, ecc_u, cand, dist, membership;
    std::vector<std::vector<bool>> commu_mask;
    std::vector<igraph_real_t> delta;

    igraph_vector_init(eccen, nb_v);
    igraph_vector_init(&ecc_l, nb_v);
    igraph_vector_init(&ecc_u, nb_v);
    igraph_vector_init(&cand, nb_v);
    igraph_vector_init(&dist, nb_v);
    igraph_vector_init(&membership, nb_v);
    delta.resize(nb_v);

    igraph_vector_fill(&ecc_l, LONG_MIN);
    igraph_vector_fill(&ecc_u, LONG_MAX);
    igraph_vector_fill(&cand, 1);

    get_commu(graph, &membership);
    nb_commu = igraph_vector_max(&membership) + 1;
    community_mask_init(&membership, nb_commu, nb_v, commu_mask);

    while (igraph_vector_contains(&cand, true))
    {
        root = select_next_root(delta, &cand, commu_mask, count_bfs);

        eccentricity(graph, root, eccen, &dist, &cand);
        count_bfs += 1;
        for (igraph_integer_t w = 0; w < nb_v; w++)
        {
            if (igraph_vector_e(&cand, w) == false)
                continue; //we only consider candidate vertices
            igraph_real_t d_rw = igraph_vector_e(&dist, w);
            igraph_real_t eccr = igraph_vector_e(eccen, root);
            igraph_real_t loww = igraph_vector_e(&ecc_l, w);
            igraph_real_t uppw = igraph_vector_e(&ecc_u, w);
            VECTOR(ecc_l)[w] = std::max(loww, std::max(eccr - d_rw, d_rw));
            VECTOR(ecc_u)[w] = std::min(uppw, eccr + d_rw);
            delta[w] = igraph_vector_e(&ecc_u, w) - igraph_vector_e(&ecc_l, w);

            if (igraph_vector_e(&ecc_l, w) == igraph_vector_e(&ecc_u, w))
            {
                VECTOR(*eccen)[w] =igraph_vector_e(&ecc_l, w);
                VECTOR(cand)[w] = false;
            }
        }
    }
    igraph_vector_destroy(&ecc_l);
    igraph_vector_destroy(&ecc_u);
    igraph_vector_destroy(&cand);
    igraph_vector_destroy(&dist);
    igraph_vector_destroy(&membership);

    return 0;
}
