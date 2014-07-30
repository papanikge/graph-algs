/*
 * George Papanikolaou CEID 2014
 * Auxiliary functions such as LEDA->Boost conversion
 */

#include <iostream>
#include <cmath>
#include <LEDA/graph/graph.h>
#include "types.h"

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
        capacities[e] = rand() % MAXIMUM_CAP + 1;
    }
    return;
}

/*
 * Function to convert an existing LEDA graph to BGL
 */
void leda2boost(const leda::graph& LG, BoostGraph& BG, const leda::edge_array<int>& capacities)
{
    leda::edge e;
    leda::node n;
    // a leda node array of boost vertices. wicked.
    leda::node_array<BoostVertex> BVs(LG);

    // purge the old contents and start constructing the Boost mirror
    BG.clear();
    forall_nodes(n, LG)
        BVs[n] = boost::add_vertex(BG);

    // now attempting to add the edges between the vertices
    forall_edges(e, LG) {
        // we also add the corresponding capacity every time since Boost keeps them inside the graph
        boost::add_edge(BVs[LG.source(e)], BVs[LG.target(e)], capacities[e], BG).first;
    }
    return;
}