#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <utility>

using ll = int64_t;
using input_t = std::vector<std::vector<ll>>;

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    inputValues.push_back(std::vector<ll>{});
    std::transform(std::cbegin(line), std::cend(line),
                   std::back_inserter(inputValues.back()),
                   [](auto const &n) { return n - '0'; });
  }
  return inputValues;
}

std::vector<std::pair<ll, ll>> directions{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

ll solve1(input_t const &aInputData) {
  using cell_data_t = std::map<std::pair<ll, ll>, ll>;

  std::vector<std::vector<cell_data_t>> grid(
      aInputData.size(), std::vector<cell_data_t>(aInputData.front().size()));

  grid[0][0].insert({{0, 0}, 0});

  auto const nrows = static_cast<ll>(aInputData.size());
  auto const ncols = static_cast<ll>(aInputData.front().size());

  bool improved = false;
  do {
    improved = false;

    for (ll i = 0; i < nrows; i++) {
      for (ll j = 0; j < ncols; j++) {
        auto const &cell = grid[i][j];
        for (auto const &best : cell) {
          for (ll nd = 0; nd < static_cast<ll>(directions.size()); nd++) {
            if ((nd + 2) % 4 == best.first.first) {
              continue;
            }
            if (best.first.second == 3 && best.first.first == nd) {
              continue;
            }

            auto const &d = directions[nd];
            ll ni = i + d.first;
            ll nj = j + d.second;
            if (ni < 0 || ni >= nrows || nj < 0 || nj >= ncols) {
              continue;
            }

            auto const nc =
                1 + ((nd == best.first.first) ? best.first.second : 0);

            auto const nheat = best.second + aInputData[ni][nj];
            auto &desCell = grid[ni][nj];
            auto &desIt = desCell[{nd, nc}];
            if (nheat < desIt || desIt == 0) {
              desIt = nheat;
              improved = true;
            }
          }
        }
      }
    }
  } while (improved);

  auto const &destination = grid[nrows - 1][ncols - 1];
  return std::min_element(std::cbegin(destination), std::cend(destination),
                          [](auto const &lhs, auto const &rhs) {
                            return lhs.second < rhs.second;
                          })
      ->second;
}

ll solve2(input_t const &aInputData) {
  using cell_data_t = std::map<std::pair<ll, ll>, ll>;

  std::vector<std::vector<cell_data_t>> grid(
      aInputData.size(), std::vector<cell_data_t>(aInputData.front().size()));

  grid[0][0].insert({{1, 0}, 0});
  grid[0][0].insert({{2, 0}, 0});

  auto const nrows = static_cast<ll>(aInputData.size());
  auto const ncols = static_cast<ll>(aInputData.front().size());

  bool improved = false;
  do {
    improved = false;

    for (ll i = 0; i < nrows; i++) {
      for (ll j = 0; j < ncols; j++) {
        auto const &cell = grid[i][j];
        for (auto const &best : cell) {
          for (ll nd = 0; nd < static_cast<ll>(directions.size()); nd++) {
            if ((nd + 2) % 4 == best.first.first) {
              continue;
            }
            if (best.first.second < 4 && nd != best.first.first) {
              continue;
            }
            if (best.first.second == 10 && best.first.first == nd) {
              continue;
            }

            auto const &d = directions[nd];

            ll ni = i + d.first;
            ll nj = j + d.second;
            if (ni < 0 || ni >= nrows || nj < 0 || nj >= ncols) {
              continue;
            }

            auto const nc =
                1 + ((nd == best.first.first) ? best.first.second : 0);

            auto const nheat = best.second + aInputData[ni][nj];
            auto &desCell = grid[ni][nj];
            auto &desIt = desCell[{nd, nc}];
            if (nheat < desIt || desIt == 0) {
              desIt = nheat;
              improved = true;
            }
          }
        }
      }
    }
  } while (improved);

  auto const &destination = grid[nrows - 1][ncols - 1];
  return std::min_element(std::cbegin(destination), std::cend(destination),
                          [](auto const &lhs, auto const &rhs) {
                            return lhs.second < rhs.second;
                          })
      ->second;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}