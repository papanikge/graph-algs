//
// George 'papanikge' Papanikolaou
// CEID Advance Algorithm Design Course 2014
// Benchmarking of Minimum Spanning Tree algorithms with Boost & LEDA
//

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_gen.h>
#include <LEDA/graph/min_span.h>
#include <LEDA/system/timer.h>
#include <boost/array.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

#define MAXIMUM_WEIGHT 10000

/*
 * Required typedefs to get some meaning out of Boost code
 */
typedef boost::adjacency_list <boost::vecS,
                               boost::vecS,
                               boost::undirectedS,
                               int,  // for the nodes, and for the edges:
                               boost::property<boost::edge_weight_t, int> > BoostGraph;
// for the vertex-edge types
typedef boost::graph_traits<BoostGraph>::vertex_descriptor BoostVertex;
typedef boost::graph_traits<BoostGraph>::edge_descriptor   BoostEdge;
// for the iterators
typedef boost::graph_traits<BoostGraph>::edge_iterator     BoostEdgeIt;
// for the edge-property-map (aka edge-array)
typedef boost::property_map<BoostGraph, boost::edge_weight_t>::type BoostWeightMap;


/*
 * Auxiliary function to assign random weights to an edge array
 */
static void assign_random_weights(const leda::graph& G, leda::edge_array<int>& weight)
{
    leda::edge e;

    srand(time(NULL));
    weight.init(G, 0);
    forall_edges(e, G) {
        weight[e] = rand() % MAXIMUM_WEIGHT + 1;
    }
    return;
}

/*
 * Wrapper around undirected random graph generation with random edge weights.
 * Calculates number-of-nodes to be the theoretical maximum so the graph would
 * be fully connected (aka complete).
 */
void connected_random_generator(leda::graph& G, const int number_of_nodes, leda::edge_array<int>& weight)
{
    int number_of_edges = 2 * number_of_nodes * log10(number_of_nodes);

    leda::random_simple_undirected_graph(G, number_of_nodes, number_of_edges);
    assign_random_weights(G, weight);

    std::cout << "Graph generated. Nodes: " << number_of_nodes << std::endl;
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

/*
 * My implementation of Kruskal's Minimum Spanning Tree algorithm
 * TODO: split to another file
 */
void my_kruskal(BoostGraph& BG)
{
    BoostEdge e;
    BoostEdgeIt e_it_st, e_it_end;
    // for the comparison
    typedef boost::indirect_cmp<BoostWeightMap, std::greater<int> > weight_greater;
    BoostWeightMap Bweights;
    weight_greater wl(Bweights);
    // sorted queue of edge weights
    std::priority_queue<BoostEdge, std::vector<BoostEdge>, weight_greater> Queue(wl);
    // property map for easy accessing of the data
    BoostWeightMap BB = get(boost::edge_weight_t(), BG);

    /* first push all edges into Queue, so they would be sorted */
    for (boost::tie(e_it_st, e_it_end) = edges(BG);
         e_it_st != e_it_end;
         ++e_it_st)
        Queue.push(*e_it_st);

    /* iterating over queue */
    while (!Queue.empty()) {
        e = Queue.top();
        Queue.pop();
        std::cout << "Weight: " << BB[e] << std::endl;
    }
    return;
}

/*
 * Main function to run all the MST versions and benchmark their time.
 * It's responsible for the LEDA2Boost transformation and for the benchmarking.
 */
static void benchmark_implementations(const leda::graph& G, const leda::edge_array<int>& weight)
{
    float T;
    BoostGraph BG;
    std::vector<BoostEdge> spanning_tree;

    // LEDA
    T = leda::used_time();
    leda::MIN_SPANNING_TREE(G, weight);
    std::cout << "\t\tLEDA MST calculation time: " << leda::used_time(T) << std::endl;

    // Transform to Boost
    std::cout << "\tTransforming LEDA graph... ";
    leda2boost(G, BG, weight);
    std::cout << "Done." << std::endl;

    // Boost
    leda::used_time(T);
    kruskal_minimum_spanning_tree(BG, std::back_inserter(spanning_tree));
    std::cout << "\t\tBoost MST calculation time: " << leda::used_time(T) << std::endl;

    // My Boost implementation benchmarking
    my_kruskal(BG);
    std::cout << "\t\tMy-Boost MST calculation time: " << leda::used_time(T) << std::endl;
    return;
}

int main(int argc, char **argv)
{
    int i, n;
    leda::graph G;
    leda::edge_array<int> weight;
    unsigned int N[] = { 10000, 40000, 70000 };

    if (argc > 2 && !strcmp(argv[1], "-n")) {
        // for custom nodes
        n = atoi(argv[2]);
        connected_random_generator(G, n, weight);
        benchmark_implementations(G, weight);
    } else {
        std::cout << "\n-=-=-=-=- Minimum Spanning Tree Benchmarking -=-=-=-=-\n";
        std::cout << "Give -n <number of nodes> if you want custom nodes\n";
        std::cout << "Moving on with the default number of nodes...\n\n";

        std::cout << ">>> Random graphs..." << std::endl;
        for (i = 0; i < 3; i++) {
            connected_random_generator(G, N[i], weight);
            benchmark_implementations(G, weight);

            // clean up
            G.clear();
        }
        std::cout << ">>> Grid graphs..." << std::endl;
        N[0] = 100;
        N[1] = 200;
        N[2] = 300;
        for (i = 0; i < 3; i++) {
            leda::grid_graph(G, N[i]);
            // nasty hack, bacause LEDA cannot produce an undirected grid
            // graph, so we make them bidirectional.
            G.make_bidirected();
            assign_random_weights(G, weight);
            std::cout << "Graph generated. " << N[i] << "x" << N[i] << " nodes" << std::endl;
            benchmark_implementations(G, weight);

            // clean up
            G.clear();
        }
    }

    return 0;
}
