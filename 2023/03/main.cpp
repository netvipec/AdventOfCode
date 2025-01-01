#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <ranges>
#include <unordered_map>
#include <utility>

using ll = int64_t;

struct Position {
  int row;
  int col;

  bool operator<(Position const &rhs) const {
    return std::tie(row, col) < std::tie(rhs.row, rhs.col);
  }
};

struct input_t {
  std::map<Position, int> iNumbers;
  std::map<Position, char> iSymbols;
};

input_t read_numbers(std::vector<std::vector<char>> const &aInputData) {
  input_t res;

  for (int row = 0; row < aInputData.size(); row++) {
    int start_col = 0;
    int number = 0;
    auto const &rowData = aInputData[row];
    for (int col = 0; col < aInputData.size(); col++) {
      if ('0' <= rowData[col] && rowData[col] <= '9') {
        if (number == 0) {
          start_col = col;
        }
        number = number * 10 + (rowData[col] - '0');
      } else if (start_col != col) {
        if (rowData[col] != '.') {
          res.iSymbols.insert(std::make_pair(Position{row, col}, rowData[col]));
        }
        if (number > 0) {
          res.iNumbers.insert(std::make_pair(Position{row, start_col}, number));
          number = 0;
        }
      }
    }

    if (number > 0) {
      res.iNumbers.insert(std::make_pair(Position{row, start_col}, number));
    }
  }

  return res;
}

input_t readInput() {
  std::vector<std::vector<char>> map;
  std::string line;
  while (std::getline(std::cin, line)) {
    std::vector<char> l;
    std::transform(std::cbegin(line), std::cend(line), std::back_inserter(l),
                   [](auto const &elem) { return elem; });
    map.push_back(l);
  }
  auto const numbers = read_numbers(map);
  return numbers;
}

int digitsCount(int n) {
  int d = 0;
  while (n > 0) {
    d++;
    n /= 10;
  }
  return d;
}

bool isAdjacent(std::pair<Position, int> const &number,
                std::pair<Position, char> const &symbol) {
  auto const digits = digitsCount(number.second);
  return ((symbol.first.row >= number.first.row - 1 &&
           symbol.first.row <= number.first.row + 1) &&
          (symbol.first.col >= number.first.col - 1 &&
           symbol.first.col <= number.first.col + digits));
}

ll solve1(input_t const &aInputData) {
  return std::accumulate(
      std::cbegin(aInputData.iNumbers), std::cend(aInputData.iNumbers), 0,
      [&](const auto &base, const auto &number) {
        auto adjacent = std::any_of(
            std::cbegin(aInputData.iSymbols), std::cend(aInputData.iSymbols),
            [&](auto const &symbol) { return isAdjacent(number, symbol); });
        return base + (adjacent ? number.second : 0);
      });
}

ll solve2(input_t const &aInputData) {
  int gearRatios = 0;
  for (auto const &symbol : aInputData.iSymbols) {
    if (symbol.second == '*') {
      std::vector<std::pair<Position, int>> adj;
      std::copy_if(
          std::cbegin(aInputData.iNumbers), std::cend(aInputData.iNumbers),
          std::back_inserter(adj),
          [&](const auto &number) { return isAdjacent(number, symbol); });
      if (adj.size() == 2) {
        gearRatios += adj.front().second * adj.back().second;
      }
    }
  }

  return gearRatios;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}