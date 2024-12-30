#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>

using ll = int64_t;
using input_t = std::vector<std::vector<ll>>;

input_t readInput() {
  input_t inputValues;
  std::vector<ll> inner;

  std::string line;
  while (std::getline(std::cin, line)) {
    std::transform(
        std::cbegin(line), std::cend(line), std::back_inserter(inner),
        [](auto const &elem) { return static_cast<ll>(elem - '0'); });

    inputValues.push_back(inner);
    inner.clear();
  }
  return inputValues;
}

const std::vector<ll> DX{1, -1, 0, 0};
const std::vector<ll> DY{0, 0, 1, -1};

bool isVisibleInDirection(input_t const &map, ll x, ll y, ll d) {
  ll x_n = x + DX[d];
  ll y_n = y + DY[d];
  bool found = true;
  while (0 <= x_n && x_n < static_cast<ll>(map.size()) && 0 <= y_n &&
         y_n < static_cast<ll>(map[x_n].size())) {
    if (map[x_n][y_n] >= map[x][y]) {
      found = false;
      break;
    }
    x_n += DX[d];
    y_n += DY[d];
  }
  return found;
}

ll solve1(input_t const &aInputData) {
  auto const edge_visible =
      2 * aInputData.size() + 2 * aInputData.front().size() - 4;

  auto inner_visible = 0;
  for (size_t x = 1; x < aInputData.size() - 1; x++) {
    for (size_t y = 1; y < aInputData[x].size() - 1; y++) {
      for (size_t d = 0; d < DX.size(); d++) {
        auto found = isVisibleInDirection(aInputData, x, y, d);
        if (found) {
          inner_visible++;
          break;
        }
      }
    }
  }

  return edge_visible + inner_visible;
}

ll visibleTreesInDirection(input_t const &map, ll x, ll y, ll d) {
  ll x_n = x + DX[d];
  ll y_n = y + DY[d];
  ll lenght = 0;
  while (0 <= x_n && x_n < static_cast<ll>(map.size()) && 0 <= y_n &&
         y_n < static_cast<ll>(map[x_n].size())) {
    if (map[x_n][y_n] >= map[x][y]) {
      ++lenght;
      break;
    }
    ++lenght;
    x_n += DX[d];
    y_n += DY[d];
  }
  return lenght;
}

ll solve2(input_t const &aInputData) {
  ll best_score = 0;

  for (size_t x = 1; x < aInputData.size() - 1; x++) {
    for (size_t y = 1; y < aInputData[x].size() - 1; y++) {
      ll score = 1;
      for (size_t d = 0; d < DX.size(); d++) {
        auto lenght = visibleTreesInDirection(aInputData, x, y, d);
        score *= lenght;
      }
      best_score = std::max(best_score, score);
    }
  }

  return best_score;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}