//
// George 'papanikge' Papanikolaou
// CEID Advance Algorithm Design Course 2014
// Benchmarking of Minimum Spanning Tree algorithms with Boost & LEDA
//

#include <iostream>
#include <vector>
#include <cstdlib>
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
#include "types.h"

void assign_random_weights(const leda::graph& G, leda::edge_array<int>& weight);
void connected_random_generator(leda::graph& G, const int number_of_nodes, leda::edge_array<int>& weight);
void leda2boost(const leda::graph& LG, BoostGraph& BG, const leda::edge_array<int>& weight);
void my_kruskal(BoostGraph& BG, std::vector<BoostEdge> spanning_tree);

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
    spanning_tree.clear();
    my_kruskal(BG, spanning_tree);
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
