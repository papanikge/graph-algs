/*
 * Required typedefs to get some meaning out of Boost code
 */

// main graph type
typedef boost::adjacency_list <boost::vecS,
                               boost::vecS,
                               boost::undirectedS,
                               int,  // for the nodes, and for the edges:
                               boost::property<boost::edge_weight_t, int> > BoostGraph;
// for the vertex-edge types
typedef boost::graph_traits<BoostGraph>::vertex_descriptor BoostVertex;
typedef boost::graph_traits<BoostGraph>::edge_descriptor   BoostEdge;
// for the iterators
typedef boost::graph_traits<BoostGraph>::vertex_iterator   BoostVertexIt;
typedef boost::graph_traits<BoostGraph>::edge_iterator     BoostEdgeIt;
// for the edge-property-map (aka edge-array)
typedef boost::property_map<BoostGraph, boost::edge_weight_t>::type BoostWeightMap;
