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

/*
 * Find minimum value from the target of outgoing edges of a vertex in a graph. Yeah.
 */
static inline int find_min_out_edges(const BoostVertex& initial,
                                     VerticesSizeType *cost,
                                     const BoostGraph& BG)
{
    int value, min;
    BoostOutEdgeIt one, two;
    bool first_iteration = true;

    for (boost::tie(one, two) = boost::out_edges(initial, BG); one != two; ++one) {
        value = cost[boost::target(*one, BG)];
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
 * Tracing the path back to the source, finding min residual capacity and augmenting it.
 */
static int augment_path(BoostGraph& BG, int f)
{
    int cap;
    int delta = 1000;  /* something big so we can find smaller values */
    BoostEdge e;
    BoostVertex u, v;
    edge_attr to_add;
    std::vector<BoostEdge> path;

    /* Finding the smaller capacity in the path */
    while (f.parent != -1) {
        /* finding the next edge, saving it and getting the capacity */
        u = boost::vertex(f, BG);
        v = boost::vertex(f.parent, BG);
        e = boost::edge(u, v, BG).first;
        path.push_back(e);
        cap = BG[e].cap;
        /* trying to find min */
        if (delta > cap)
            delta = cap;
        /* next... */
        f = f.parent;
    }

    if (delta == 1000)
        std::cerr << "ERROR: There should have been a aug path there." << std::endl;

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
int shortest_aug_path(BoostGraph& BG, const BoostVertex& source, const BoostVertex& target)
{
    int flow = 0;
    unsigned int n = boost::num_vertices(BG);
    BoostVertex i, j;
    BoostOutEdgeIt current, next;
    std::vector<BoostVertex> avail;
    /* We'll just use an vertex-index array to store the distances for the sake of simplicity. */
    VerticesSizeType distances[n];
    std::fill_n(distances, n, 0);

    /* Getting the distance labels by reversed BFS. Creating the visitor inline. */
    boost::breadth_first_search(boost::make_reverse_graph(BG), boost::vertex(target, BG),
               boost::visitor(boost::make_bfs_visitor(boost::record_distances(&distances[0],
                                                                              boost::on_tree_edge()))));

    /* Starting main loop... */
    i = source;
    while (distances[source] < n) {
        /* Iterating through all out going edges of current node */
        for (boost::tie(current, next) = boost::out_edges(boost::vertex(i, BG), BG); current != next; ++current) {
            /* Selecting only admissible ones */
            if (distances[i] == distances[boost::target(*current, BG)] + 1)
                avail.push_back(boost::target(*current, BG));
        }

        if (avail.size() != 0) {
            /* Get the requirements */
            j = avail[0];
            /* ADVANCE operation */
            j.parent = i;
            i = j;
            if (i == target) {
                /* We're there. AUGMENT operation. */
                flow += augment_path(BG, i);
                /* Let's go from the top again */
                i = source;
            }
        } else {
            /* RETREAT operation. Relabel first. */
            distances[i] = 1 + find_min_out_edges(boost::vertex(i, BG), distances, BG);
            /* ...and then backtrack. */
            if (i != source)
                i = i.parent;
        }
    }

    return flow;
}
