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

#define MAXIMUM_WEIGHT 10000

/*
 * Required typedefs to get some meaning out of Boost code
 */
struct EdgeProperties {
    int weight;
};
typedef boost::adjacency_list <boost::vecS,
                               boost::vecS,
                               boost::undirectedS,
                               int,  // for the nodes, and for the edges:
                               EdgeProperties> BoostGraph;
typedef boost::graph_traits<BoostGraph>::vertex_descriptor BoostVertex;
typedef boost::graph_traits<BoostGraph>::edge_descriptor   BoostEdge;
typedef boost::property_map<BoostGraph, int EdgeProperties::*>::type BWeightMap;


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
void leda2boost(const leda::graph& LG, BoostGraph& BG, const leda::edge_array<int>& weight, BWeightMap& Bweight)
{
    leda::edge e;
    leda::node n;
    BoostEdge BE;
    // a leda node array of boost vertices. wicked.
    leda::node_array<BoostVertex> BVs(LG);

    // purge the old contents and start constructing the Boost mirror
    BG.clear();
    forall_nodes(n, LG)
        BVs[n] = boost::add_vertex(BG);

    forall_edges(e, LG) {
        BE = boost::add_edge(BVs[LG.source(e)], BVs[LG.target(e)], BG).first;
        // we also need to "transfuse" the weights
        Bweight[BE] = weight[e];
    }
}

/*
 * Main function to run all the MST versions and benchmark their time
 */
static void benchmark_implementations(const leda::graph& G, const leda::edge_array<int>& weight)
{
    float T;

    T = leda::used_time();
    leda::MIN_SPANNING_TREE(G, weight);
    std::cout << "\t\tLEDA MST calculation time: " << leda::used_time(T) << std::endl;
}

int main(int argc, char **argv)
{
    int i, n;
    leda::graph G;
    leda::edge_array<int> weight;
    unsigned int N[] = { 10000, 40000, 70000 };

    if (argc > 2 && !strcmp(argv[1], "-n")) {
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
            // this is a hack, LEDA cannot produce an undirected grid
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
