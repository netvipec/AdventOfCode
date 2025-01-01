#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>

using ll = int64_t;
using input_t = std::vector<std::string>;

static std::vector<std::string> split(std::string const &s,
                                      std::string const &delim = " ") {
  std::vector<std::string> elems;
  int start = 0;
  int end = s.find(delim);
  while (end != -1) {
    elems.push_back(s.substr(start, end - start));
    start = end + delim.size();
    end = s.find(delim, start);
  }
  elems.push_back(s.substr(start, end - start));
  return elems;
}

input_t readInput() {
  input_t inputValues;

  std::string line;
  std::getline(std::cin, line);
  inputValues = split(line, ",");
  return inputValues;
}

ll calculate_hash(std::string const &str) {
  return std::accumulate(std::cbegin(str), std::cend(str), 0ll,
                         [](auto const &base, auto const &c) {
                           return 17 * (base + static_cast<ll>(c)) % 256;
                         });
}

ll solve1(input_t const &aInputData) {
  return std::accumulate(std::cbegin(aInputData), std::cend(aInputData), 0ll,
                         [](auto const &base, auto const &elem) {
                           return base + calculate_hash(elem);
                         });
}

ll solve2(input_t const &aInputData) {
  std::vector<std::vector<std::pair<std::string, ll>>> memory(256);
  for (auto const &inst : aInputData) {
    auto equal_idx = inst.find('=');
    if (equal_idx != std::string::npos) {
      auto const parts = split(inst, "=");
      auto const hash = calculate_hash(parts[0]);
      auto &box = memory[hash];
      auto it =
          std::find_if(std::begin(box), std::end(box), [&](auto const &elem) {
            return elem.first == parts[0];
          });
      if (it == std::cend(box)) {
        box.emplace_back(parts[0], std::stoll(parts[1]));
      } else {
        it->second = std::stoll(parts[1]);
      }
    } else {
      auto minus_idx = inst.find('-');
      assert(minus_idx != std::string::npos);
      auto const variable = inst.substr(0, minus_idx);
      auto const hash = calculate_hash(variable);
      auto &box = memory[hash];
      auto it =
          std::find_if(std::cbegin(box), std::cend(box), [&](auto const &elem) {
            return elem.first == variable;
          });
      if (it != std::end(box)) {
        box.erase(it);
      }
    }
  }

  ll ii = 1;
  return std::accumulate(
      std::cbegin(memory), std::cend(memory), 0ll,
      [&](auto const &base, auto const &box) {
        ll i = 1;
        return base + ii++ * std::accumulate(std::cbegin(box), std::cend(box),
                                             0ll,
                                             [&](auto const &b, auto const &e) {
                                               return b + i++ * e.second;
                                             });
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