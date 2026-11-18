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
  if (!g.has_vertex(s)){
    return {};
  }
  std::vector<bool> visited(g.vertex_count(), false);
  std::map<int, int> parent;
  parent[s] = -1;
  dfs_visit(g, s, visited, parent);
  return parent;
}

void dfs_visit(const AdjList& g, int x, std::vector<bool>& visited,
               std::map<int,int>& parent)
{
  visited[x] = true;
  for (int y : g.out(x)){
    if (!visited[y]){
      parent[y] = x;
      dfs_visit(g, y, visited, parent);
    }
  }
}

std::map<int,int> dfs_iterative(const AdjList& g, int s)
{
  if (!g.has_vertex(s)){
    return {};
  }
  std::vector<bool> visited(g.vertex_count(), false);
  std::map<int,int> parent;
  // each entry holds the vertex and the vertex that pushed it
  std::stack<std::pair<int,int>> todo;
  todo.push({s, -1});
  while (!todo.empty()){
    auto [x, p] = todo.top();
    todo.pop();
  
    // a vertex could be pushed multiple times, but we only want to count the first pop
    if (visited[x]){
      continue;
    }
    visited[x] = true;
    parent[x] = p;
    // push in dec order to make sure the smallest neigbor gets popped first
    for (int y: reversed(g.out(x))){
      if (!visited[y]){
        todo.push({y, x});
      }
    }
  }

  return parent;
}

bool has_cycle(const AdjList& g)
{
  std::vector<int> post;
  return dfs_finish_order(g, post);
}

bool is_bipartite(const AdjList& g)
{
  int n = g.vertex_count();
  // -1 is uncolored
  std::vector<int> color(n, -1);
  for (int s = 0; s < n; ++s){
    if (color[s] != -1){
      continue;
    }
    color[s] = 0;
    std::stack<int> todo;
    todo.push(s);
    while (!todo.empty()){
      int x = todo.top();
      todo.pop();
      // look at all adj vertices
      for (int y : g.adj(x)){
        if (color[y] == -1){
          color[y] = 1 - color[x];
          todo.push(y);
        } else if (color[y]==color[x]){
          return false;
        }
      }
    }
  }
  return true;
}

std::vector<int> topological_sort(const AdjList& g)
{
  std::vector<int> order;
  if (dfs_finish_order(g, order)){
    return {};
  }
  std::reverse(order.begin(), order.end());
  return order;
}

std::set<std::set<int>> strongly_connected_components(const AdjList& g)
{
  // TODO implement algo from class here?
  return {};
}
