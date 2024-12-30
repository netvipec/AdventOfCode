#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <utility>
#include <vector>

using ll = int64_t;

struct MapData {
  std::vector<std::vector<ll>> map;
  std::pair<ll, ll> start;
  std::pair<ll, ll> end;
};

using input_t = MapData;

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    inputValues.map.push_back(std::vector<ll>{});
    std::transform(std::cbegin(line), std::cend(line),
                   std::back_inserter(inputValues.map.back()),
                   [&](auto const &c) {
                     if (c == 'E') {
                       inputValues.end.first = inputValues.map.size() - 1;
                       inputValues.end.second = inputValues.map.back().size();
                       return 'z' - 'a';
                     }
                     if (c == 'S') {
                       inputValues.start.first = inputValues.map.size() - 1;
                       inputValues.start.second = inputValues.map.back().size();
                       return 'a' - 'a';
                     }
                     return c - 'a';
                   });
  }
  return inputValues;
}

std::vector<std::pair<ll, ll>> neighbours(std::pair<ll, ll> const &pos) {
  std::vector<std::pair<ll, ll>> dir{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  std::vector<std::pair<ll, ll>> n;
  std::transform(std::cbegin(dir), std::cend(dir), std::back_inserter(n),
                 [&](auto const &d) {
                   return std::make_pair(pos.first + d.first,
                                         pos.second + d.second);
                 });
  return n;
}

ll solve1(input_t const &aInputData) {
  std::set<std::pair<ll, ll>> visited;
  visited.insert(aInputData.start);

  std::deque<std::pair<std::pair<ll, ll>, ll>> process{{aInputData.start, 0}};
  while (!process.empty()) {
    auto const pos = process.front();
    process.pop_front();

    if (pos.first == aInputData.end) {
      return pos.second;
    }

    auto const n = neighbours(pos.first);
    std::for_each(std::cbegin(n), std::cend(n), [&](auto const &np) {
      if (!(0 <= np.first &&
            np.first < static_cast<ll>(aInputData.map.size()) &&
            0 <= np.second &&
            np.second < static_cast<ll>(aInputData.map[np.first].size()))) {
        return;
      }

      if (aInputData.map[np.first][np.second] <=
          aInputData.map[pos.first.first][pos.first.second] + 1) {
        auto pair = visited.insert(np);
        if (!pair.second) {
          return;
        }

        process.emplace_back(np, pos.second + 1);
      }
    });
  }
  return -1;
}

ll solve2(input_t const &aInputData) {
  std::set<std::pair<ll, ll>> visited;
  visited.insert(aInputData.end);

  std::deque<std::pair<std::pair<ll, ll>, ll>> process{{aInputData.end, 0}};
  while (!process.empty()) {
    auto const pos = process.front();
    process.pop_front();

    if (aInputData.map[pos.first.first][pos.first.second] == 0) {
      return pos.second;
    }

    auto const n = neighbours(pos.first);
    std::for_each(std::cbegin(n), std::cend(n), [&](auto const &np) {
      if (!(0 <= np.first &&
            np.first < static_cast<ll>(aInputData.map.size()) &&
            0 <= np.second &&
            np.second < static_cast<ll>(aInputData.map[np.first].size()))) {
        return;
      }

      if (aInputData.map[pos.first.first][pos.first.second] - 1 <=
          aInputData.map[np.first][np.second]) {
        auto pair = visited.insert(np);
        if (!pair.second) {
          return;
        }

        process.emplace_back(np, pos.second + 1);
      }
    });
  }
  return -1;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}