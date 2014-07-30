//
// George 'papanikge' Papanikolaou
// CEID Advance Algorithm Design Course 2014
// Benchmarking of Maximum flow algorithms with Boost & LEDA
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <boost/array.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_gen.h>
#include <LEDA/graph/max_flow.h>
#include <LEDA/system/timer.h>

// #include "types.h"

/*
 * Main function to run all the benchmarks.
 */
static void benchmark(const leda::graph& G, leda::edge_array<int>& capacities)
{
    float T;
    int max_flow;
    leda::node s, t;
    leda::edge_array<int> flow(G);

    // pick random source/sink nodes
    s = G.choose_node();
    do {
        t = G.choose_node();
    } while (s == t);

    // LEDA's internal
    T = leda::used_time();
    max_flow = leda::MAX_FLOW(G, s, t, capacities, flow);
    std::cout << "\t\tLEDA Calculation time: " << leda::used_time(T) << std::endl;
    if (CHECK_MAX_FLOW(G, s, t, capacities, flow))
        std::cout << "\t\tLEDA's maximum flow was: " << max_flow << std::endl;
    else
        std::cout << "\t\tLEDA Maximum flow calculation was wrong!!!" << std::endl;

    /* My implementation TODO */

    return;
}

int main(int argc, char **argv)
{
    int i, n;
    leda::graph G;
    leda::edge_array<int> capacities;
    unsigned int N[] = { 1000, 3000, 5000 };

    std::cout << "\n-=-=-=-=- Maximum Flow Algorithm Benchmarking -=-=-=-=-\n";
    if (argc > 2 && !strcmp(argv[1], "-n")) {
        /* Custom nodes from the command line */
        n = atoi(argv[2]);
        leda::random_graph(G, n, n*log10(n));
        generate_random_capacities(G, capacities);
        benchmark(G, capacities);
    } else {
        /* Default values in the nodes */
        std::cout << "Give -n <number of nodes> if you want a custom amount\n";
        std::cout << "Moving on with the default values...\n\n";

        std::cout << ">>> Random graphs...\n";
        for (i = 0; i < 3; i++) {
            leda::random_graph(G, N[i], N[i]*log10(N[i]));
            generate_random_capacities(G, capacities);
            std::cout << "\tGraph generated.\n";
            benchmark(G, capacities);

            // clean up
            G.clear();
        }
    }
    return 0;
}
