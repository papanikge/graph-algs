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
 * Tracing the path back to the source and finding the minimux residue capacity
 */
static int min_res_on_path(BoostGraph& BG, VerticesSizeType *parent, int f)
{
    int cap;
    int delta = 1000;  /* something big so we can find smaller values */
    BoostVertex u, v;

    while (parent[f] != NULL) {
        /* getting the capacity */
        u = boost::vertex(f, BG);
        v = boost::vertex(parent[f], BG);
        cap = BG[boost::edge(u, v, BG).first];
        /* trying to find min */
        if (delta > cap)
            delta = cap;
        /* next... */
        f = parent[f];
    }
    return delta;
}

/*
 * this file's core function
 */
int shortest_aug_path(BoostGraph& BG, BoostVertex& source, BoostVertex& target)
{
    int i, j, s, t, delta;
    unsigned int n = boost::num_vertices(BG);
    unsigned int m = boost::num_edges(BG);
    BoostOutEdgeIt current, next;
    std::vector<BoostVertex> avail;
    /* We use an "index map" in order to easily assign IDs to vertices, and be able
     * to use regular arrays. Boost visitors use regular arrays and they are simpler
     * than using Propery Maps */
    IndexMap index = boost::get(boost::vertex_index, BG);
    /* We'll just use a regular array to store the distances for the sake of simplicity. */
    VerticesSizeType distances[n];
    std::fill_n(distances, n, 0);
    /* ...and a similar one for the parent nodes */
    VerticesSizeType parent[n];
    std::fill_n(parent, n, NULL); // TODO: maybe null is not allowed. in that case choose an unchosen id

    /* Getting the distance labels by reversed BFS. Creating the visitor inline. */
    boost::breadth_first_search(boost::make_reverse_graph(BG), boost::vertex(t, BG),
               boost::visitor(boost::make_bfs_visitor(boost::record_distances(&distances[0],
                                                                              boost::on_tree_edge())))); 

    /* getting the index, since we're working we them now, and starting main loop... */
    s = index[source];
    t = index[target];
    i = s;
    while (distances[s] < n) {
        /* Iterating through all out going edges of current node */
        for (boost::tie(current, next) = boost::out_edges(boost::vertex(i, BG), BG); current != next; ++current) {
            /* Selecting only admissible ones */
            if (distances[i] == distances[boost::target(*current)] + 1)
                avail.push_back(boost::target(*current, BG));
        }

        if (avail.size() != 0) {
            /* Get the requirements */
            j = index[avail[0]]; // TODO: (maybe) we need a way to go to the second
            /* ADVANCE operation */
            parent[j] = i;
            i = j;
            if (i == t) {
                /* We're there. AUGMENT operation.
                 * We need to find the minimum residual capacity and use it to augment */
                // TODO: we need to augment and edit the graph
                delta = min_res_on_path(BG, parent, i);
                /* Let's go again */
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
