//
// George 'papanikge' Papanikolaou
// CEID Advance Algorithm Design Course 2014
// Library of Efficient Data types and Algorithms (LEDA) testing
//

#include "basic.h"
#include <LEDA/system/timer.h>
#include <iostream>
#include <cmath>

/*
 * Internal auxiliary function to construct a clique around a node
 */
static void generate_clique(leda::graph& G, leda::node& v, int nodes_per_clique)
{
    int i;
    leda::node w, r;
    leda::list<leda::node> L;

    // Constructing clique
    for (i = 0; i < nodes_per_clique; i++) {
        w = G.new_node();
        // bidirectional
        G.new_edge(w,v);
        G.new_edge(v,w);
        // keeping it for later, so we can fill in the missing edges
        L.append(w);
    }

    // Inserting missing edges (to itself included)
    forall(r, L) {
        forall(w, L) {
            if (r == w)
                continue;
            G.new_edge(r,w);
        }
    }
}

/*
 * Constructing hard graphs with k connected cliques.
 * Of course they should have k SCC.
 *
 * @k : number of cliques
 * @n : number of nodes in the graph
 */
static void construct_cliqued_graph(leda::graph& G, int k, long n)
{
    int i;
    leda::node f, v;
    long nodes_per_clique = n/k;

    /* Constructing the initial nodes that will connect the cliques */
    f = G.new_node();
    generate_clique(G, f, nodes_per_clique);
    for (i = 1; i < k; i++) {
        v = G.new_node();
        G.new_edge(f,v);
        generate_clique(G, v, nodes_per_clique);
        f = v;
    }
}

/*
 * This function run the two implementations of the algorithm sequentially and checks their output.
 */
static void test_implementations(leda::graph& G, leda::node_array<int>& sccnum)
{
    float T;
    int mine, leda;

    T = leda::used_time();
    leda = leda::STRONG_COMPONENTS(G, sccnum);
    std::cout << "\t\tLEDA Strong-Components: " << leda << " -- Time to calculate: " << leda::used_time(T) << std::endl;
    sccnum.init(G, 0);
    mine = my_STRONG_COMPONENTS(G, sccnum);
    std::cout << "\t\tMy   Strong-Components: " << mine << " -- Time to calculate: " << leda::used_time(T) << std::endl;
}

int main()
{
    int i, j, e;
    bool ret;
    int N[] = { 20000, 40000, 90000 };
    int K[] = { 5, 10, 20 };
    leda::graph G, T;
    leda::node t, s;

    // Testing our implementation with the checker.
    std::cout << "!>>> Checking implementation... " << std::endl;
    leda::complete_graph(G, 100);
    s = G.choose_node();
    t = G.new_node();
    G.new_edge(s, t);
    leda::node_array<int> check_nums(G, 0);
    my_STRONG_COMPONENTS(G, check_nums);
    ret = STRONG_COMPONENTS_checker(G, check_nums);
    if (ret)
        std::cout << "\tCheck. Implementations match." << std::endl;
    else
        std::cout << "\tBEWARE: Implementations DON'T match." << std::endl;

    // Random graphs.
    std::cout << "!>>> Random graphs... " << std::endl;
    for (i = 0; i < 3; i++) {
        e = N[i] * log(N[i]);
        leda::random_graph(G, N[i], e);
        std::cout << "\tGraph generated. Nodes: " << N[i] << std::endl;
        leda::node_array<int> compnum(G, 0);

        // We got our graph, and node array. Lets' go...
        test_implementations(G, compnum);

        // clean up
        G.clear();
    }

    // Cliqued graphs.
    N[0] = 10000;
    std::cout << "!>>> Cliqued graphs... " << std::endl;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            construct_cliqued_graph(G, K[j], N[i]);
            std::cout << "\tGraph generated. Cliques: " << K[j] << " and " << N[i] << " nodes." << std::endl;
            leda::node_array<int> compnum(G, 0);

            test_implementations(G, compnum);

            // clean up
            G.clear();
        }
    }

    return 0;
}
