// finds contradictions in tables of double replacement reactions from chem h
// lab

#include <array>
#include <deque>
#include <iomanip>
#include <iostream>
#include <vector>

//===-- definitions --===

// clang-format off
enum cat { ag, ca, cu, fe, sr, zn, pb, mn, al, ba, k, mg, na, nh4, sn, cr, co, ni, num_cats };
// clang-format on

const std::array<std::string, num_cats> cat_names = {
    "Ag", "Ca", "Cu", "Fe", "Sr",  "Zn", "Pb", "Mn", "Al",
    "Ba", "K",  "Mg", "Na", "NH4", "Sn", "Cr", "Co", "Ni",
};

std::ostream &operator<<(std::ostream &os, const cat &c) {
  return os << cat_names.at(c);
}

enum an { no3, cl, i, po4, cro4, oh, br, co3, so4, c2o4, c2h3o2, num_ans };

const std::array<std::string, num_ans> an_names = {
    "NO3", "Cl", "I", "PO4", "CrO4", "OH", "Br", "CO3", "SO4", "C2O4", "C2H3O2",
};

std::ostream &operator<<(std::ostream &os, const an &a) {
  return os << an_names.at(a);
}

using compound = std::pair<const cat, const an>;

std::ostream &operator<<(std::ostream &os, const compound &c) {
  return os << c.first << c.second;
}

struct mix {
  const compound s0, s1;
  bool p;
};

using evidence_table =
    std::array<std::array<std::deque<size_t>, num_ans>, num_cats>;

std::ostream &operator<<(std::ostream &os, const evidence_table &t) {
  os << "\t";
  for (size_t i(0); i < num_ans; ++i) {
    os << std::setw(8) << an_names[i];
  }

  os << "\n";

  for (size_t i(0); i < num_cats; ++i) {
    os << std::setw(8) << cat_names[i];
    for (size_t j(0); j < num_ans; ++j) {
      os << std::setw(8) << t[i][j].size();
    }
    os << "\n";
  }

  return os;
}

//===-- data --===

const std::vector<compound> s1_rows{
    {al, cl}, {ba, cl},  {k, i},     {k, po4},  {k, cro4},  {mg, cl},
    {na, oh}, {na, br},  {nh4, co3}, {na, so4}, {na, c2o4}, {na, c2h3o2},
    {sn, cl}, {cr, so4}, {co, cl},   {ni, cl},
};

const std::vector<compound> s1_cols{
    {ag, no3}, {ca, no3}, {cu, no3}, {fe, no3},
    {sr, no3}, {zn, no3}, {pb, no3}, {mn, no3},
};

// clang-format off
const std::vector<std::vector<bool>> s1_data{
    {1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {1, 1, 1, 0, 1, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 1, 1},
    {1, 0, 0, 1, 0, 1, 1, 1},
};
// clang-format on

const std::vector<compound> s2_rows{
    {na, so4}, {na, oh}, {na, br}, {na, c2o4}, {na, c2h3o2}};

const std::vector<compound> s2_cols{{k, cro4}, {k, i}, {k, po4}, {nh4, co3}};

const std::vector<std::vector<bool>> s2_data(5, std::vector<bool>(4, false));

const std::vector<compound> s3_rows{{al, cl}, {ba, cl}, {mg, cl}, {sn, cl},
                                    {co, cl}, {ni, cl}, {cr, so4}};

const std::vector<compound> s3_cols{{k, cro4},  {k, i},       {k, po4},
                                    {na, so4},  {na, oh},     {na, br},
                                    {na, c2o4}, {na, c2h3o2}, {nh4, co3}};

// clang-format off
const std::vector<std::vector<bool>> s3_data{
    {1, 0, 1, 0, 1, 0, 0, 0, 0},
    {1, 0, 1, 1, 1, 0, 1, 0, 1},
    {0, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 1, 1, 1, 1, 1},
    {0, 0, 1, 0, 1, 0, 0, 1, 1},
    {0, 0, 1, 0, 1, 1, 0, 0, 1},
};
// clang-format on

//===-- actual code --===

void table_to_list(const std::vector<compound> &rows,
                   const std::vector<compound> &cols,
                   const std::vector<std::vector<bool>> &table,
                   std::deque<mix> &list) {
  for (size_t row(0); row < rows.size(); ++row) {
    for (size_t col(0); col < cols.size(); ++col) {
      if (rows[row].first != cols[col].first &&
          rows[row].second != cols[col].second) {
        list.push_back({rows[row], cols[col], table[row][col]});
      }
    }
  }
}

std::deque<mix> gen_mixes() {
  std::deque<mix> mixes;
  mixes.push_back({{}, {}, false});  // mixes are 1-indexed, [0] used as delim
  table_to_list(s1_rows, s1_cols, s1_data, mixes);
  table_to_list(s2_rows, s2_cols, s2_data, mixes);
  table_to_list(s3_rows, s3_cols, s3_data, mixes);
  return mixes;
}

evidence_table gen_soluble_ev(const std::deque<mix> &mixes) {
  evidence_table soluble_ev;
  for (size_t i(1); i < mixes.size(); ++i) {
    if (!mixes[i].p) {
      soluble_ev[mixes[i].s0.first][mixes[i].s0.second].push_back(i);
      soluble_ev[mixes[i].s0.first][mixes[i].s1.second].push_back(i);
      soluble_ev[mixes[i].s1.first][mixes[i].s0.second].push_back(i);
      soluble_ev[mixes[i].s1.first][mixes[i].s1.second].push_back(i);
    }
  }
  return soluble_ev;
}

// TODO: maybe push more evidence: take advantage of delim
evidence_table gen_insol_ev(const std::deque<mix> &mixes,
                            const evidence_table &soluble_ev) {
  evidence_table insol_ev;
  for (size_t i(1); i < mixes.size(); ++i) {
    if (mixes[i].p) {
      if (!soluble_ev[mixes[i].s0.first][mixes[i].s1.second].empty()) {
        insol_ev[mixes[i].s1.first][mixes[i].s0.second].push_back(i);
      }
      if (!soluble_ev[mixes[i].s1.first][mixes[i].s0.second].empty()) {
        insol_ev[mixes[i].s0.first][mixes[i].s1.second].push_back(i);
      }
    }
  }
  return insol_ev;
}

std::deque<compound> gen_contradictions(const evidence_table &soluble_ev,
                                        const evidence_table &insol_ev) {
  std::deque<compound> contradictions;
  for (size_t i(0); i < num_cats; ++i) {
    for (size_t j(0); j < num_ans; ++j) {
      if (!soluble_ev[i][j].empty() && !insol_ev[i][j].empty()) {
        contradictions.push_back({static_cast<cat>(i), static_cast<an>(j)});
      }
    }
  }
  return contradictions;
}

long long find_worst_mix(const std::deque<compound> &contradictions,
                         const evidence_table &soluble_ev,
                         const evidence_table &insol_ev,
                         const size_t mixes_size) {
  std::vector<int> trouble_count(mixes_size);
  for (const auto &contradiction : contradictions) {
    for (const size_t mix_num :
         soluble_ev[contradiction.first][contradiction.second]) {
      ++trouble_count[mix_num];
    }

    for (const size_t mix_num :
         insol_ev[contradiction.first][contradiction.second]) {
      ++trouble_count[mix_num];
    }
  }
  return std::max_element(trouble_count.begin(), trouble_count.end()) -
         trouble_count.begin();
}

std::ostream &operator<<(std::ostream &os, const std::deque<size_t> &di) {
  for (const size_t i : di) {
    os << " " << i;
  }
  return os;
}

int main() {
  std::deque<mix> mixes(gen_mixes());

  evidence_table soluble_ev(gen_soluble_ev(mixes));
  std::cout << "number of reactions supporting solubility\n"
            << soluble_ev << "\n";

  evidence_table insol_ev(gen_insol_ev(mixes, soluble_ev));
  std::cout << "number of reactions supporting insolubility\n"
            << insol_ev << "\n";

  std::deque<compound> contradictions(gen_contradictions(soluble_ev, insol_ev));
  for (const auto &contradiction : contradictions) {
    std::cout << contradiction << "\nsoluble evidence:"
              << soluble_ev[contradiction.first][contradiction.second]
              << "\ninsoluble evidence:"
              << insol_ev[contradiction.first][contradiction.second] << "\n\n";
  }
}
