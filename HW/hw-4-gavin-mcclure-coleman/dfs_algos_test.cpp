// FILE: dfs_algos_test.cpp
// NAME: S. Bowers
// DATE: Fall, 2026
// DESC: Unit tests for the DFS-based graph algorithms.

#include <map>
#include <utility>
#include <vector>
#include "doctest.h"
#include "adj_list.h"
#include "dfs_algos.h"

// search tree: each vertex reached maps to its parent (root maps to -1)
using Tree = std::map<int,int>;

// Returns a graph with n vertices and the given edges.
AdjList make_graph(int n, const std::vector<std::pair<int,int>>& edges)
{
  AdjList g(n);
  for (auto [x, y] : edges)
    g.add_edge(x, y);
  return g;
}

//----------------------------------------------------------------------
// dfs_recursive and dfs_iterative
//----------------------------------------------------------------------

TEST_CASE("dfs: invalid start vertex gives an empty tree")
{
  AdjList g = make_graph(3, {{0, 1}});
  CHECK(dfs_recursive(g, 3).empty());
  CHECK(dfs_recursive(g, -1).empty());
  CHECK(dfs_iterative(g, 3).empty());
  CHECK(dfs_iterative(g, -1).empty());
}

TEST_CASE("dfs: start vertex with no out-edges")
{
  AdjList g = make_graph(3, {{1, 0}, {2, 0}});
  CHECK(dfs_recursive(g, 0) == Tree{{0, -1}});
  CHECK(dfs_iterative(g, 0) == Tree{{0, -1}});
}

TEST_CASE("dfs: follows a path")
{
  AdjList g = make_graph(4, {{0, 1}, {1, 2}, {2, 3}});
  Tree expected{{0, -1}, {1, 0}, {2, 1}, {3, 2}};
  CHECK(dfs_recursive(g, 0) == expected);
  CHECK(dfs_iterative(g, 0) == expected);
}

TEST_CASE("dfs: follows edge direction and skips unreachable vertices")
{
  // 0 can reach 1 but not 2 or 3
  AdjList g = make_graph(4, {{0, 1}, {2, 0}, {3, 2}});
  Tree expected{{0, -1}, {1, 0}};
  CHECK(dfs_recursive(g, 0) == expected);
  CHECK(dfs_iterative(g, 0) == expected);
}

TEST_CASE("dfs: longest path first then tries neighbors")
{
  // from 0, reaches 2 through 1
  AdjList g = make_graph(3, {{0, 1}, {0, 2}, {1, 2}});
  Tree expected{{0, -1}, {1, 0}, {2, 1}};
  CHECK(dfs_recursive(g, 0) == expected);
  CHECK(dfs_iterative(g, 0) == expected);
}

TEST_CASE("dfs: cycles and self-loops do not revisit vertices")
{
  AdjList g = make_graph(3, {{0, 0}, {0, 1}, {1, 2}, {2, 0}});
  Tree expected{{0, -1}, {1, 0}, {2, 1}};
  CHECK(dfs_recursive(g, 0) == expected);
  CHECK(dfs_iterative(g, 0) == expected);
}

TEST_CASE("dfs: backtracks to try remaining neighbors")
{
  // 0->1->3 is a dead end, so the search backs up to 0 and then
  // visits 2, then 4 from 2
  AdjList g = make_graph(5, {{0, 1}, {0, 2}, {1, 3}, {2, 4}});
  Tree expected{{0, -1}, {1, 0}, {3, 1}, {2, 0}, {4, 2}};
  CHECK(dfs_recursive(g, 0) == expected);
  CHECK(dfs_iterative(g, 0) == expected);
}

TEST_CASE("dfs: iterative and recursive give same tree from every start")
{
  AdjList g = make_graph(8, {{0, 1}, {0, 4}, {1, 2}, {1, 5}, {2, 3},
                             {3, 1}, {4, 5}, {5, 6}, {6, 4}, {6, 7},
                             {7, 2}});
  for (int s = 0; s < g.vertex_count(); ++s) {
    CAPTURE(s);
    CHECK(dfs_iterative(g, s) == dfs_recursive(g, s));
  }
}

//----------------------------------------------------------------------
// has_cycle
//----------------------------------------------------------------------

TEST_CASE("has_cycle: graph with no edges")
{
  CHECK_FALSE(has_cycle(AdjList(1)));
  CHECK_FALSE(has_cycle(AdjList(4)));
}

TEST_CASE("has_cycle: a self-loop is a cycle")
{
  CHECK(has_cycle(make_graph(3, {{0, 1}, {1, 1}})));
}

TEST_CASE("has_cycle: two-node cycle")
{
  CHECK(has_cycle(make_graph(2, {{0, 1}, {1, 0}})));
}

TEST_CASE("has_cycle: a path is not a cycle")
{
  CHECK_FALSE(has_cycle(make_graph(4, {{0, 1}, {1, 2}, {2, 3}})));
}

TEST_CASE("has_cycle: two paths to the same vertex, not a cycle")
{
  // 3 is reached twice from 0, once through 1 and once through 2
  CHECK_FALSE(has_cycle(make_graph(4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}})));
}

TEST_CASE("has_cycle: a longer cycle")
{
  CHECK(has_cycle(make_graph(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}})));
  CHECK(has_cycle(make_graph(4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 0}})));
}

TEST_CASE("has_cycle: checks vertices not reachable from vertex 0")
{
  // 2 reaches 1 after the search from 0 has finished it
  CHECK_FALSE(has_cycle(make_graph(3, {{0, 1}, {2, 1}})));
  // the cycle 2->3->4->2 is not reachable from 0
  CHECK(has_cycle(make_graph(5, {{0, 1}, {2, 3}, {3, 4}, {4, 2}})));
}

//----------------------------------------------------------------------
// is_bipartite
//----------------------------------------------------------------------

TEST_CASE("is_bipartite: graph with no edges")
{
  CHECK(is_bipartite(AdjList(1)));
  CHECK(is_bipartite(AdjList(4)));
}

TEST_CASE("is_bipartite: a self-loop is not bipartite")
{
  CHECK_FALSE(is_bipartite(make_graph(3, {{0, 1}, {1, 1}})));
}

TEST_CASE("is_bipartite: even cycles are bipartite and odd cycles are not")
{
  CHECK(is_bipartite(make_graph(2, {{0, 1}})));
  CHECK(is_bipartite(make_graph(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}})));
  CHECK_FALSE(is_bipartite(make_graph(3, {{0, 1}, {1, 2}, {2, 0}})));
}

TEST_CASE("is_bipartite: ignores edge direction")
{
  // the path 1-0-2, with both edges into 0
  CHECK(is_bipartite(make_graph(3, {{1, 0}, {2, 0}})));
  // the triangle 0-1-2, with edges in mixed directions
  CHECK_FALSE(is_bipartite(make_graph(3, {{0, 1}, {2, 1}, {0, 2}})));
}

TEST_CASE("is_bipartite: checks vertices not reachable from vertex 0")
{
  // only the second component, 2-3-4, has an odd cycle
  CHECK_FALSE(is_bipartite(make_graph(5, {{0, 1}, {2, 3}, {3, 4}, {4, 2}})));
}

//----------------------------------------------------------------------
// topological_sort
//----------------------------------------------------------------------

// Returns true if order lists each vertex of g once, with x before y
// for each edge x->y.
bool is_topological_order(const AdjList& g, const std::vector<int>& order)
{
  int n = g.vertex_count();
  if ((int)order.size() != n)
    return false;
  // position of each vertex in order (-1 if not seen yet)
  std::vector<int> pos(n, -1);
  for (int i = 0; i < n; ++i) {
    int x = order[i];
    if (!g.has_vertex(x) or pos[x] != -1)
      return false;
    pos[x] = i;
  }
  for (int x = 0; x < n; ++x)
    for (int y : g.out(x))
      if (pos[x] > pos[y])
        return false;
  return true;
}

TEST_CASE("topological_sort: graph with no edges")
{
  AdjList g(3);
  CHECK(is_topological_order(g, topological_sort(g)));
}

TEST_CASE("topological_sort: a path has only one order")
{
  CHECK(topological_sort(make_graph(4, {{0, 1}, {1, 2}, {2, 3}})) ==
        std::vector<int>{0, 1, 2, 3});
  CHECK(topological_sort(make_graph(4, {{3, 2}, {2, 1}, {1, 0}})) ==
        std::vector<int>{3, 2, 1, 0});
}

TEST_CASE("topological_sort: a vertex reached by two paths")
{
  // the last vertex is reached from 0 directly and through the middle
  // vertex, thus, each graph has only one order
  CHECK(topological_sort(make_graph(3, {{0, 1}, {0, 2}, {2, 1}})) ==
        std::vector<int>{0, 2, 1});
  CHECK(topological_sort(make_graph(3, {{0, 1}, {0, 2}, {1, 2}})) ==
        std::vector<int>{0, 1, 2});
  AdjList g = make_graph(4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}});
  CHECK(is_topological_order(g, topological_sort(g)));
}

TEST_CASE("topological_sort: includes vertices not reachable from vertex 0")
{
  AdjList g = make_graph(5, {{0, 2}, {1, 2}, {3, 4}});
  CHECK(is_topological_order(g, topological_sort(g)));
}

TEST_CASE("topological_sort: a larger graph")
{
  AdjList g = make_graph(8, {{0, 3}, {1, 3}, {1, 4}, {2, 4}, {3, 5},
                             {4, 5}, {4, 6}, {5, 7}, {6, 7}, {2, 7}});
  CHECK(is_topological_order(g, topological_sort(g)));
}

TEST_CASE("topological_sort: a graph with a cycle gives an empty vector")
{
  CHECK(topological_sort(make_graph(2, {{0, 1}, {1, 1}})).empty());
  CHECK(topological_sort(make_graph(2, {{0, 1}, {1, 0}})).empty());
  // the cycle 2->3->4->2 is not reachable from 0
  CHECK(topological_sort(make_graph(5, {{0, 1}, {2, 3}, {3, 4}, {4, 2}})).empty());
}

//----------------------------------------------------------------------
// strongly_connected_components
//----------------------------------------------------------------------

// the strongly connected components of a graph
using Components = std::set<std::set<int>>;

TEST_CASE("scc: graph with no edges has one component per vertex")
{
  CHECK(strongly_connected_components(AdjList(1)) == Components{{0}});
  CHECK(strongly_connected_components(AdjList(3)) == Components{{0}, {1}, {2}});
}

TEST_CASE("scc: a self-loop does not join any vertices")
{
  CHECK(strongly_connected_components(make_graph(2, {{0, 0}, {0, 1}})) ==
        Components{{0}, {1}});
}

TEST_CASE("scc: a cycle is one component")
{
  CHECK(strongly_connected_components(make_graph(2, {{0, 1}, {1, 0}})) ==
        Components{{0, 1}});
  CHECK(strongly_connected_components(make_graph(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}})) ==
        Components{{0, 1, 2, 3}});
}

TEST_CASE("scc: each vertex of a path is its own component")
{
  CHECK(strongly_connected_components(make_graph(4, {{0, 1}, {1, 2}, {2, 3}})) ==
        Components{{0}, {1}, {2}, {3}});
  CHECK(strongly_connected_components(make_graph(4, {{3, 2}, {2, 1}, {1, 0}})) ==
        Components{{0}, {1}, {2}, {3}});
}

TEST_CASE("scc: a source component with an edge to a sink component")
{
  AdjList g1 = make_graph(4, {{0, 1}, {1, 0}, {1, 2}, {2, 3}, {3, 2}});
  CHECK(strongly_connected_components(g1) == Components{{0, 1}, {2, 3}});
  AdjList g2 = make_graph(4, {{2, 3}, {3, 2}, {3, 0}, {0, 1}, {1, 0}});
  CHECK(strongly_connected_components(g2) == Components{{0, 1}, {2, 3}});
}

TEST_CASE("scc: a larger graph")
{
  // four components connected by four edges
  AdjList g = make_graph(8, {{0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 3},
                             {6, 7}, {7, 6}, {2, 3}, {4, 5}, {1, 6},
                             {7, 5}});
  CHECK(strongly_connected_components(g) ==
        Components{{0, 1, 2}, {3, 4}, {5}, {6, 7}});
}

//----------------------------------------------------------------------
// Basic scalability tests for larger graphs containing "long paths",
// i.e., where a recursive DFS solution  may run out of stack space.
// ----------------------------------------------------------------------

// Returns the path 0->1->...->n-1.
AdjList make_long_path(int n)
{
  AdjList g(n);
  for (int x = 0; x < n - 1; ++x)
    g.add_edge(x, x + 1);
  return g;
}

// The length of the longest path
const int LONG = 100000;

TEST_CASE("long path: dfs_iterative")
{
  Tree t = dfs_iterative(make_long_path(LONG), 0);
  CHECK(t.size() == LONG);
  CHECK(t[LONG - 1] == LONG - 2);
}

TEST_CASE("long path: has_cycle")
{
  AdjList g = make_long_path(LONG);
  CHECK_FALSE(has_cycle(g));
  g.add_edge(LONG - 1, 0);
  CHECK(has_cycle(g));
}

TEST_CASE("long path: topological_sort")
{
  std::vector<int> order = topological_sort(make_long_path(LONG));
  REQUIRE(order.size() == LONG);
  CHECK(order.front() == 0);
  CHECK(order.back() == LONG - 1);
}

TEST_CASE("long path: strongly_connected_components")
{
  AdjList g = make_long_path(LONG);
  CHECK(strongly_connected_components(g).size() == LONG);
  g.add_edge(LONG - 1, 0);
  CHECK(strongly_connected_components(g).size() == 1);
}
