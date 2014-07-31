/*
 * George 'papanikge' Papanikolaou
 * Required typedefs to get some meaning out of Boost code
 */

#ifndef BOOST_TYPES
#define BOOST_TYPES

#include <boost/array.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

// main graph type
typedef boost::adjacency_list <boost::vecS,
                               boost::vecS,
                               boost::directedS,
                               // maybe? --> boost::no_property
                               int,  // this one for the nodes, and the next for the edges:
                               boost::property<boost::edge_weight_t, int> > BoostGraph;
// for the vertex-edge types
typedef boost::graph_traits<BoostGraph>::vertex_descriptor BoostVertex;
typedef boost::graph_traits<BoostGraph>::edge_descriptor   BoostEdge;
// for the iterators
typedef boost::graph_traits<BoostGraph>::vertex_iterator   BoostVertexIt;
typedef boost::graph_traits<BoostGraph>::edge_iterator     BoostEdgeIt;
// for the edge-property-map (aka edge-array)
typedef boost::property_map<BoostGraph, boost::edge_weight_t>::type BoostWeightMap;

#endif
