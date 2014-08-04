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
 * this file's core function
 */
int shortest_aug_path(BoostGraph& BG, std::vector<BoostEdge> ret_flow)
{
    int i, s, t;     /* pivot, source & sink ('t' from target) */
    unsigned int n = boost::num_vertices(BG);
    unsigned int m = boost::num_edges(BG);
    /* We'll just use a regular array to store the distances for the sake of simplicity. */
    boost::graph_traits<Graph>::vertices_size_type distances[n];
    std::fill_n(distances, n, 0);

    /* Finding target and source... randomly */
    srand(time(NULL));
    s = rand() % (n + 1);
    t = rand() % (n + 1);

    /* Getting the distance labels by reversed breadth first search.
     * Creating the visitor inline. */
    boost::breadth_first_search(boost::make_reverse_graph(BG), boost::vertex(t, BG),
               boost::visitor(boost::make_bfs_visitor(boost::record_distances(&distances[0], boost::on_tree_edge())))); 

    i = s;
    while (distances[s] < n) {
        if (/* condition: contains an admissible path */) {
            /* code */
        }
    }
}
