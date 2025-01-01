#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;
using grid_t = std::vector<std::vector<bool>>;
using input_t = std::vector<grid_t>;

input_t readInput() {
  input_t inputValues;
  grid_t grid;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      inputValues.push_back(grid);
      grid.clear();
      continue;
    }

    grid.push_back(std::vector<bool>{});
    std::transform(std::cbegin(line), std::cend(line),
                   std::back_inserter(grid.back()), [](auto const &cell) {
                     assert(cell == '.' || cell == '#');
                     return cell == '#';
                   });
  }
  if (!grid.empty()) {
    inputValues.push_back(grid);
  }
  return inputValues;
}

bool grid_fold_vertically(grid_t const &grid, ll middle_index, ll differences) {
  auto const diff =
      std::accumulate(std::cbegin(grid), std::cend(grid), 0ll,
                      [&](auto const &base, auto const &row) {
                        ll d = 0;
                        for (ll j = 0; j <= middle_index; j++) {
                          auto li = middle_index - j;
                          auto ri = middle_index + j + 1;
                          if (li < 0 || ri >= static_cast<ll>(row.size())) {
                            break;
                          }

                          if (row[li] != row[ri]) {
                            d++;
                          }
                        }
                        return base + d;
                      });
  return diff == differences;
}

bool grid_fold_horizontally(grid_t const &grid, ll middle_index,
                            ll differences) {
  ll diff = 0;
  for (ll j = 0; j <= middle_index; j++) {
    auto li = middle_index - j;
    auto ri = middle_index + j + 1;

    if (li < 0 || ri >= static_cast<ll>(grid.size())) {
      break;
    }

    for (ll i = 0; i < static_cast<ll>(grid.front().size()); i++) {
      if (grid[li][i] != grid[ri][i]) {
        diff++;
      }
    }
  }
  return diff == differences;
}

ll get_vertical_reflexion(grid_t const &grid, ll differences) {
  for (ll i = 0; i < static_cast<ll>(grid.front().size()) - 1; i++) {
    if (grid_fold_vertically(grid, i, differences)) {
      return i + 1;
    }
  }
  return 0;
}

ll get_horizontal_reflexion(grid_t const &grid, ll differences) {
  for (ll i = 0; i < static_cast<ll>(grid.size()) - 1; i++) {
    if (grid_fold_horizontally(grid, i, differences)) {
      return i + 1;
    }
  }
  return 0;
}

ll solve1(input_t const &aInputData) {
  ll acc = 0;
  for (size_t i = 0; i < aInputData.size(); i++) {
    auto const vertical_reflexion = get_vertical_reflexion(aInputData[i], 0);
    auto const horizontal_reflexion =
        get_horizontal_reflexion(aInputData[i], 0);

    auto const score =
        vertical_reflexion +
        100 * (vertical_reflexion > 0 ? 0 : horizontal_reflexion);
    acc += score;
  }
  return acc;
}

ll solve2(input_t const &aInputData) {
  ll acc = 0;
  for (size_t i = 0; i < aInputData.size(); i++) {
    auto const vertical_reflexion = get_vertical_reflexion(aInputData[i], 1);
    auto const horizontal_reflexion =
        get_horizontal_reflexion(aInputData[i], 1);

    auto const score =
        vertical_reflexion +
        100 * (vertical_reflexion > 0 ? 0 : horizontal_reflexion);
    acc += score;
  }
  return acc;
}

int main() {
  auto const inputData = readInput();

  // std::for_each(
  //     std::cbegin(inputData), std::cend(inputData), [](auto const &grid) {
  //       std::for_each(std::cbegin(grid), std::cend(grid), [](auto const &row)
  //       {
  //         std::for_each(std::cbegin(row), std::cend(row),
  //                       [](auto const &cell) { std::cout << cell; });
  //         std::cout << std::endl;
  //       });
  //       std::cout << std::endl;
  //     });

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}