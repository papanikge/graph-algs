/*
 * George Papanikolaou CEID 2014
 * Auxiliary functions such as LEDA->Boost conversion
 */

#include <iostream>
#include <utility>
#include <ctime>
#include <LEDA/graph/graph.h>
#include "boost-types.h"

/*
 * Fill in the capacities vector of an already generated graph.
 */
void generate_random_capacities(const leda::graph& G, leda::edge_array<int>& capacities)
{
    leda::edge e;

    srand(time(NULL));
    capacities.init(G, 0);
    forall_edges(e, G)
        capacities[e] = rand() % (MAXIMUM_CAP + 1);
    return;
}

/*
 * Remove edges that go directly from source to target, to assist the implementation
 * against some edge cases and to make the graphs more difficult.
 */
void delete_direct_vertices(leda::graph& G, const leda::node source, const leda::node target)
{
    leda::edge e;

    forall_edges(e, G)
        if ((G.source(e) == source) && (G.target(e) == target))
            G.del_edge(e);
    return;
}

/*
 * Initialize a given graph to the most hard case for shortest augmenting path algorithms.
 * The resulting graph is going to be hard with the returned set of source/target nodes.
 */
std::pair<leda::node, leda::node> generate_hard_graph(leda::graph& G, unsigned int how_many, leda::edge_array<int>& capacities)
{
    unsigned int i, k, p;
    leda::edge e;
    leda::node source, target, w, v, r;
    leda::list<leda::node> LL, LR;
    leda::list<leda::edge> L1, LM, LK;

    /* Initial calculations. */
    k = (how_many + 2)/6;
    p = (how_many - 4)/6;

    /* Source and Target. */
    source = G.new_node();
    target = G.new_node();

    /* The first two sets are connected with each other. And each of them with
     * either the source or the target. We keep the nodes in lists to fill the
     * connections after creating the nodes, and the edges in lists to assign
     * the required capacities later.
     * @LL: List (nodes) left set (connected with source)
     * @LR: List (nodes) right set (connected with target)
     * @LK: List (edges) with k capacity
     * @L1: List (edges) with unit capacity */
    for (i = 0; i < k; i++) {
        w = G.new_node();
        e = G.new_edge(source, w);
        LL.append(w);
        LK.append(e);
    }
    for (i = 0; i < k; i++) {
        w = G.new_node();
        e = G.new_edge(w, target);
        LR.append(w);
        LK.append(e);
    }
    /* (inter-)connect them */
    forall (w, LL) {
        forall (v, LR) {
            e = G.new_edge(w, v);
            L1.append(e);
        }
    }

    /* Two more sets whose nodes are connected in sequence and they are
     * connected with the previous sets every 2 nodes.
     * @LM: List (edges) with maximum capacity */
    w = G.new_node();
    e = G.new_edge(source, w);
    LM.append(e);
    for (i = 0; i < ((2*p)-1); i++) {
        v = G.new_node();
        e = G.new_edge(w, v);
        LM.append(e);
        if (i % 4 == 0) {
            forall (r, LR) {
                e = G.new_edge(v, r);
                LK.append(e);
            }
        } else if (i % 2 == 0) {
            forall (r, LL) {
                e = G.new_edge(v, r);
                LK.append(e);
            }
        }
        w = v;
    }
    w = G.new_node();
    for (i = 0; i < ((2*p)-1); i++) {
        v = G.new_node();
        e = G.new_edge(w, v);
        LM.append(e);
        if (i % 4 == 0) {
            forall (r, LR) {
                e = G.new_edge(r, v);
                LK.append(e);
            }
        } else if (i % 2 == 0) {
            forall (r, LL) {
                e = G.new_edge(r, v);
                LK.append(e);
            }
        }
        w = v;
    }
    e = G.new_edge(w, target);
    LM.append(e);

    /* Done. Add the capacities. */
    capacities.init(G, 0);
    forall (e, L1)
        capacities[e] = 1;
    forall (e, LK)
        capacities[e] = k;
    forall (e, LM)
        capacities[e] = MAXIMUM_CAP;

    return std::make_pair(source, target);
}

/*
 * Function to convert an existing LEDA graph to Boost Graph format.
 * Returns pair with nodes in the new graph that mirror provided nodes in the provided graph.
 */
#pragma GCC diagnostic ignored "-Wuninitialized"
std::pair<BoostVertex, BoostVertex> leda2boost(const leda::graph& LG,
                                               BoostGraph& BG,
                                               const leda::edge_array<int>& capacities,
                                               const leda::node s,
                                               const leda::node t)
{
    leda::edge e;
    leda::node n;
    BoostVertex first, second;
    edge_attr cap_to_add;
    /* A LEDA node-array of Boost vertices. */
    leda::node_array<BoostVertex> BVs(LG);

    /* Purge the old contents and start constructing the Boost mirror. */
    BG.clear();
    forall_nodes(n, LG) {
        /* Constructing vertices first. We could add properties here. */
        BVs[n] = boost::add_vertex(BG);
        /* keeping those vertices for later */
        if (n == s)
            first = BVs[n];
        if (n == t)
            second = BVs[n];
    }

    /* Now attempting to add the edges between the vertices. */
    forall_edges(e, LG) {
        /* We also add the corresponding capacity every time since Boost keeps them inside the graph. */
        cap_to_add.cap = capacities[e];
        boost::add_edge(BVs[LG.source(e)], BVs[LG.target(e)], cap_to_add, BG);
    }

    return std::make_pair(first, second);
}
