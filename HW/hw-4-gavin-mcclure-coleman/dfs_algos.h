// FILE: dfs_algos.h
// NAME: Gavin McClure-Coleman
// DATE: Fall, 2026
// DESC: Graph algorithms based on depth-first search (DFS).

#ifndef DFS_ALGOS_H
#define DFS_ALGOS_H

#include <map>
#include <set>
#include <vector>
#include "adj_list.h"

// Returns the DFS search tree from s, as a map from each vertex
// reached to its parent (the root s maps to -1). Tries out-neighbors
// in increasing order. Returns an empty map if s is not a vertex.
std::map<int,int> dfs_recursive(const AdjList& g, int s);

// Helper for dfs_recursive: visits x and then each unvisited
// out-neighbor of x, recording parents in the search tree.
void dfs_visit(const AdjList& g, int x, std::vector<bool>& visited,
               std::map<int,int>& parent);

// Same result as dfs_recursive, computed without recursion using an
// explicit stack.
std::map<int,int> dfs_iterative(const AdjList& g, int s);

// Returns true if g has a directed cycle (a self-loop counts).
bool has_cycle(const AdjList& g);

// Returns true if g is bipartite, treating each edge as undirected.
bool is_bipartite(const AdjList& g);

// Returns a topological order of the vertices of g, or an empty
// vector if g has a cycle.
std::vector<int> topological_sort(const AdjList& g);

// Returns the strongly connected components of g, each as a set of
// vertices.
std::set<std::set<int>> strongly_connected_components(const AdjList& g);

#endif
