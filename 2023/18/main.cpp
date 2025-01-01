#include <algorithm>
#include <bits/stdc++.h>
#include <cmath>
#include <cstddef>
#include <deque>
#include <iterator>
#include <limits>
#include <numeric>
#include <optional>
#include <string>
#include <utility>
#include <vector>

using ll = int64_t;

struct point {
  ll x;
  ll y;

  point(ll xx, ll yy) : x(xx), y(yy) {}
  bool operator<(point const &other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
  bool operator==(point const &other) const {
    return std::tie(x, y) == std::tie(other.x, other.y);
  }
};

struct data {
  char iDirection;
  int iSize;
  unsigned long iColor;
};

using input_t = std::vector<data>;

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
    std::istringstream iss(line);
    auto const elem = split(line);
    assert(elem.size() == 3);

    inputValues.push_back(
        data{elem[0].front(), std::stoi(elem[1]),
             std::stoul(elem[2].substr(2, elem[2].size() - 3), 0, 16)});
  }
  return inputValues;
}

bool in_bounds(point const &p, point const &min, point const &max) {
  return min.x <= p.x && p.x <= max.x && min.y <= p.y && p.y <= max.y;
}

int inside_nodes(std::set<point> const &map, point const &min,
                 point const &max) {
  std::set<point> visited;
  std::set<point> full_visited;
  std::deque<point> to_process;

  std::set<point> inside;

  for (int x = min.x; x <= max.x; x++) {
    for (int y = min.y; y <= max.y; y++) {
      auto const n = point(x, y);
      if (map.find(n) == std::cend(map) &&
          full_visited.find(n) == std::cend(full_visited)) {

        bool in = true;

        to_process.clear();
        visited.clear();

        to_process.push_back(n);
        while (!to_process.empty()) {
          auto const node = to_process.front();
          to_process.pop_front();
          if (visited.find(node) != std::cend(visited)) {
            continue;
          }
          visited.emplace(node);
          auto const up = point(node.x, node.y - 1);
          auto const down = point(node.x, node.y + 1);
          auto const left = point(node.x - 1, node.y);
          auto const right = point(node.x + 1, node.y);
          if (in_bounds(up, min, max)) {
            if (map.find(up) == std::cend(map)) {
              to_process.push_back(up);
            }
          } else {
            in = false;
          }
          if (in_bounds(down, min, max)) {
            if (map.find(down) == std::cend(map)) {
              to_process.push_back(down);
            }
          } else {
            in = false;
          }
          if (in_bounds(left, min, max)) {
            if (map.find(left) == std::cend(map)) {
              to_process.push_back(left);
            }
          } else {
            in = false;
          }
          if (in_bounds(right, min, max)) {
            if (map.find(right) == std::cend(map)) {
              to_process.push_back(right);
            }
          } else {
            in = false;
          }
        }

        full_visited.insert(std::cbegin(visited), std::cend(visited));
        if (in) {
          inside.insert(std::cbegin(visited), std::cend(visited));
        }
      }
    }
  }

  // for (auto const &p : inside) {
  //   std::cout << p.x << " " << p.y << std::endl;
  // }

  return inside.size();
}

ll solve1(input_t const &aInputData) {
  int x = 0;
  int y = 0;

  std::set<point> map;

  for (auto const &movement : aInputData) {
    for (int i = 1; i <= movement.iSize; i++) {
      switch (movement.iDirection) {
      case 'R':
        map.emplace(x + i, y);
        break;
      case 'D':
        map.emplace(x, y + i);
        break;
      case 'L':
        map.emplace(x - i, y);
        break;
      case 'U':
        map.emplace(x, y - i);
        break;
      }
    }
    switch (movement.iDirection) {
    case 'R':
      x += movement.iSize;
      break;
    case 'D':
      y += movement.iSize;
      break;
    case 'L':
      x -= movement.iSize;
      break;
    case 'U':
      y -= movement.iSize;
      break;
    }
  }
  point min{std::numeric_limits<int>::max(), std::numeric_limits<int>::max()};
  point max{std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};
  for (auto const &cell : map) {
    min.x = std::min(min.x, cell.x);
    max.x = std::max(max.x, cell.x);
    min.y = std::min(min.y, cell.y);
    max.y = std::max(max.y, cell.y);
  }
  auto const border = map.size();
  int inside = inside_nodes(map, min, max);
  return border + inside;
}

// https://www.geeksforgeeks.org/area-of-a-polygon-with-given-n-ordered-vertices/
ll calculate_area_polygon(std::vector<point> const &points, ll min_x) {
  // Initialize area
  double area = 0.0;

  // Calculate value of shoelace formula
  int j = points.size() - 1;
  for (int i = 0; i < points.size(); i++) {
    area += (points[j].x + points[i].x) * (points[j].y - points[i].y);
    j = i; // j is previous vertex to i
  }

  // Return absolute value
  return std::abs(area / 2.0);
}

// https://en.wikipedia.org/wiki/Pick%27s_theorem
ll calculate_inside_points(ll area, ll boundary) {
  const auto inside = area + 1 - boundary / 2;
  return inside + boundary;
}

ll solve2(input_t const &aInputData) {
  ll x = 0;
  ll y = 0;

  std::vector<point> vertex;
  ll perimeter = 0;
  ll min_x = std::numeric_limits<ll>::max();

  // for (size_t i = 0; i < aInputData.size(); i++) {
  //   auto const &movement = aInputData[i];
  //   switch (movement.iDirection) {
  //   case 'R':
  //     x += movement.iSize;
  //     break;
  //   case 'D':
  //     y += movement.iSize;
  //     break;
  //   case 'L':
  //     x -= movement.iSize;
  //     break;
  //   case 'U':
  //     y -= movement.iSize;
  //     break;
  //   }
  //   vertex.push_back(point{x, y});
  //   min_x = std::min(min_x, x);
  //   perimeter += movement.iSize;
  // }

  vertex.clear();
  for (auto const &movement : aInputData) {
    switch (movement.iColor & 0xf) {
    case 0:
      // std::cout << "R " << (movement.iColor >> 4) << std::endl;
      y += movement.iColor >> 4;
      break;
    case 1:
      // std::cout << "D " << (movement.iColor >> 4) << std::endl;
      x += movement.iColor >> 4;
      break;
    case 2:
      // std::cout << "L " << (movement.iColor >> 4) << std::endl;
      y -= movement.iColor >> 4;
      break;
    case 3:
      // std::cout << "U " << (movement.iColor >> 4) << std::endl;
      x -= movement.iColor >> 4;
      break;
    }
    vertex.push_back(point{x, y});
    min_x = std::min(min_x, x);
    perimeter += (movement.iColor >> 4);
  }

  const auto area = calculate_area_polygon(vertex, min_x);
  for (point const &p : vertex) {
    std::cout << "(" << p.x << "," << p.y << "),";
  }
  std::cout << std::endl;

  std::cout << "perimeter: " << perimeter << std::endl;
  std::cout << "area: " << area << std::endl;
  return calculate_inside_points(area, perimeter);
}

int main() {
  auto const inputData = readInput();

  std::for_each(std::cbegin(inputData), std::cend(inputData),
                [](auto const &elem) {
                  std::cout << elem.iDirection << " " << elem.iSize << ",";
                });
  std::cout << std::endl;

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}