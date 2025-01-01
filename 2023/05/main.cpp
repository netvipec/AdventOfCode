#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <string>
#include <utility>

using ll = int64_t;

struct map_range {
  ll destination;
  ll source;
  ll length;

  bool operator<(const map_range &other) const {
    return source + length < other.source + other.length;
  }
};

struct data {
  std::vector<ll> seeds;
  std::map<std::string, std::pair<std::string, std::vector<map_range>>>
      conversion;
};
using input_t = data;

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
  const auto seeds_line = split(line, ": ");
  assert(seeds_line.size() == 2);
  const auto seeds_numbers_part = split(seeds_line[1], " ");
  std::transform(std::cbegin(seeds_numbers_part), std::cend(seeds_numbers_part),
                 std::back_inserter(inputValues.seeds),
                 [](auto const &elem) { return std::stoll(elem); });
  std::getline(std::cin, line);
  while (std::getline(std::cin, line)) {
    const auto categories = split(line, " ");
    assert(categories.size() == 2);
    const auto categories_part = split(categories[0], "-");
    assert(categories_part.size() == 3);
    const auto origin = categories_part[0];
    const auto destination = categories_part[2];

    inputValues.conversion[origin] =
        std::make_pair(destination, std::vector<map_range>{});
    while (std::getline(std::cin, line)) {
      if (line.empty()) {
        break;
      }
      std::istringstream iss(line);
      ll destination_value, origin_value, length;
      if (!(iss >> destination_value >> origin_value >> length)) {
        break;
      } // error

      inputValues.conversion[origin].second.push_back(
          map_range{destination_value, origin_value, length});
    }

    std::sort(std::begin(inputValues.conversion[origin].second),
              std::end(inputValues.conversion[origin].second),
              [](auto const &lhs, auto const &rhs) {
                return lhs.source < rhs.source;
              });
  }
  return inputValues;
}

ll solve1(input_t const &aInputData) {
  std::vector<ll> locations;
  std::transform(
      std::cbegin(aInputData.seeds), std::cend(aInputData.seeds),
      std::back_inserter(locations), [&](const auto &seed) {
        ll origin_value = seed;
        std::string origin_str = "seed";
        while (origin_str != "location") {
          auto const conversion_it = aInputData.conversion.find(origin_str);
          assert(conversion_it != std::cend(aInputData.conversion));
          origin_str = conversion_it->second.first;

          auto origin_new_value_it = std::find_if(
              std::cbegin(conversion_it->second.second),
              std::cend(conversion_it->second.second), [&](const auto &elem) {
                return elem.source <= origin_value &&
                       origin_value < elem.source + elem.length;
              });

          if (origin_new_value_it != std::cend(conversion_it->second.second)) {
            origin_value = origin_new_value_it->destination +
                           (origin_value - origin_new_value_it->source);
          }
        }
        return origin_value;
      });
  auto const it =
      std::min_element(std::cbegin(locations), std::cend(locations));
  return *it;
}

ll solve2(input_t const &aInputData) {
  std::vector<map_range> old_locations;
  for (size_t i = 0; i < aInputData.seeds.size(); i += 2) {
    old_locations.push_back(
        map_range{0, aInputData.seeds[i], aInputData.seeds[i + 1]});
  }
  std::vector<map_range> new_locations;
  std::string origin_str = "seed";
  while (origin_str != "location") {
    auto const conversion_it = aInputData.conversion.find(origin_str);
    assert(conversion_it != std::cend(aInputData.conversion));
    origin_str = conversion_it->second.first;
    auto const &conversion_list = conversion_it->second.second;

    for (auto const &seed : old_locations) {
      auto actual_origin_value = seed.source;
      while (actual_origin_value < seed.source + seed.length) {
        auto new_seed =
            map_range{0, actual_origin_value,
                      seed.source + seed.length - actual_origin_value};
        auto origin_new_value_it = std::upper_bound(
            std::cbegin(conversion_list), std::cend(conversion_list),
            map_range{0, actual_origin_value, 0});

        if (origin_new_value_it == std::cend(conversion_list)) {
          new_locations.push_back(
              map_range{0, actual_origin_value, new_seed.length});
          actual_origin_value = new_seed.source + new_seed.length;
        } else {
          if (actual_origin_value < origin_new_value_it->source) {
            new_locations.push_back(map_range{
                0, actual_origin_value,
                std::min(origin_new_value_it->source - actual_origin_value,
                         new_seed.length)});
          } else {
            new_locations.push_back(map_range{
                0,
                actual_origin_value - origin_new_value_it->source +
                    origin_new_value_it->destination,
                std::min(origin_new_value_it->source +
                             origin_new_value_it->length - actual_origin_value,
                         new_seed.length)});
          }
          actual_origin_value += new_locations.back().length;
        }
      }
    }

    old_locations.swap(new_locations);
    new_locations.clear();
  }
  auto const it = std::min_element(
      std::cbegin(old_locations), std::cend(old_locations),
      [](const auto &lhs, const auto &rhs) { return lhs.source < rhs.source; });
  return it->source;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}