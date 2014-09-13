/*
 * George 'papanikge' Papanikolaou
 * CEID Advance Algorithm Design Course 2014
 * Benchmarking of Shortest Augmenting Path MF Algorithm with Boost & LEDA
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_gen.h>
#include <LEDA/graph/max_flow.h>
#include <LEDA/system/timer.h>
#include "boost-types.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"

/* Global self-explanatory debug variable */
static bool debug = false;

/* Prototypes */
std::pair<leda::node, leda::node> generate_hard_graph(leda::graph& G, unsigned int how_many, leda::edge_array<int>& capacities);
void generate_random_capacities(const leda::graph& G, leda::edge_array<int>& capacities);
void delete_direct_vertices(leda::graph& G, const leda::node source, const leda::node target);
std::pair<BoostVertex, BoostVertex> leda2boost(const leda::graph& LG, BoostGraph& BG, const leda::edge_array<int>& capacities, leda::node s, leda::node t);
long edmonds_karp_flow(BoostGraph& BG, const BoostVertex& source, const BoostVertex& target);

/*
 * Runs benchmarks for a given graph with the given source and target of course.
 * Is responsible for converting the graph to BGL.
 * We are printing directly to cerr in this function to avoid flush() pollution.
 */
static void benchmark(leda::graph& G, leda::edge_array<int>& capacities, leda::node s, leda::node t)
{
    float T;
    long leda_flow, my_flow;
    BoostGraph BG;
    leda::edge_array<int> flow(G);
    std::pair <BoostVertex, BoostVertex> Bst;

    /* Initial obligations */
    std::cerr << "  Preparing graphs... ";
    delete_direct_vertices(G, s, t);
    Bst = leda2boost(G, BG, capacities, s, t);
    std::cerr << "Done." << std::endl;

    /* LEDA's internal implementation */
    T = leda::used_time();
    leda_flow = leda::MAX_FLOW(G, s, t, capacities, flow);
    std::cerr << "\tLEDA's Calculation time: " << leda::used_time(T);
    if (debug) {
        if (CHECK_MAX_FLOW(G, s, t, capacities, flow))
            std::cerr << KGRN << "... [Correct]" << KNRM << std::endl;
        else
            std::cerr << KRED << "... [Wrong]" << KNRM << std::endl;
    } else {
        std::cerr << std::endl;
    }

    /* My implementation */
    T = leda::used_time();
    my_flow = edmonds_karp_flow(BG, Bst.first, Bst.second);
    std::cerr << "\tCustom SAP Algorithm implementation time: " << leda::used_time(T) << std::endl;

    /* Testing & final errands */
    if (debug) {
        if (my_flow == leda_flow)
            std::cerr << KGRN << "\t[Return values match!]" << KNRM << std::endl;
        else
            std::cerr << KRED << "\t[Return values do NOT match!]" << KNRM << std::endl;
    }
    return;
}

/*
 * Finds source and target randomly and calls benchmark()
 */
static void find_nodes_and_benchmark(leda::graph& G, leda::edge_array<int>& capacities)
{
    leda::node s, t;

    /* Pick random source/sink nodes. */
    s = G.choose_node();
    do {
        t = G.choose_node();
    } while (s == t);

    return benchmark(G, capacities, s, t);
}

int main(int argc, char **argv)
{
    int i, n, m;
    leda::graph G;
    leda::edge_array<int> capacities;
    std::pair<leda::node, leda::node> p;
    unsigned int N[] = { 8000, 15000, 30000 };

    std::cout << "\n-=-=-=-=- Shortest Augmenting Path MF Algorithm Benchmarking -=-=-=-=-\n";
    if (argc == 5 && !strcmp(argv[1], "-n") && !strcmp(argv[3], "-m")) {
        /* Custom nodes and edges from the command line */
        n = atoi(argv[2]);
        m = atoi(argv[4]);
        debug = true;
        leda::random_simple_loopfree_graph(G, n, m);
        generate_random_capacities(G, capacities);

        /* Safety check */
        if (G.is_undirected())
            std::cerr << "BUG! Graph is undirected!" << std::endl;

        find_nodes_and_benchmark(G, capacities);
    } else {
        /* Default values in the nodes */
        std::cout << "Give -n <number of nodes> -m <number of edges> if you want a custom amount.\n";
        std::cout << "Moving on with the default values...\n\n";

        std::cout << KYEL << ">>> Random graphs..." << KNRM << std::endl;
        for (i = 0; i < 3; i++) {
            std::cout << "Generating random graph with " << N[i] << " nodes... ";
            std::cout.flush();
            leda::random_simple_loopfree_graph(G, N[i], N[i]*2);
            generate_random_capacities(G, capacities);
            std::cout << "Done." << std::endl;

            find_nodes_and_benchmark(G, capacities);

            /* cleaning up for the next iteration */
            G.clear();
        }
        N[0] = 200;
        N[1] = 400;
        N[2] = 600;
        std::cout << KYEL << ">>> Hard graphs..." << KNRM << std::endl;
        for (i = 0; i < 3; i++) {
            std::cout << "Generating graph with " << N[i] << " nodes... ";
            std::cout.flush();
            p = generate_hard_graph(G, N[i], capacities);
            std::cout << "Done." << std::endl;

            benchmark(G, capacities, p.first, p.second);

            /* cleaning up for the next iteration */
            G.clear();
        }
    }
    return 0;
}
