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
                               // vertex properties:
                               boost::no_property,
                               // edge properties:
                               boost::property<boost::edge_weight_t, int> > BoostGraph;
// for the vertex-edge types
typedef boost::graph_traits<BoostGraph>::vertex_descriptor BoostVertex;
typedef boost::graph_traits<BoostGraph>::edge_descriptor   BoostEdge;
// for the iterators
typedef boost::graph_traits<BoostGraph>::vertex_iterator   BoostVertexIt;
typedef boost::graph_traits<BoostGraph>::out_edge_iterator BoostOutEdgeIt;
// for easy accessing of vertices
typedef boost::property_map<BoostGraph, boost::vertex_index_t>::type IndexMap;
// for creating arrays with information
typedef boost::graph_traits<BoostGraph>::vertices_size_type VerticesSizeType;

#endif
