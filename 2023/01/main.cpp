#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::string>;
const std::vector<std::string> k_digit_str = {"zero",  "one",  "two", "three",
                                              "four",  "five", "six", "seven",
                                              "eight", "nine"};

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    inputValues.push_back(line);
  }
  return inputValues;
}

bool is_number(const char &a_char) { return '0' <= a_char && a_char <= '9'; }

ll solve1(input_t const &aInputData) {
  return std::accumulate(
      std::cbegin(aInputData), std::cend(aInputData), 0,
      [](const auto &base, const auto &elem) {
        auto const first_it =
            std::find_if(std::cbegin(elem), std::cend(elem), is_number);
        auto const last_it =
            std::find_if(std::crbegin(elem), std::crend(elem), is_number);
        return base + (*first_it - '0') * 10 + (*last_it - '0');
      });
}

std::vector<std::string>::const_iterator find_digit(const std::string &line,
                                                    int idx) {
  if (is_number(line[idx])) {
    return std::cbegin(k_digit_str) + (line[idx] - '0');
  }
  const auto it = std::find_if(
      std::cbegin(k_digit_str), std::cend(k_digit_str), [&](auto const &elem) {
        return (idx + elem.size() <= line.size() &&
                std::equal(std::cbegin(elem), std::cend(elem),
                           std::cbegin(line) + idx));
      });
  if (it != std::cend(k_digit_str)) {
    return it;
  }
  return std::cend(k_digit_str);
}

ll find_calibration(const std::string &line) {
  auto first = [&]() {
    for (int idx = 0; idx < static_cast<int>(line.size()); idx++) {
      auto it = find_digit(line, idx);
      if (it != std::cend(k_digit_str)) {
        return it;
      }
    }
    return std::cend(k_digit_str);
  }();
  auto last = [&]() {
    for (int idx = line.size() - 1; idx >= 0; idx--) {
      auto it = find_digit(line, idx);
      if (it != std::cend(k_digit_str)) {
        return it;
      }
    }
    return std::cend(k_digit_str);
  }();
  return std::distance(std::cbegin(k_digit_str), first) * 10 +
         std::distance(std::cbegin(k_digit_str), last);
}

ll solve2(input_t const &aInputData) {
  return std::accumulate(std::cbegin(aInputData), std::cend(aInputData), 0,
                         [&](const auto &base, const auto &elem) {
                           return base + find_calibration(elem);
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