#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;
using input_t = std::string;

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    inputValues = line;
    break;
  }
  return inputValues;
}

ll search_distint_letter(std::string const &message, size_t size) {
  std::unordered_set<char> letters;
  for (auto it = message.cbegin();
       it != std::next(message.cbegin(), message.size() - size); it++) {
    letters.clear();
    std::transform(it, std::next(it, size),
                   std::inserter(letters, letters.end()),
                   [](auto const &l) { return l; });
    if (letters.size() == size) {
      return std::distance(std::cbegin(message), it) + size;
    }
  }
  return -1;
}

ll solve1(input_t const &aInputData) {
  return search_distint_letter(aInputData, 4);
}

ll solve2(input_t const &aInputData) {
  return search_distint_letter(aInputData, 14);
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}