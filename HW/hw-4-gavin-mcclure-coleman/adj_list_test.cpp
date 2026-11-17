// FILE: adj_list_test.cpp
// NAME: S. Bowers
// DATE: Fall 2026
// DESC: Unit tests for the AdjList class.

#include <set>
#include <stdexcept>
#include "doctest.h"
#include "adj_list.h"

using Set = std::set<int>;

TEST_CASE("new graph has n vertices and no edges")
{
  AdjList g(5);
  CHECK(g.vertex_count() == 5);
  CHECK(g.edge_count() == 0);
  for (int x = 0; x < 5; ++x) {
    CHECK(g.out(x).empty());
    CHECK(g.in(x).empty());
    CHECK(g.adj(x).empty());
  }
}

TEST_CASE("constructor rejects fewer than one vertex")
{
  CHECK_THROWS_AS(AdjList(0), std::invalid_argument);
  CHECK_THROWS_AS(AdjList(-3), std::invalid_argument);
  CHECK_NOTHROW(AdjList(1));
}

TEST_CASE("has_vertex accepts only 0 to n-1")
{
  AdjList g(3);
  CHECK(g.has_vertex(0));
  CHECK(g.has_vertex(1));
  CHECK(g.has_vertex(2));
  CHECK_FALSE(g.has_vertex(3));
  CHECK_FALSE(g.has_vertex(-1));
}

TEST_CASE("add_edge adds a directed edge")
{
  AdjList g(3);
  g.add_edge(0, 1);
  CHECK(g.edge_count() == 1);
  CHECK(g.has_edge(0, 1));
  CHECK_FALSE(g.has_edge(1, 0));
}

TEST_CASE("add_edge ignores duplicate edges")
{
  AdjList g(3);
  g.add_edge(0, 1);
  g.add_edge(0, 1);
  CHECK(g.edge_count() == 1);
  CHECK(g.out(0) == Set{1});
  CHECK(g.in(1) == Set{0});
}

TEST_CASE("add_edge ignores invalid vertices")
{
  AdjList g(3);
  g.add_edge(0, 3);
  g.add_edge(-1, 0);
  g.add_edge(5, 7);
  CHECK(g.edge_count() == 0);
  CHECK_FALSE(g.has_edge(0, 3));
  CHECK(g.out(0).empty());
}

TEST_CASE("an edge and its reversal are different edges")
{
  AdjList g(2);
  g.add_edge(0, 1);
  g.add_edge(1, 0);
  CHECK(g.edge_count() == 2);
  CHECK(g.has_edge(0, 1));
  CHECK(g.has_edge(1, 0));
}

TEST_CASE("self-loops are allowed")
{
  AdjList g(2);
  g.add_edge(1, 1);
  CHECK(g.edge_count() == 1);
  CHECK(g.has_edge(1, 1));
  CHECK(g.out(1) == Set{1});
  CHECK(g.in(1) == Set{1});
  CHECK(g.adj(1) == Set{1});
}

TEST_CASE("out, in, and adj return the correct neighbors")
{
  // edges: 0->1, 0->2, 2->0, 3->0
  AdjList g(4);
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(2, 0);
  g.add_edge(3, 0);
  CHECK(g.out(0) == Set{1, 2});
  CHECK(g.in(0) == Set{2, 3});
  CHECK(g.adj(0) == Set{1, 2, 3});
  CHECK(g.out(1).empty());
  CHECK(g.in(1) == Set{0});
  CHECK(g.adj(1) == Set{0});
  CHECK(g.out(3) == Set{0});
  CHECK(g.in(3).empty());
}

TEST_CASE("out, in, and adj return empty sets for invalid vertices")
{
  AdjList g(2);
  g.add_edge(0, 1);
  CHECK(g.out(2).empty());
  CHECK(g.in(-1).empty());
  CHECK(g.adj(5).empty());
}

TEST_CASE("changing a returned set does not change the graph")
{
  AdjList g(3);
  g.add_edge(0, 1);
  Set s = g.out(0);
  s.insert(2);
  CHECK_FALSE(g.has_edge(0, 2));
  CHECK(g.out(0) == Set{1});
}

TEST_CASE("remove_edge removes only the given edge")
{
  AdjList g(3);
  g.add_edge(0, 1);
  g.add_edge(1, 0);
  g.add_edge(1, 2);
  g.remove_edge(0, 1);
  CHECK(g.edge_count() == 2);
  CHECK_FALSE(g.has_edge(0, 1));
  CHECK(g.has_edge(1, 0));
  CHECK(g.out(0).empty());
  CHECK(g.in(1).empty());
  CHECK(g.in(0) == Set{1});
}

TEST_CASE("remove_edge ignores missing edges and invalid vertices")
{
  AdjList g(3);
  g.add_edge(0, 1);
  g.remove_edge(1, 0);
  g.remove_edge(0, 2);
  g.remove_edge(0, 5);
  g.remove_edge(-1, 1);
  CHECK(g.edge_count() == 1);
  CHECK(g.has_edge(0, 1));
}

TEST_CASE("an edge can be added again after it is removed")
{
  AdjList g(2);
  g.add_edge(0, 1);
  g.remove_edge(0, 1);
  g.add_edge(0, 1);
  CHECK(g.edge_count() == 1);
  CHECK(g.has_edge(0, 1));
}

TEST_CASE("out, in, has_edge, and edge_count agree after many changes")
{
  AdjList g(5);
  // add 9 edges, one of them twice
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(1, 2);
  g.add_edge(2, 0);
  g.add_edge(2, 3);
  g.add_edge(3, 3);
  g.add_edge(3, 4);
  g.add_edge(4, 0);
  g.add_edge(4, 1);
  g.add_edge(0, 1);
  // remove 3 edges, add one back, and remove a missing edge
  g.remove_edge(0, 2);
  g.remove_edge(3, 3);
  g.remove_edge(4, 1);
  g.add_edge(4, 1);
  g.remove_edge(1, 0);
  CHECK(g.edge_count() == 7);
  // each edge must appear in out, in, and has_edge, or in none of them
  int out_total = 0;
  int in_total = 0;
  for (int x = 0; x < 5; ++x) {
    out_total += g.out(x).size();
    in_total += g.in(x).size();
    for (int y = 0; y < 5; ++y) {
      bool edge = g.has_edge(x, y);
      CHECK(edge == (g.out(x).count(y) == 1));
      CHECK(edge == (g.in(y).count(x) == 1));
    }
  }
  CHECK(out_total == 7);
  CHECK(in_total == 7);
}
