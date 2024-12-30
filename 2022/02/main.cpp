#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;
using input_t = std::vector<std::pair<char, char>>;

static std::vector<std::string> split(std::string const &s, char delim,
                                      bool removeEmpty = false) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (std::getline(ss, item, delim)) {
    if (removeEmpty && item.empty())
      continue;
    elems.push_back(item);
  }
  return elems;
}

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    auto elems = split(line, ' ');
    assert(elems.size() == 2);

    inputValues.push_back(std::make_pair(elems.front()[0], elems.back()[0]));
  }
  return inputValues;
}

ll solve1(input_t const &aInputData) {
  return std::accumulate(std::cbegin(aInputData), std::cend(aInputData), 0,
                         [](auto const &base, auto const &round) {
                           auto first = static_cast<int>(round.first) - 'A';
                           auto second = static_cast<int>(round.second) - 'X';
                           auto score = 0;
                           if (first == second) {
                             // Draw
                             score = 3 + second + 1;
                           } else if ((first == 0 && second == 1) ||
                                      (first == 1 && second == 2) ||
                                      (first == 2 && second == 0)) {
                             // Win
                             score = 6 + second + 1;
                           } else {
                             // Loose
                             score = second + 1;
                           }
                           return base + score;
                         });
}

ll solve2(input_t const &aInputData) {
  return std::accumulate(
      std::cbegin(aInputData), std::cend(aInputData), 0,
      [](auto const &base, auto const &round) {
        auto score = 0;
        switch (round.second) {
        case 'X': // Loose
          score = ((static_cast<int>(round.first) - 'A') + 2) % 3 + 1;
          break;
        case 'Y': // Draw
          score = (static_cast<int>(round.first) - 'A') + 4;
          break;
        case 'Z': // Win
          score = ((static_cast<int>(round.first) - 'A') + 1) % 3 + 7;
          break;
        }
        return base + score;
      });
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}