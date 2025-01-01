#include <algorithm>
#include <bits/stdc++.h>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;
using input_t = std::vector<std::vector<ll>>;

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
  while (std::getline(std::cin, line)) {
    auto const values_str = split(line, " ");
    std::vector<ll> values;
    values.reserve(values_str.size());
    std::transform(std::cbegin(values_str), std::cend(values_str),
                   std::back_inserter(values),
                   [](auto const &elem) { return std::stoll(elem); });
    inputValues.push_back(values);
  }
  return inputValues;
}

ll solve1(input_t const &aInputData) {
  return std::accumulate(
      std::cbegin(aInputData), std::cend(aInputData), 0ll,
      [](const auto &base, const auto &r) {
        auto row = r;
        std::vector<ll> next_row;
        auto all_zeros =
            std::all_of(std::cbegin(row), std::cend(row),
                        [](auto const &elem) { return elem == 0; });
        std::vector<ll> last_elem;
        while (!all_zeros) {
          assert(row.size() > 0);
          next_row.reserve(row.size() - 1);

          for (size_t i = 0; i < row.size() - 1; i++) {
            next_row.push_back(row[i + 1] - row[i]);
          }
          next_row.swap(row);
          next_row.clear();
          all_zeros = std::all_of(std::cbegin(row), std::cend(row),
                                  [](auto const &elem) { return elem == 0; });
          if (!all_zeros) {
            last_elem.push_back(row.back());
          }
        }

        auto const next_elem =
            std::accumulate(
                std::cbegin(last_elem), std::cend(last_elem), 0ll,
                [](const auto &b, const auto &elem) { return b + elem; }) +
            r.back();

        return base + next_elem;
      });
}

ll solve2(input_t const &aInputData) {
  return std::accumulate(
      std::cbegin(aInputData), std::cend(aInputData), 0ll,
      [](const auto &base, const auto &r) {
        auto row = r;
        ll next_elem = 0;
        std::vector<ll> next_row;
        auto all_zeros =
            std::all_of(std::cbegin(row), std::cend(row),
                        [](auto const &elem) { return elem == 0; });
        std::vector<ll> first_elem;
        while (!all_zeros) {
          assert(row.size() > 0);
          next_row.reserve(row.size() - 1);

          for (size_t i = 0; i < row.size() - 1; i++) {
            next_row.push_back(row[i + 1] - row[i]);
          }
          next_row.swap(row);
          next_row.clear();
          all_zeros = std::all_of(std::cbegin(row), std::cend(row),
                                  [](auto const &elem) { return elem == 0; });
          if (!all_zeros) {
            first_elem.push_back(row.front());
          }
        }

        next_elem =
            r.front() - std::accumulate(std::crbegin(first_elem),
                                        std::crend(first_elem), 0ll,
                                        [](const auto &b, const auto &elem) {
                                          auto v = elem - b;
                                          return v;
                                        });

        return base + next_elem;
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