/*
 * George Papanikolaou CEID 2014
 * Shortest Augmenting Path Maximum Flow algorithm implementation
 * aka "Edmonds–Karp algorithm"
 */

#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>
#include <algorithm>
#include "boost-types.h"
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map.hpp>

/*
 * Find minimum value from the target of outgoing edges of a vertex in a graph. Yeah.
 */
static inline int find_min_out_edges(const BoostVertex& initial,
                                     const IndexMap *index,
                                     const VerticesSizeType *cost,
                                     const BoostGraph& G)
{
    int value, min;
    BoostOutEdgeIt one, two;
    bool first_iteration = true;

    for (boost::tie(one, two) = boost::out_edges(initial, G); one != two; ++one) {
        value = cost[index[boost::target(*one)]];
        if (first_iteration) {
            min = value;
            first_iteration = false;
        } else {
            if (value < min)
                min = value;
        }
    }
    return min;
}

/*
 * this file's core function
 */
int shortest_aug_path(BoostGraph& BG, std::vector<BoostEdge> ret_flow)
{
    int i, j, s, t;     /* pivots, source & sink ('t' from target) */
    unsigned int   n = boost::num_vertices(BG);
    unsigned int   m = boost::num_edges(BG);
    BoostVertex    src, trg;
    BoostOutEdgeIt one, two;
    /* Vertices index map */
    IndexMap index = boost::get(boost::vertex_index, BG);
    /* We'll just use a regular array to store the distances for the sake of simplicity. */
    VerticesSizeType distances[n];
    std::fill_n(distances, n, 0);
    /* ...and a similar one for the parent nodes */
    VerticesSizeType parent[n];

    /* Finding target and source... randomly */
    srand(time(NULL));
    s = rand() % (n + 1);
    t = rand() % (n + 1);

    /* Getting the distance labels by reversed BFS. Creating the visitor inline. */
    boost::breadth_first_search(boost::make_reverse_graph(BG), boost::vertex(t, BG),
               boost::visitor(boost::make_bfs_visitor(boost::record_distances(&distances[0],
                                                                              boost::on_tree_edge())))); 

    // TODO: somewhere we should construct the augmented tree into "ret_flow"
    i = s;
    while (distances[s] < n) {
        /* trying to get an outgoing edge */
        boost::tie(one, two) = boost::out_edges(boost::vertex(i, BG), BG);
        if (one) {
            /* ADVANCE operation */
            src = boost::source(*one, BG);
            trg = boost::target(*one, BG);

            /* Safety: Source of the edge should be i. */
            if (index[src] != i) {
                std::cerr << "BUG!!" << std::endl;
                exit(EXIT_FAILURE);
            }

            j = index[trg];
            parent[j] = i;
            i = j;
            if (i == t) {
                /* We're there. AUGMENT operation. */
                // TODO
                i = s;
            }
        } else {
            /* RETREAT operation. Relabel first. */
            distances[i] = 1 + find_min_out_edges(boost::vertex(i, BG), index, distances, BG);
            /* ...and then backtrack. */
            if (i != s)
                i = parent[i];
        }
    }
}
