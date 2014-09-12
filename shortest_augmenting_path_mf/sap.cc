/*
 * George Papanikolaou CEID 2014
 * Shortest Augmenting Path Maximum Flow algorithm implementation
 * aka "Edmonds–Karp algorithm"
 */

#include <cstdlib>
#include <vector>
#include <utility>
#include <algorithm>
#include "boost-types.h"
#include <boost/graph/visitors.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/graph_utility.hpp>

/*
 * Find minimum value from the target of outgoing edges of a vertex in a graph. Yeah.
 */
static inline int find_min_out_edges(const BoostVertex& initial,
                                     VerticesSizeType *cost,
                                     const BoostGraph& BG)
{
    int value;
    int min = 30000; // Something big so we it will be assigned initialy
    BoostOutEdgeIt one, two;

    for (boost::tie(one, two) = boost::out_edges(initial, BG); one != two; ++one) {
        value = cost[boost::target(*one, BG)];
        if (value < min)
            min = value;
    }
    return min;
}

/*
 * Tracing the path back to the source, finding min residual capacity and augmenting it.
 */
static int augment_path(BoostGraph& BG, BoostVertex& f, std::vector<BoostVertex>& parent)
{
    int cap;
    int delta = MAXIMUM_CAP + 1; // Something big so we can find smaller values
    BoostEdge e;
    edge_attr to_add;
    std::vector<BoostEdge> path;

    /* Finding the smaller capacity in the path */
    while (parent[f] != BoostGraph::null_vertex()) {
        /* finding the next edge, saving it and getting the capacity */
        e = boost::edge(parent[f], f, BG).first;
        path.push_back(e);
        cap = BG[e].cap;
        /* trying to find min */
        if (delta > cap)
            delta = cap;
        /* next... */
        f = parent[f];
    }

    if (delta == (MAXIMUM_CAP + 1))
        std::cerr << "ERROR: There should have been an aug path there." << std::endl;

    /* Iterating over the path to saturate the edges (using delta).
     * This is where the actual graph gets altered. */
    for (std::vector<BoostEdge>::iterator it = path.begin(); it != path.end(); ++it) {
        /* reduce the existing one */
        BG[*it].cap -= delta;
        if (BG[*it].cap <= 0)
            boost::remove_edge(*it, BG);
        /* create an opposite one */
        to_add.cap = delta;
        boost::add_edge(boost::target(*it, BG), boost::source(*it, BG), to_add, BG);
    }

    return delta;
}

/*
 * this file's core function
 */
long edmonds_karp_flow(BoostGraph& BG, const BoostVertex& source, const BoostVertex& target)
{
    long flow = 0;
    unsigned int n = boost::num_vertices(BG);
    BoostVertex i, j;
    BoostOutEdgeIt current, next;
    std::vector<BoostVertex> avail;
    /* We'll just use an vertex-index array to store the distances for the sake of simplicity. */
    VerticesSizeType distances[n];
    std::fill_n(distances, n, 0);
    /* ...and a regular vector for the parents */
    std::vector<BoostVertex> parent(n);
    parent[source] = BoostGraph::null_vertex(); // so we can find it later

    /* Getting the distance labels by reversed BFS. Creating the visitor inline. */
    boost::breadth_first_search(boost::make_reverse_graph(BG), target,
               boost::visitor(boost::make_bfs_visitor(boost::record_distances(&distances[0],
                                                                              boost::on_tree_edge()))));

    /* Starting main loop... */
    i = source;
    while (distances[source] < n) {
        avail.clear();
        /* Iterating through all out going edges of current node */
        for (boost::tie(current, next) = boost::out_edges(i, BG); current != next; ++current) {
            /* Selecting only admissible ones */
            if (distances[i] == distances[boost::target(*current, BG)] + 1)
                avail.push_back(boost::target(*current, BG));
        }

        if (avail.size() != 0) {
            /* Get the requirements */
            j = avail[0];
            /* ADVANCE operation. First keeping parent pointer */
            parent[j] = i;
            i = j;
            if (i == target) {
                /* We're there. AUGMENT operation. */
                flow += augment_path(BG, i, parent);
                /* Let's go from the top again */
                i = source;
            }
        } else {
            /* RETREAT operation. Relabel first. */
            distances[i] = 1 + find_min_out_edges(i, distances, BG);
            /* ...and then backtrack. */
            if (i != source)
                i = parent[i];
        }
    }

    return flow;
}
