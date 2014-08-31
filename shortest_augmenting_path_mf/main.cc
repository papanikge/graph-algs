/*
 * George 'papanikge' Papanikolaou
 * CEID Advance Algorithm Design Course 2014
 * Benchmarking of Shortest Augmenting Path MF Algorithm with Boost & LEDA
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_gen.h>
#include <LEDA/graph/max_flow.h>
#include <LEDA/system/timer.h>
#include "boost-types.h"

/* Prototypes */
void generate_random_capacities(const leda::graph& G, leda::edge_array<int>& capacities);
void leda2boost(const leda::graph& LG, BoostGraph& BG, const leda::edge_array<int>& capacities);
int  shortest_aug_path(BoostGraph& BG, std::vector<BoostEdge> ret_flow);

/*
 * Runs benchmarks for a given graph.
 * We are printing directly to cerr in this function to avoid multiple flush()es
 */
static void benchmark(const leda::graph& G, leda::edge_array<int>& capacities)
{
    float T;
    int leda_flow, my_flow;
    BoostGraph BG;
    leda::node s, t;
    leda::edge_array<int> flow(G);

    /* Pick random source/sink nodes. TODO: user option? */
    s = G.choose_node();
    do {
        t = G.choose_node();
    } while (s == t);

    /* Initial errands */
    std::cerr << "\t  Preparing graphs... ";
    leda2boost(G, BG, capacities);
    std::cerr << "Done." << std::endl;

    /* LEDA's internal implementation */
    T = leda::used_time();
    leda_flow = leda::MAX_FLOW(G, s, t, capacities, flow);
    std::cerr << "\t\tLEDA's Calculation time: " << leda::used_time(T);
    if (CHECK_MAX_FLOW(G, s, t, capacities, flow))
        std::cerr << "... [Correct]" << std::endl;
    else
        std::cerr << "... [Wrong]" << std::endl;

    /* My implementation */
    T = leda::used_time();
    my_flow = shortest_aug_path(BG, s, t);
    std::cerr << "\t\tCustom SAP Algorithm implementation time: " << leda::used_time(T) << std::endl;

    /* Testing & final errands */
    if (my_flow == leda_flow)
        std::cerr << "\t\t[Return values match!]" << std::endl;
    else
        std::cerr << "\t\t[Return values do NOT match!]" << std::endl;
    return;
}

int main(int argc, char **argv)
{
    int i, n;
    leda::graph G;
    leda::edge_array<int> capacities;
    unsigned int N[] = { 1000, 3000, 5000 };

    std::cout << "\n-=-=-=-=- Shortest Augmenting Path MF Algorithm Benchmarking -=-=-=-=-\n";
    if (argc > 2 && !strcmp(argv[1], "-n")) {
        /* Custom nodes from the command line */
        n = atoi(argv[2]);
        leda::random_simple_loopfree_graph(G, n, n*log10(n));
        generate_random_capacities(G, capacities);

        /* Safety check */
        if (G.is_undirected())
            std::cerr << "BUG! Graph is undirected!" << std::endl;

        benchmark(G, capacities);
    } else {
        /* Default values in the nodes */
        std::cout << "Give -n <number of nodes> if you want a custom amount\n";
        std::cout << "Moving on with the default values...\n\n";

        std::cout << ">>> Random graphs...\n";
        for (i = 0; i < 3; i++) {
            std::cout << "\tGenerating random graph with " << N[i] << " nodes... ";
            std::cout.flush();
            leda::random_simple_loopfree_graph(G, N[i], N[i]*log10(N[i]));
            generate_random_capacities(G, capacities);
            std::cout << "Done." << std::endl;

            benchmark(G, capacities);

            /* cleaning up for the next iteration */
            G.clear();
        }
    }
    return 0;
}
