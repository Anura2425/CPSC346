// FILE: adj_list.h
// NAME: Gavin McClure-Coleman
// DATE: Fall, 2026
// DESC: Adjacency list representation of a directed graph. Vertices
//       are assumed to be labeled from 0 to n-1, where n is fixed
//       when the graph is created.

#ifndef ADJ_LIST_H
#define ADJ_LIST_H

#include <set>
#include <vector>

class AdjList
{
public:

  // Creates a graph with n vertices (0 to n-1) and no edges. Throws
  // std::invalid_argument if n is less than 1.
  AdjList(int n);

  // Adds the edge (x,y). Does nothing if x or y is not a vertex or if
  // the edge already exists.
  void add_edge(int x, int y);

  // Removes the edge (x,y). Does nothing if the edge does not exist.
  void remove_edge(int x, int y);

  // Returns true if the graph has the edge (x,y).
  bool has_edge(int x, int y) const;

  // Returns true if x is a vertex, i.e., 0 <= x < vertex_count().
  bool has_vertex(int x) const;

  // Returns the vertices y with an edge (x,y). Returns the empty set
  // if x is not a vertex.
  std::set<int> out(int x) const;

  // Returns the vertices y with an edge (y,x). Returns the empty set
  // if x is not a vertex.
  std::set<int> in(int x) const;

  // Returns the vertices y with an edge (x,y) or (y,x). Returns the
  // empty set if x is not a vertex.
  std::set<int> adj(int x) const;

  // Returns the number of vertices.
  int vertex_count() const;

  // Returns the number of edges.
  int edge_count() const;

private:

  // number of vertices and edges
  int num_vertices;
  int num_edges;

  // out_edges[x] holds each y with an edge (x,y), and in_edges[y]
  // holds each x with an edge (x,y)
  std::vector<std::set<int>> out_edges;
  std::vector<std::set<int>> in_edges;

};

#endif
