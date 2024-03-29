#pragma once

#include <bits/stdc++.h>

class Graph {
 public:
  void make_silly_snake() {
    _nodes = {{{1, "i-see"}},      {{2, "the-silly-snake"}}, {{3, "i-be"}},
              {{4, "you-saw-me"}}, {{5, "once-like"}},       {{0, "kekule"}}};
  }

  void make_mr_sticky() {
    _nodes = {{{1, "."}}, {{2, "."}, {3, "."}, {4, "."}}, {},
              {},         {{5, "."}, {6, "."}},           {},
              {}};
  }

  void make_driftin_dragonfly() {
    _nodes = {{{1, "dis-be-me-head"}},
              {{2, "me"}, {5, "dis-be-me"}, {9, "dis-be-me"}},
              {{3, "long-tail"}},
              {{4, "be-dis"}},
              {},
              {{6, "front-right"}},
              {{7, "me-right"}},
              {{8, "back"}},
              {{2, "be-dis"}},
              {{10, "front-left"}},
              {{11, "and-left"}},
              {{12, "back"}},
              {{2, "be-dis"}}};
  }

  void make_slinky_star() {
    _nodes = {{}};
    _nodes.resize(15);
    for (int i(1); i < 15; i += 2) {
      _nodes[0].push_back({i, "-"});
      _nodes[i] = {{i + 1, "-"}};
    }
  }

  void make_kathy_da_grate() {
    _nodes.clear();
    _nodes.resize(15);
    for (int i(0); i < 5; ++i) {
      _nodes[i] = {{(i + 1) % 5, "-"}, {2 * i + 5, "-"}};
      _nodes[2 * i + 5] = {{2 * i + 6, "-"}};
    }
  }

  void make_dodos_in_space() {
    _nodes.clear();
    _nodes.resize(50);
    for (int i(0); i < 50; i += 2)
      _nodes[i] = {{i + 1, "Yippee-Dodo-#" + std::to_string(i)}};
  }

  void make_purty_pitcher() {
    _nodes = {{}, {}};
    sierpinski(3, 0, 1);
  }

 protected:
  friend class Tests;

  struct Edge {
    int _dst;
    std::string _tag;
  };

  std::vector<std::vector<Edge>> _nodes;

 private:
  int sierpinski(int depth, int root, int root1) {
    if (depth == 0) {
      _nodes[root1].push_back({root, ""});
      _nodes.push_back({{root, ""}, {root1, ""}});
      return _nodes.size() - 1;
    }

    int mid(_nodes.size());
    _nodes.push_back({});
    return sierpinski(depth - 1, sierpinski(depth - 1, root, mid),
                      sierpinski(depth - 1, mid, root1));
  }

  void old_sierpinski() {
    static const int &DEPTH(3);
    static const int &NUM_ROWS(1 << DEPTH);
    static const char &AUTOMATON_RULE(60);

    _nodes = {{}};
    std::vector<int> prev_gen = {0};

    for (int i(1); i < NUM_ROWS; ++i) {
      std::vector<int> curr_gen(prev_gen.size() + 1, -1);

      for (int j(0); j < static_cast<int>(curr_gen.size()); ++j) {
        const auto prev_gen_get([&](const size_t pos) {
          return pos < prev_gen.size() ? std::min(prev_gen[pos] + 1, 1) : 0;
        });

        if ((AUTOMATON_RULE >> (prev_gen_get(j - 1) * 4 + prev_gen_get(j) * 2 +
                                prev_gen_get(j + 1)) &
             1) == 1) {
          curr_gen[j] = _nodes.size();

          std::vector<Edge> new_node;
          if (j > 0 && curr_gen[j - 1] != -1)
            new_node.push_back({curr_gen[j - 1], ""});

          if (j > 0 && prev_gen[j - 1] != -1)
            new_node.push_back({prev_gen[j - 1], ""});

          if (j < int(prev_gen.size()) && prev_gen[j] != -1)
            new_node.push_back({prev_gen[j], ""});

          _nodes.push_back(new_node);
        }
      }

      prev_gen = curr_gen;
    }
  }
};
