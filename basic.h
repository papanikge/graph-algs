//
// George 'papanikge' Papanikolaou
// CEID Advance Algorithm Desing Course 2014
// Library of Efficient Data types and Algorithms (LEDA) testing
//
// Basic Header file
//
#ifndef BASIC_H
#define BASIC_H

#include <LEDA/graph/graph.h>
#include <LEDA/graph/graph_gen.h>
#include <LEDA/graph/node_array.h>
#include <LEDA/graph/basic_graph_alg.h>

int my_STRONG_COMPONENTS(leda::graph& G, leda::node_array<int>& compnum);
bool STRONG_COMPONENTS_checker(leda::graph& G, leda::node_array<int>& compnum);

#endif
