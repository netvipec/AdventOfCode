#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <ratio>
#include <string>
#include <utility>

using ll = int64_t;
using input_t = std::vector<std::string>;

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      break;
    }
    inputValues.push_back(line);
  }
  return inputValues;
}

ll SNAFU_to_decimal(std::string const &str) {
  return std::accumulate(std::cbegin(str), std::cend(str), 0ll,
                         [&](auto const &base, auto const &elem) {
                           ll n = 0;
                           switch (elem) {
                           case '2':
                           case '1':
                           case '0':
                             n = elem - '0';
                             break;
                           case '-':
                             n = -1;
                             break;
                           case '=':
                             n = -2;
                             break;
                           default:
                             assert(false);
                           }
                           auto const r = 5 * base + n;
                           return r;
                         });
}

char snafu_digit(ll d) {
  assert(-2 <= d && d <= 2);
  switch (d) {
  case -2:
    return '=';
  case -1:
    return '-';
  case 0:
    return '0';
  case 1:
    return '1';
  case 2:
    return '2';
  default:
    assert(false);
    return ' ';
  }
}

std::string decimal_to_SNAFU(ll const n) {
  ll power5 = 1;
  ll count = 0;
  std::vector<ll> d{0};
  while (power5 < n) {
    d.push_back(d.back() + power5 * 2);
    power5 *= 5;
    count++;
  }
  if (std::abs(n - power5) > d[count]) {
    power5 /= 5;
    count--;
  }

  std::string snafu;
  ll nn = n;
  for (ll power = power5; power >= 1; power /= 5) {
    auto digit = nn / power;
    auto const rest = nn - digit * power;
    if (std::abs(rest) > d[count]) {
      if (rest > 0) {
        digit++;
      } else {
        digit--;
      }
    }
    if (digit > 2) {
      digit = 2;
    }
    if (digit < -2) {
      digit = -2;
    }
    nn = nn - digit * power;
    snafu.push_back(snafu_digit(digit));
    count--;
  }
  return snafu;
}

std::string solve1(input_t const &aInputData) {
  return decimal_to_SNAFU(
      std::accumulate(std::cbegin(aInputData), std::cend(aInputData), 0ll,
                      [](auto const &base, auto const &elem) {
                        return base + SNAFU_to_decimal(elem);
                      }));
}

ll solve2(input_t const &aInputData) {
  // note
  return 0;
}

int main() {
  auto const inputData = readInput();

  // std::cout << decimal_to_SNAFU(1) << std::endl;
  // std::cout << decimal_to_SNAFU(2) << std::endl;
  // std::cout << decimal_to_SNAFU(3) << std::endl;
  // std::cout << decimal_to_SNAFU(4) << std::endl;
  // std::cout << decimal_to_SNAFU(5) << std::endl;
  // std::cout << decimal_to_SNAFU(6) << std::endl;
  // std::cout << decimal_to_SNAFU(7) << std::endl;
  // std::cout << decimal_to_SNAFU(8) << std::endl;
  // std::cout << decimal_to_SNAFU(9) << std::endl;
  // std::cout << decimal_to_SNAFU(10) << std::endl;
  // std::cout << decimal_to_SNAFU(15) << std::endl;
  // std::cout << decimal_to_SNAFU(20) << std::endl;
  // std::cout << decimal_to_SNAFU(2022) << std::endl;
  // std::cout << decimal_to_SNAFU(12345) << std::endl;
  // std::cout << decimal_to_SNAFU(1747) << std::endl;
  // std::cout << decimal_to_SNAFU(906) << std::endl;
  // std::cout << decimal_to_SNAFU(198) << std::endl;
  // std::cout << decimal_to_SNAFU(11) << std::endl;
  // std::cout << decimal_to_SNAFU(201) << std::endl;
  // std::cout << decimal_to_SNAFU(31) << std::endl;
  // std::cout << decimal_to_SNAFU(1257) << std::endl;
  // std::cout << decimal_to_SNAFU(32) << std::endl;
  // std::cout << decimal_to_SNAFU(353) << std::endl;
  // std::cout << decimal_to_SNAFU(107) << std::endl;
  // std::cout << decimal_to_SNAFU(37) << std::endl;
  // std::cout << decimal_to_SNAFU(314159265) << std::endl;

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}