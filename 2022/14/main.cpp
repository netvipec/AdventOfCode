#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <unordered_set>
#include <utility>

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

namespace std {

template <> struct hash<Point> {
  std::size_t operator()(const Point &k) const {
    using std::hash;
    using std::size_t;

    return std::hash<ll>()(k.x) ^ std::hash<ll>()(k.y);
    ;
  }
};

} // namespace std

using input_t = std::unordered_set<Point>;

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

// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](int ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

void insert_point_lines(input_t &input, Point const &start, Point const &end) {
  if (start.x == end.x) {
    auto const start_y = std::min(start.y, end.y);
    auto const end_y = std::max(start.y, end.y);
    for (ll i = start_y; i <= end_y; i++) {
      input.emplace(start.x, i);
    }
  } else {
    assert(start.y == end.y);

    auto const start_x = std::min(start.x, end.x);
    auto const end_x = std::max(start.x, end.x);
    for (ll i = start_x; i <= end_x; i++) {
      input.emplace(i, start.y);
    }
  }
}

input_t readInput() {
  input_t inputValues;

  std::vector<Point> points;

  std::string line;
  while (std::getline(std::cin, line)) {
    auto const pointsElem = split(line, " -> ");
    points.clear();
    std::transform(std::cbegin(pointsElem), std::cend(pointsElem),
                   std::back_inserter(points), [](auto const &point) {
                     auto const elem = split(point, ',');
                     assert(elem.size() == 2);
                     auto const x = std::stoi(elem[0]);
                     auto const y = std::stoi(elem[1]);
                     return Point{x, y};
                   });

    assert(points.size() > 1);
    for (int i = 1; i < static_cast<ll>(points.size()); i++) {
      insert_point_lines(inputValues, points[i - 1], points[i]);
    }
  }
  return inputValues;
}

void print(input_t const &map, input_t const &rocks) {
  auto min_x_cmp = [](auto const &lhs, auto const &rhs) {
    return lhs.x < rhs.x;
  };
  auto min_y_cmp = [](auto const &lhs, auto const &rhs) {
    return lhs.y < rhs.y;
  };
  auto const min_x =
      std::min_element(std::cbegin(map), std::cend(map), min_x_cmp)->x - 2;
  auto const max_x =
      std::max_element(std::cbegin(map), std::cend(map), min_x_cmp)->x + 2;
  auto const min_y =
      std::min_element(std::cbegin(map), std::cend(map), min_y_cmp)->y - 2;
  auto const max_y =
      std::max_element(std::cbegin(map), std::cend(map), min_y_cmp)->y + 2;

  for (int j = min_y; j <= max_y; j++) {
    for (int i = min_x; i <= max_x; i++) {
      auto const rocks_it = rocks.find(Point{i, j});
      if (rocks_it != std::cend(rocks)) {
        std::cout << "#";
      } else {
        auto const it = map.find(Point{i, j});
        if (it == std::cend(map)) {
          std::cout << ".";
        } else {
          std::cout << "o";
        }
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

bool is_blocked(input_t const &map, Point const &p) {
  return map.find(p) != std::cend(map);
}

bool sand_drop_placement(input_t &map, ll max_y, Point const &drop_point,
                         Point &placement_point) {
  placement_point = drop_point;
  bool move_it = true;
  do {
    if (!is_blocked(map, Point{placement_point.x, placement_point.y + 1})) {
      ++placement_point.y;
    } else if (!is_blocked(
                   map, Point{placement_point.x - 1, placement_point.y + 1})) {
      ++placement_point.y;
      --placement_point.x;
    } else if (!is_blocked(
                   map, Point{placement_point.x + 1, placement_point.y + 1})) {
      ++placement_point.y;
      ++placement_point.x;
    } else {
      move_it = false;
    }

    if (placement_point.y >= max_y) {
      return false;
    }
  } while (move_it);

  // std::cout << placement_point.x << ", " << placement_point.y << std::endl;
  return true;
}

ll solve1(input_t const &aInputData) {
  ll max_y = std::max_element(
                 std::cbegin(aInputData), std::cend(aInputData),
                 [](auto const &lhs, auto const &rhs) { return lhs.y < rhs.y; })
                 ->y;

  auto map = aInputData;
  ll count = 0;
  Point drop_point(500, 0);
  Point placement_point;
  while (sand_drop_placement(map, max_y, drop_point, placement_point)) {
    ++count;
    map.insert(placement_point);
  }
  return count;
}

bool is_blocked2(input_t const &map, Point const &p, ll max_y) {
  return map.find(p) != std::cend(map) || p.y == max_y;
}

bool sand_drop_placement2(input_t &map, ll max_y, Point const &drop_point,
                          Point &placement_point) {
  placement_point = drop_point;
  bool move_it = true;
  do {
    if (!is_blocked2(map, Point{placement_point.x, placement_point.y + 1},
                     max_y)) {
      ++placement_point.y;
    } else if (!is_blocked2(map,
                            Point{placement_point.x - 1, placement_point.y + 1},
                            max_y)) {
      ++placement_point.y;
      --placement_point.x;
    } else if (!is_blocked2(map,
                            Point{placement_point.x + 1, placement_point.y + 1},
                            max_y)) {
      ++placement_point.y;
      ++placement_point.x;
    } else {
      move_it = false;
    }
  } while (move_it);

  // std::cout << placement_point.x << ", " << placement_point.y << std::endl;
  return true;
}

ll solve2(input_t const &aInputData) {
  ll max_y = std::max_element(
                 std::cbegin(aInputData), std::cend(aInputData),
                 [](auto const &lhs, auto const &rhs) { return lhs.y < rhs.y; })
                 ->y +
             2;

  auto map = aInputData;
  ll count = 0;
  Point drop_point(500, 0);
  Point placement_point;
  while (sand_drop_placement2(map, max_y, drop_point, placement_point)) {
    ++count;
    map.insert(placement_point);
    if (placement_point == drop_point) {
      break;
    }
    // print(map, aInputData);
  }

  // print(map, aInputData);
  return count;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}