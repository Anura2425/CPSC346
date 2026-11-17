// FILE: dfs_algos.cpp
// NAME: Gavin McClure-Coleman
// DATE: Fall, 2026
// DESC: Function definitions for the DFS-based graph algorithms.

#include <algorithm>
#include <stack>
#include "dfs_algos.h"


// Helper that returns the elements of s in decreasing order.
std::vector<int> reversed(const std::set<int>& s)
{
  return std::vector<int>(s.rbegin(), s.rend());
}

std::map<int,int> dfs_recursive(const AdjList& g, int s)
{
  // TODO
  return {};
}

void dfs_visit(const AdjList& g, int x, std::vector<bool>& visited,
               std::map<int,int>& parent)
{
  // TODO
}

std::map<int,int> dfs_iterative(const AdjList& g, int s)
{
  // TODO
  return {};
}

bool has_cycle(const AdjList& g)
{
  // TODO
  return false;
}

bool is_bipartite(const AdjList& g)
{
  // TODO
  return false;
}

std::vector<int> topological_sort(const AdjList& g)
{
  // TODO
  return {};
}

std::set<std::set<int>> strongly_connected_components(const AdjList& g)
{
  // TODO
  return {};
}
