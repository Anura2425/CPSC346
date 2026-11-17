// FILE: adj_list.cpp
// NAME: Gavin McClure-Coleman
// DATE: Fall, 2026
// DESC: Function definitions for the AdjList class.

#include <stdexcept>
#include "adj_list.h"

AdjList::AdjList(int n)
{
  // check n before sizing the vectors
  if (n < 1)
    throw std::invalid_argument("AdjList: n must be at least 1");
  num_vertices = n;
  num_edges = 0;
  // one empty set of neighbors for each vertex
  out_edges.resize(n);
  in_edges.resize(n);
}

void AdjList::add_edge(int x, int y)
{
  // TODO
}

void AdjList::remove_edge(int x, int y)
{
  // TODO
}

bool AdjList::has_edge(int x, int y) const
{
  // TODO
  return false;
}

bool AdjList::has_vertex(int x) const
{
  // TODO
  return false;
}

std::set<int> AdjList::out(int x) const
{
  // TODO
  return {};
}

std::set<int> AdjList::in(int x) const
{
  // TODO
  return {};
}

std::set<int> AdjList::adj(int x) const
{
  // TODO
  return {};
}

int AdjList::vertex_count() const
{
  // TODO
  return 0;
}

int AdjList::edge_count() const
{
  // TODO
  return 0;
}
