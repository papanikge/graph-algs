/*
 * George Papanikolaou CEID 2014
 * Shortest Augmenting Path Maximum Flow algorithm implementation
 * aka "Edmonds–Karp algorithm"
 */

#include <map>
#include "boost-types.h"
#include <boost/graph/breadth_first_search.hpp>

/*
 * subclassing the default visitor for our own needs
 */
class MyVisitor : public boost::default_dfs_visitor
{
public:
    void discover_vertex(BoostVertex v, const BoostGraph& g) const
    {
        // TODO something when discovering the node
        return;
    }
};

/*
 * this file's core function
 */
int shortest_aug_path(BoostGraph& BG, std::vector<BoostEdge> ret_flow)
{
    MyVisitor vis
    breadth_first_search(BG, boost::visitor(vis)); // add second argument the intial core if desired
}
