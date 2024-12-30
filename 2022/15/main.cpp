#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using ll = int64_t;

struct Point {
  ll x = 0;
  ll y = 0;

  Point() = default;
  Point(Point const &) = default;
  Point &operator=(Point const &other) = default;

  Point(ll xx, ll yy) : x(xx), y(yy) {}

  bool operator==(Point const &other) const {
    return std::tie(x, y) == std::tie(other.x, other.y);
  }
  bool operator<(Point const &other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
};

struct InputData {
  Point sensor;
  Point beacon;
  ll distance;
};

using input_t = std::vector<InputData>;

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

ll manhattan_distance(Point const &p1, Point const &p2) {
  return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    line.erase(remove(std::begin(line), std::end(line), ','), std::end(line));

    auto const elems = split(line, ' ');
    assert(elems.size() == 10);

    Point sensor;
    sensor.x = std::stoi(elems[2].substr(2));
    sensor.y = std::stoi(elems[3].substr(2, elems[3].size() - 3));

    Point beacon;
    beacon.x = std::stoi(elems[8].substr(2));
    beacon.y = std::stoi(elems[9].substr(2));

    inputValues.push_back(
        InputData{sensor, beacon, manhattan_distance(sensor, beacon)});
  }
  return inputValues;
}

bool is_in_range(std::vector<std::pair<ll, ll>> const &ranges, ll x) {
  return std::any_of(
      std::cbegin(ranges), std::cend(ranges),
      [=](auto const &range) { return range.first <= x && x <= range.second; });
}

ll solve1(input_t const &aInputData) {
  std::vector<std::pair<ll, ll>> ranges;
  auto constexpr LINE_VALUE = 2000000;
  std::for_each(std::cbegin(aInputData), std::cend(aInputData),
                [&](auto const &pair) {
                  auto const &sensor = pair.sensor;
                  auto const distance = pair.distance;

                  auto const diff = std::abs(sensor.y - LINE_VALUE);
                  if (0 <= diff && diff <= distance) {
                    auto const jlen = distance - diff;
                    ranges.emplace_back(sensor.x - jlen, sensor.x + jlen);
                  }
                });

  assert(ranges.size() > 1);
  std::sort(std::begin(ranges), std::end(ranges));
  auto current_range = *std::cbegin(ranges);
  std::vector<std::pair<ll, ll>> final_ranges;
  for (auto range_it = std::next(std::cbegin(ranges));
       range_it != std::cend(ranges); ++range_it) {
    if (current_range.first <= range_it->first &&
        range_it->second <= current_range.second) {
      continue;
    }
    if (range_it->first <= current_range.second &&
        current_range.second < range_it->second) {
      current_range.second = range_it->second;
      continue;
    }

    if (range_it->first > current_range.second) {
      final_ranges.push_back(current_range);
    }
  }
  final_ranges.push_back(current_range);

  std::set<ll> used_line_x;
  std::for_each(std::cbegin(aInputData), std::cend(aInputData),
                [&](auto const &p) {
                  if (p.sensor.y == LINE_VALUE) {
                    used_line_x.insert(p.sensor.x);
                  }
                  if (p.beacon.y == LINE_VALUE) {
                    used_line_x.insert(p.beacon.x);
                  }
                });
  auto const alreadyUsedPositions =
      std::accumulate(std::cbegin(used_line_x), std::cend(used_line_x), 0ll,
                      [&](auto const &base, auto const &x) {
                        return base + (is_in_range(final_ranges, x) ? 1 : 0);
                      });

  return std::accumulate(std::cbegin(final_ranges), std::cend(final_ranges),
                         0ll,
                         [](auto const &base, auto const &range) {
                           return base + (range.second - range.first + 1);
                         }) -
         alreadyUsedPositions;
}

ll calculate_inside_distance(input_t const &input, Point const &p) {
  return std::accumulate(
      std::cbegin(input), std::cend(input), 0ll,
      [&](auto const &base, auto const &i) {
        auto const d = manhattan_distance(p, i.sensor);
        return base + ((d > i.distance + 1) ? 0 : (i.distance - d + 1));
      });
}

ll solve2(input_t const &aInputData) {
  auto average = std::accumulate(
      std::cbegin(aInputData), std::cend(aInputData), Point{},
      [](auto const &base, auto const &pp) {
        return Point{base.x + pp.beacon.x, base.y + pp.beacon.y};
      });
  average.x /= aInputData.size();
  average.y /= aInputData.size();

  ll insideDistance = calculate_inside_distance(aInputData, average);
  std::deque<std::pair<Point, ll>> process{{average, insideDistance}};

  std::pair<Point, ll> p;
  std::set<Point> visited{average};
  constexpr ll limit = 4000000;

  while (!process.empty()) {
    p = process.front();
    process.pop_front();

    for (int stride = p.second; stride >= 0; stride /= 2) {
      if (stride == 0) {
        stride = 1;
      }
      bool found = false;
      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          if (i == 0 && j == 0) {
            continue;
          }

          Point np{p.first.x + stride * i, p.first.y + stride * j};

          if (np.x < 0 || np.x > limit || np.y < 0 || np.y > limit) {
            continue;
          }

          auto const visited_it = visited.find(np);
          if (visited_it != std::cend(visited)) {
            continue;
          }

          visited.insert(np);

          auto const newInsideDistance =
              calculate_inside_distance(aInputData, np);
          if (newInsideDistance <= p.second) {
            process.emplace_back(np, newInsideDistance);
            insideDistance = newInsideDistance;
            if (insideDistance == 0) {
              return np.x * limit + np.y;
            }
            found = true;
          }
        }
      }
      if (found) {
        break;
      } else if (stride == 1) {
        break;
      }
    }
  }

  return -1;
}

int main() {
  auto const inputData = readInput();

  // std::for_each(
  //     std::cbegin(inputData), std::cend(inputData), [](auto const &elem) {
  //       std::cout << elem.sensor.x << "," << elem.sensor.y << " -> "
  //                 << elem.beacon.x << "," << elem.beacon.y << std::endl;
  //     });
  // std::cout << std::endl;

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}