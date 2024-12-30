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
  ll z = 0;

  bool operator<(Point const &other) const {
    return std::tie(x, y, z) < std::tie(other.x, other.y, other.z);
  }
};

using input_t = std::set<Point>;

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

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    auto const elems = split(line, ',');
    auto const x = std::stoi(elems[0]);
    auto const y = std::stoi(elems[1]);
    auto const z = std::stoi(elems[2]);
    inputValues.insert(Point{x, y, z});
  }
  return inputValues;
}

std::vector<Point> get_neighbors(Point const &point) {
  return std::vector<Point>{Point{point.x + 1, point.y, point.z},
                            Point{point.x - 1, point.y, point.z},
                            Point{point.x, point.y + 1, point.z},
                            Point{point.x, point.y - 1, point.z},
                            Point{point.x, point.y, point.z + 1},
                            Point{point.x, point.y, point.z - 1}};
}

ll solve1(input_t const &aInputData) {
  return std::accumulate(std::cbegin(aInputData), std::cend(aInputData), 0ll,
                         [&](auto const &base, auto const &point) {
                           auto const neighbors = get_neighbors(point);
                           auto const free_neighbor = std::count_if(
                               std::cbegin(neighbors), std::cend(neighbors),
                               [&](auto const &neighbor) {
                                 return aInputData.find(neighbor) ==
                                        std::cend(aInputData);
                               });
                           return base + free_neighbor;
                         });
}

template <typename T>
std::set<Point> dfs(Point const &start, input_t const &blocked,
                    T const &minmax_x, T const &minmax_y, T const &minmax_z) {
  std::set<Point> visited{start};
  std::deque<Point> process{start};
  while (!process.empty()) {
    auto const p = process.front();
    process.pop_front();

    if (p.x <= minmax_x.first->x || p.x >= minmax_x.second->x ||
        p.y <= minmax_y.first->y || p.y >= minmax_y.second->y ||
        p.z <= minmax_z.first->z || p.z >= minmax_z.second->z) {
      return std::set<Point>{};
    }

    auto const neighbors = get_neighbors(p);
    std::for_each(std::cbegin(neighbors), std::cend(neighbors),
                  [&](auto const &neighbor) {
                    auto const blocked_it = blocked.find(neighbor);
                    if (blocked_it != std::cend(blocked)) {
                      return;
                    }

                    auto const visited_it = visited.insert(neighbor);
                    if (!visited_it.second) {
                      return;
                    }

                    process.push_back(neighbor);
                  });
  }
  return visited;
}

ll solve2(input_t const &aInputData) {
  std::set<Point> surface;

  auto const total_surface_are = std::accumulate(
      std::cbegin(aInputData), std::cend(aInputData), 0ll,
      [&](auto const &base, auto const &point) {
        auto const neighbors = get_neighbors(point);
        auto const free_neighbor =
            std::count_if(std::cbegin(neighbors), std::cend(neighbors),
                          [&](auto const &neighbor) {
                            auto const free_side = aInputData.find(neighbor) ==
                                                   std::cend(aInputData);
                            if (free_side) {
                              surface.insert(neighbor);
                            }
                            return free_side;
                          });
        return base + free_neighbor;
      });

  auto const minmax_x = std::minmax_element(
      std::cbegin(aInputData), std::cend(aInputData),
      [](auto const &lhs, auto const &rhs) { return lhs.x < rhs.x; });
  auto const minmax_y = std::minmax_element(
      std::cbegin(aInputData), std::cend(aInputData),
      [](auto const &lhs, auto const &rhs) { return lhs.y < rhs.y; });
  auto const minmax_z = std::minmax_element(
      std::cbegin(aInputData), std::cend(aInputData),
      [](auto const &lhs, auto const &rhs) { return lhs.z < rhs.z; });

  std::set<Point> inside_pockets;
  std::for_each(
      std::cbegin(surface), std::cend(surface), [&](auto const &point) {
        auto const visited =
            dfs(point, aInputData, minmax_x, minmax_y, minmax_z);
        inside_pockets.insert(std::cbegin(visited), std::cend(visited));
      });

  return total_surface_are -
         std::accumulate(std::cbegin(inside_pockets), std::cend(inside_pockets),
                         0ll, [&](auto const &base, auto const &point) {
                           auto const neighbors = get_neighbors(point);
                           auto const inside_neighbor = std::count_if(
                               std::cbegin(neighbors), std::cend(neighbors),
                               [&](auto const &neighbor) {
                                 return aInputData.find(neighbor) !=
                                        std::cend(aInputData);
                               });
                           return base + inside_neighbor;
                         });
  ;
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}