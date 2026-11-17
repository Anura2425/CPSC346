// FILE: main.cpp
// NAME: S. Bowers
// DATE: Fall, 2026
// DESC: Reads a graph from a text file and prints the result of each
//       DFS-based algorithm. Usage: ./hw4 <graph-file>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "adj_list.h"
#include "dfs_algos.h"

// Reads a graph file where the first line is the number of vertices
// n, and each subsequent line is an edge "x y" (x->y). Returns false
// and prints a message if the file cannot be read or isn't
// well-formed.
bool read_graph(const std::string& filename, int& n,
                std::vector<std::pair<int,int>>& edges)
{
  std::ifstream in(filename);
  if (!in) {
    std::cerr << "error: cannot open " << filename << "\n";
    return false;
  }
  std::string line;
  if (!std::getline(in, line)) {
    std::cerr << "error: " << filename << " is empty\n";
    return false;
  }
  std::istringstream first(line);
  if (!(first >> n) or n < 1) {
    std::cerr << "error: line 1 must be the number of vertices (at least 1)\n";
    return false;
  }
  int line_num = 1;
  while (std::getline(in, line)) {
    ++line_num;
    std::istringstream words(line);
    int x, y;
    if (!(words >> x >> y) or x < 0 or x >= n or y < 0 or y >= n) {
      std::cerr << "error: bad edge on line " << line_num << "\n";
      return false;
    }
    edges.push_back({x, y});
  }
  return true;
}

// Returns a search tree as text, for example {0: -1, 1: 0, 2: 1}.
std::string tree_str(const std::map<int,int>& tree)
{
  std::string s = "{";
  for (auto [x, parent] : tree) {
    if (s.size() > 1)
      s += ", ";
    s += std::to_string(x) + ": " + std::to_string(parent);
  }
  return s + "}";
}

// Returns a set of vertices as text, for example {0, 1, 2}.
std::string set_str(const std::set<int>& vertices)
{
  std::string s = "{";
  for (int x : vertices) {
    if (s.size() > 1)
      s += ", ";
    s += std::to_string(x);
  }
  return s + "}";
}

// Returns a vertex order as text, for example [0, 2, 1].
std::string order_str(const std::vector<int>& order)
{
  std::string s = "[";
  for (int x : order) {
    if (s.size() > 1)
      s += ", ";
    s += std::to_string(x);
  }
  return s + "]";
}

// Returns a set of components as text, for example {{0, 1}, {2}}.
std::string components_str(const std::set<std::set<int>>& components)
{
  std::string s = "{";
  for (const std::set<int>& c : components) {
    if (s.size() > 1)
      s += ", ";
    s += set_str(c);
  }
  return s + "}";
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " <graph-file>\n";
    return 1;
  }
  int n = 0;
  std::vector<std::pair<int,int>> edges;
  if (!read_graph(argv[1], n, edges))
    return 1;
  AdjList g(n);
  for (auto [x, y] : edges)
    g.add_edge(x, y);

  std::cout << "vertices: " << g.vertex_count() << ", edges: "
            << g.edge_count() << "\n";
  std::cout << "dfs_recursive from 0: " << tree_str(dfs_recursive(g, 0)) << "\n";
  std::cout << "dfs_iterative from 0: " << tree_str(dfs_iterative(g, 0)) << "\n";
  std::cout << "has_cycle: " << (has_cycle(g) ? "yes" : "no") << "\n";
  std::cout << "is_bipartite: " << (is_bipartite(g) ? "yes" : "no") << "\n";
  std::vector<int> order = topological_sort(g);
  std::cout << "topological_sort: ";
  if (order.empty())
    std::cout << "none (the graph has a cycle)\n";
  else
    std::cout << order_str(order) << "\n";
  std::cout << "strongly_connected_components: "
            << components_str(strongly_connected_components(g)) << "\n";
}
