/*
 * George Papanikolaou CEID 2014
 * Kruskal Minimum spanning tree implementation
 * using lists for every node which is kind slow.
 */

#include <map>
#include <deque>
#include <boost/array.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include "types.h"

/*
 * Merge two lists by adding only at the end
 */
static inline void mergedeques(std::deque<BoostVertex> to, std::deque<BoostVertex> from)
{
    std::deque<BoostVertex>::iterator it;

    for (it = from.begin(); it != from.end(); ++it) {
        to.push_back(*it);
    }
}

/*
 * My implementation of Kruskal's Minimum Spanning Tree algorithm
 * with lists(actually deques) & sub-forests using course's directions
 */
void my_kruskal(BoostGraph& BG, std::vector<BoostEdge> spanning_tree)
{
    typedef boost::indirect_cmp<BoostWeightMap, std::greater<int> > weight_greater;

    int i = 0;
    unsigned int n = num_edges(BG);
    BoostEdge e;
    BoostVertex u, v;
    BoostEdgeIt eit_st, eit_end;
    BoostVertexIt vit_st, vit_end;
    // we are going to use a map for easy pointer maninpulation of the other
    // data structures. Every edge maps to something easily changeable that way
    std::map<BoostVertex, int> mapper;
    // for the comparison
    BoostWeightMap Bweights;
    weight_greater w(Bweights);
    // sorted queue of edge weights
    std::priority_queue<BoostEdge, std::vector<BoostEdge>, weight_greater> Queue(w);
    // static array of dynamic lists. Initially we need to have one list for
    // each edge but then we need the ability to add more to some cells. Weird.
    std::deque<BoostVertex> L[num_vertices(BG)];

    // first push all edges into Queue, so they would be sorted,
    for (boost::tie(eit_st, eit_end) = edges(BG); eit_st != eit_end; ++eit_st)
        Queue.push(*eit_st);

    // then initialize vertices to a counter in the dictionary,
    // then add it to the required list. We have |edges| number of lists.
    for (boost::tie(vit_st, vit_end) = vertices(BG); vit_st != vit_end; ++vit_st) {
        mapper[*vit_st] = i;
        L[i].push_back(*vit_st);
        i++;
    }

    // iterating over queue
    while (!Queue.empty() && spanning_tree.size() < (n - 1)) {
        e = Queue.top();
        Queue.pop();
        u = source(e, BG);
        v = target(e, BG);

        if (L[mapper[u]].front() != L[mapper[v]].front()) {
            // No circle occuring, we can add the edge to the MST
            spanning_tree.push_back(e);
            // merge lists for the next iteration
            mergedeques(L[mapper[u]], L[mapper[v]]);
            // and the maps of course
            mapper[v] = mapper[u];
        }
    }
    return;
}
