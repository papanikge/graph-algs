/*
 * George Papanikolaou CEID 2014
 * Auxiliary functions such as leda->boost conversion
 */

#include <iostream>
#include <cmath>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_gen.h>
#include <LEDA/graph/min_span.h>
#include <LEDA/system/timer.h>
#include <boost/array.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
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
void leda2boost(const leda::graph& LG, BoostGraph& BG, const leda::edge_array<int>& weight)
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
        // we also add the coresponding weight every time
        boost::add_edge(BVs[LG.source(e)], BVs[LG.target(e)], weight[e], BG).first;
    }
    return;
}
