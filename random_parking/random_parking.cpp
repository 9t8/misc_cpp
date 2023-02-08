#include <iostream>
#include <random>

typedef long long ll;

ll sim_rand_parking(const double width) {
  if (width < 1) {
    return 0;
  }

  ll cars_parked(0);

  // prefix sum of preceding valid area for each itvl
  std::vector<double> itvl_prefixes{width - 1};

  for (; !itvl_prefixes.empty(); ++cars_parked) {
    // generate random location for new car
    static std::mt19937_64 rng(std::random_device{}());
    double new_loc(std::uniform_real_distribution<>(0,
#ifdef __clang__  // https://github.com/llvm/llvm-project/issues/19141
                                                    itvl_prefixes.back()
#else
                                                    std::nextafter(
                                                        itvl_prefixes.back(),
                                                        itvl_prefixes.back() +
                                                            1)
#endif
                                                        )(rng));

    auto selected_itvl(
        lower_bound(itvl_prefixes.begin(), itvl_prefixes.end(), new_loc));

    double left_offset(new_loc - (selected_itvl == itvl_prefixes.begin()
                                      ? 0
                                      : selected_itvl[-1])),
        right_offset(*selected_itvl - new_loc);

    // insert new car
    if (left_offset < 1 && right_offset < 1) {
      // remove interval
      double itvl_len(left_offset + right_offset);
      while (++selected_itvl < itvl_prefixes.end()) {
        selected_itvl[-1] = *selected_itvl - itvl_len;
      }
      itvl_prefixes.pop_back();
      continue;
    }
    if (left_offset >= 1 && right_offset >= 1) {
      // add interval
      size_t stop(static_cast<size_t>(selected_itvl - itvl_prefixes.begin()));
      itvl_prefixes.push_back(itvl_prefixes.back() - 2);
      for (size_t i(itvl_prefixes.size() - 2); i > stop; --i) {
        itvl_prefixes[i] = itvl_prefixes[i - 1] - 2;
      }
      itvl_prefixes[stop] = new_loc - 1;
      continue;
    }
    // shorten interval
    double lost_area(std::min(left_offset, 1.) + std::min(right_offset, 1.));
    for (; selected_itvl < itvl_prefixes.end(); ++selected_itvl) {
      *selected_itvl -= lost_area;
    }
  }
  return cars_parked;
}

int main() {
  for (const double width :
       std::vector<double>{.5, 1, 2, 3, 3.5, 4, 5, 1e1, 1e2, 1e3, 1e4, 1e5}) {
    ll num_trials(static_cast<ll>(1e6 / width)), total(0);
    for (ll i(0); i < num_trials; ++i) {
      total += sim_rand_parking(width);
    }
    std::cout << width << " (" << num_trials << " trials): "
              << static_cast<double>(total) / static_cast<double>(num_trials) /
                     width
              << std::endl;
  }
}