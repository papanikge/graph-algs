/*
 * George Papanikolaou CEID 2014
 * Auxiliary functions such as LEDA->Boost conversion
 */

#include <iostream>
#include <utility>
#include <cmath>
#include <LEDA/graph/graph.h>
#include "boost-types.h"

#define MAXIMUM_CAP 100

/*
 * Fill in the capacities vector of an already generated graph
 */
void generate_random_capacities(const leda::graph& G, leda::edge_array<int>& capacities)
{
    leda::edge e;

    srand(time(NULL));
    capacities.init(G, 0);
    forall_edges(e,G) {
        capacities[e] = rand() % (MAXIMUM_CAP + 1);
    }
    return;
}

/*
 * Function to convert an existing LEDA graph to Boost Graph format.
 * Returns pair with nodes in the new graph that mirror provided nodes in the provided graph.
 */
std::pair<BoostVertex, BoostVertex> leda2boost(const leda::graph& LG,
                                               BoostGraph& BG,
                                               const leda::edge_array<int>& capacities,
                                               leda::node s,
                                               leda::node t)
{
    leda::edge e;
    leda::node n;
    BoostVertex first, second;
    /* A LEDA node-array of Boost vertices. */
    leda::node_array<BoostVertex> BVs(LG);

    /* Purge the old contents and start constructing the Boost mirror. */
    BG.clear();
    forall_nodes(n, LG) {
        /* Constructing vertices first. We could add properties here. */
        BVs[n] = boost::add_vertex(BG);
        /* keeping those vertices for later */
        if (n == s)
            first = BVs[n];
        if (n == t)
            second = BVs[n];
    }

    /* Now attempting to add the edges between the vertices. */
    forall_edges(e, LG) {
        /* We also add the corresponding capacity every time since Boost keeps them inside the graph. */
        boost::add_edge(BVs[LG.source(e)], BVs[LG.target(e)], capacities[e], BG).first;
    }

    return std::make_pair(first, second);
}
