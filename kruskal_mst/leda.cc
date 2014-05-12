//
// George 'papanikge' Papanikolaou
// CEID Advance Algorithm Design Course 2014
// Benchmarking of Minimum Spanning Tree algorithms with Boost & LEDA
//

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_gen.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/graph/basic_graph_alg.h>
#include <LEDA/graph/min_span.h>
#include <LEDA/system/timer.h>

/*
 * Wrapper around random graph generation. Calculates number-of-nodes to be
 * the theoretical maximum so the graph would be fully connected (aka complete).
 * TODO: eventually we need to be able to generate boost *and* LEDA graphs,
 * so we may need to make this a class of some sort??
 */
void connected_random_generator(leda::graph& G, int number_of_nodes)
{
    int number_of_edges = 2*number_of_nodes*log(number_of_nodes);

    leda::random_simple_undirected_graph(G, number_of_nodes, number_of_edges);
    std::cout << "Graph generated. Nodes: " << number_of_nodes << std::endl;

    ///// TODO: WHERE ???>>
    // leda::node_array<int> compnum(G, 0);
    return;
}

/*
 * Main function to run all the MST versions and benchmark their time
 */
void benchmark_implementations(leda::graph& G)
{
    float T;
    leda::node_array<int> sccnum; // TODO: DEFINITELY useless. we need to init it somewhere else and with diff stuff

    sccnum.init(G, 0);
    T = leda::used_time();
    leda::MIN_SPANNING_TREE(G, *sccnum);
    std::cout << "\t\tLEDA MST calculation time: " << leda::used_time(T) << std::endl;
}

int main(int argc, char **argv)
{
    int i, n;
    leda::graph G;
    int N[] = { 10000, 40000, 70000 };

    if (argc > 2 && !strcmp(argv[1], "-n")) {
        n = atoi(argv[2]);
        connected_random_generator(G, n);
        benchmark_implementations(G);
    } else {
        std::cout << ">>> Minimum Spanning Tree Benchmarking <<<\n";
        std::cout << "Give -n <number of nodes> if you want custom nodes\n";
        std::cout << "Moving on with the default number of nodes...\n\n";
        for (i = 0; i < 3; i++) {
            std::cout << ">>> Random graphs...";
            connected_random_generator(G, N[i]);
            benchmark_implementations(G);

            // clean up
            G.clear();
        }
        N[0] = 100;
        N[1] = 200;
        N[2] = 300;
        for (i = 0; i < 3; i++) {
            std::cout << ">>> Grid graphs...";
            leda::grid_graph(G, N[i]);
            // this is a hack, LEDA cannot produce an undirected grid
            // graph, so we make them bidirectional.
            G.make_bidirected();
            benchmark_implementations(G);

            // clean up
            G.clear();
        }
    }

    return 0;
}
