#include <algorithm>
#include <array>
#include <bits/stdc++.h>
#include <cassert>
#include <iterator>
#include <limits>
#include <numeric>
#include <utility>

using ll = int64_t;

struct BlueprintInfo {
  ll id;
  std::vector<std::array<int8_t, 4>> robots_cost;
};

using input_t = std::vector<BlueprintInfo>;

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
    line.erase(remove(std::begin(line), std::end(line), ':'), std::end(line));
    line.erase(remove(std::begin(line), std::end(line), '.'), std::end(line));
    auto const elems = split(line, ' ');

    auto const id = std::stoi(elems[1]);
    auto const ore_robot_cost = static_cast<int8_t>(std::stoi(elems[6]));
    auto const clay_robot_cost = static_cast<int8_t>(std::stoi(elems[12]));
    auto const obsidian_robot_cost1 = static_cast<int8_t>(std::stoi(elems[18]));
    auto const obsidian_robot_cost2 = static_cast<int8_t>(std::stoi(elems[21]));
    auto const geode_robot_cost1 = static_cast<int8_t>(std::stoi(elems[27]));
    auto const geode_robot_cost2 = static_cast<int8_t>(std::stoi(elems[30]));

    inputValues.push_back(BlueprintInfo{
        id, std::vector<std::array<int8_t, 4>>{
                {ore_robot_cost, 0, 0, 0},
                {clay_robot_cost, 0, 0, 0},
                {obsidian_robot_cost1, obsidian_robot_cost2, 0, 0},
                {geode_robot_cost1, 0, geode_robot_cost2, 0}}});
  }
  return inputValues;
}

struct ProcessData {
  uint8_t minutes = 0;
  std::array<uint8_t, 4> robots;
  std::array<uint8_t, 4> materials;

  bool operator<(ProcessData const &other) const {
    return std::tie(other.minutes, robots, materials) <
           std::tie(minutes, other.robots, other.materials);
  }
};

ll bfs(BlueprintInfo const &blueprint, ll const total_minutes) {
  std::deque<ProcessData> process{ProcessData{0, {1, 0, 0, 0}, {0, 0, 0, 0}}};
  std::set<ProcessData> visited;

  std::array<int8_t, 4> maximum{
      std::max(
          std::max(blueprint.robots_cost[0][0], blueprint.robots_cost[1][0]),
          std::max(blueprint.robots_cost[2][0], blueprint.robots_cost[3][0])),
      blueprint.robots_cost[2][1], blueprint.robots_cost[3][2],
      std::numeric_limits<int8_t>::max()};

  ll best_geode = 0;
  while (!process.empty()) {
    auto const p = process.front();
    process.pop_front();

    if (p.minutes > total_minutes) {
      continue;
    }

    if (p.minutes == total_minutes) {
      if (p.materials.back() > best_geode) {
        best_geode = p.materials.back();
      }
      continue;
    }

    auto const visited_pair = visited.insert(p);
    if (!visited_pair.second) {
      continue;
    }

    if (p.materials.back() + p.robots.back() * (total_minutes - p.minutes) >
        best_geode) {
      best_geode =
          p.materials.back() + p.robots.back() * (total_minutes - p.minutes);
    }

    bool skip = false;
    for (int robot_type = 3; robot_type >= 0; robot_type--) {
      if (p.robots[robot_type] >= maximum[robot_type]) {
        continue;
      }
      if (p.materials[robot_type] +
              (total_minutes - p.minutes) * p.robots[robot_type] >=
          maximum[robot_type] * (total_minutes - p.minutes)) {
        continue;
      }
      auto const &robot_cost = blueprint.robots_cost[robot_type];
      bool can_be_produced = true;
      for (int material_type = 0; material_type < 4; material_type++) {
        auto const &material = p.materials[material_type];

        if (material < robot_cost[material_type]) {
          can_be_produced = false;
          break;
        }
      }

      if (can_be_produced) {
        auto new_process_data = p;
        new_process_data.minutes++;
        for (int i = 0; i < 4; i++) {
          new_process_data.materials[i] -= robot_cost[i];
        }
        for (int i = 0; i < 4; i++) {
          new_process_data.materials[i] += new_process_data.robots[i];
        }
        new_process_data.robots[robot_type] += 1;
        auto const visited_it = visited.find(new_process_data);
        if (visited_it != std::cend(visited)) {
          continue;
        }
        // Produce material
        process.push_back(new_process_data);

        if (robot_type == 3) {
          skip = true;
          break;
        }
      }
    }

    if (!skip) {
      auto new_process_data = p;
      new_process_data.minutes++;
      for (int i = 0; i < 4; i++) {
        new_process_data.materials[i] += new_process_data.robots[i];
      }
      // Produce material
      auto const visited_it = visited.find(new_process_data);
      if (visited_it != std::cend(visited)) {
        continue;
      }
      process.push_back(new_process_data);
    }
  }

  return best_geode;
}

ll solve1(input_t const &aInputData) {
  ll constexpr total_minutes = 24;
  return std::accumulate(std::cbegin(aInputData), std::cend(aInputData), 0ll,
                         [&](auto const &base, auto const &blueprint) {
                           auto const best_geode =
                               bfs(blueprint, total_minutes);
                           return base + blueprint.id * best_geode;
                         });
}

ll solve2(input_t const &aInputData) {
  ll constexpr total_minutes = 32;
  return std::accumulate(
      std::cbegin(aInputData), std::next(std::cbegin(aInputData), 3), 1ll,
      [&](auto const &base, auto const &blueprint) {
        auto const best_geode = bfs(blueprint, total_minutes);
        return base * best_geode;
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