#include <algorithm>
#include <bits/stdc++.h>
#include <cstdint>
#include <deque>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;
using input_t = std::deque<ll>;

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    ll n;
    if (!(iss >> n)) {
      break;
    } // error

    inputValues.push_back(n);
  }
  return inputValues;
}

ll mix(std::deque<std::pair<ll, uint16_t>> &arrangement, ll const times = 1) {
  auto const size = static_cast<int>(arrangement.size());
  for (int counter = 0; counter < times; counter++) {
    for (int i = 0; i < size; i++) {
      auto const elem_it = std::find_if(
          std::cbegin(arrangement), std::cend(arrangement),
          [&](auto const &elem) { return elem.second == (i % size); });

      auto const elem_value = elem_it->first;
      auto const elem_index = elem_it->second;
      if (elem_value > 0) {
        auto next_it = arrangement.erase(elem_it);
        auto const elem_move = elem_value % arrangement.size();

        auto distance = std::distance(next_it, std::end(arrangement));
        if (static_cast<ll>(elem_move) <= distance) {
          arrangement.insert(std::next(next_it, elem_move),
                             {elem_value, elem_index});
        } else {
          auto insert_it = std::cbegin(arrangement) + (elem_move - distance);
          arrangement.insert(insert_it, {elem_value, elem_index});
        }
      } else if (elem_value < 0) {
        auto next_it = arrangement.erase(elem_it);
        auto const elem_move = std::abs(elem_value) % arrangement.size();
        auto distance = std::distance(std::begin(arrangement), next_it);
        if (static_cast<ll>(elem_move) <= distance) {
          arrangement.insert(std::next(next_it, -elem_move),
                             {elem_value, elem_index});
        } else {
          auto insert_it = std::cend(arrangement) - (elem_move - distance);
          arrangement.insert(insert_it, {elem_value, elem_index});
        }
      }
    }
  }

  auto const zero_it =
      std::find_if(std::cbegin(arrangement), std::cend(arrangement),
                   [](auto const &elem) { return elem.first == 0; });
  auto const zero_pos = std::distance(std::cbegin(arrangement), zero_it);
  ll sums = 0;
  for (int i = 1; i < 4; i++) {
    auto const v = i * 1000;
    auto const elem = arrangement[(zero_pos + v) % arrangement.size()].first;
    sums += elem;
  }
  return sums;
}

ll solve1(input_t const &aInputData) {
  std::deque<std::pair<ll, uint16_t>> arrangement;
  for (int i = 0; i < static_cast<ll>(aInputData.size()); i++) {
    arrangement.emplace_back(aInputData[i], i);
  }

  return mix(arrangement);
}

ll solve2(input_t const &aInputData) {
  std::deque<std::pair<ll, uint16_t>> arrangement;
  for (int i = 0; i < static_cast<ll>(aInputData.size()); i++) {
    arrangement.emplace_back(aInputData[i] * 811589153ll, i);
  }

  return mix(arrangement, 10);
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}