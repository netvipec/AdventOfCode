#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using ll = int64_t;

struct ValveInfo {
  std::vector<std::string> names;
  std::vector<ll> flow_rates;
  std::vector<std::vector<ll>> connections;
};

struct GraphData {
  std::vector<std::vector<int>> edges;
  std::vector<ll> flow_rates;
  std::vector<ll> names_index;
};

using input_t = ValveInfo;

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

ll find_path_length(ll valve_from, ll valve_to, input_t const &input) {
  std::unordered_set<ll> visited{valve_from};
  std::deque<std::pair<ll, ll>> process{{valve_from, 0}};
  while (!process.empty()) {
    auto const p = process.front();
    process.pop_front();

    if (p.first == valve_to) {
      return p.second;
    }

    std::for_each(std::cbegin(input.connections[p.first]),
                  std::cend(input.connections[p.first]),
                  [&](auto const &connection_index) {
                    auto const visited_pair = visited.insert(connection_index);
                    if (!visited_pair.second) {
                      return;
                    }

                    process.emplace_back(connection_index, p.second + 1);
                  });
  }

  return -1;
}

GraphData reduce_graph(input_t const &input) {
  GraphData graph_data;

  for (int i = 0; i < static_cast<int>(input.flow_rates.size()); i++) {
    if (input.flow_rates[i] > 0) {
      graph_data.names_index.push_back(i);
      graph_data.flow_rates.push_back(input.flow_rates[i]);
    }
  }

  graph_data.edges.resize(graph_data.names_index.size(),
                          std::vector<int>(graph_data.names_index.size(), 0));
  for (int i = 0; i < static_cast<int>(graph_data.names_index.size()) - 1;
       i++) {
    for (int j = i + 1; j < static_cast<int>(graph_data.names_index.size());
         j++) {
      auto const path_length1 = find_path_length(
          graph_data.names_index[i], graph_data.names_index[j], input);
      auto const path_length2 = find_path_length(
          graph_data.names_index[j], graph_data.names_index[i], input);
      graph_data.edges[i][j] = path_length1;
      graph_data.edges[j][i] = path_length2;
    }
  }

  return graph_data;
}

input_t readInput() {
  input_t inputValues;

  std::vector<std::vector<std::string>> connections;

  std::string line;
  while (std::getline(std::cin, line)) {
    line.erase(remove(std::begin(line), std::end(line), ','), std::end(line));
    line.erase(remove(std::begin(line), std::end(line), ';'), std::end(line));
    auto const elems = split(line, ' ');
    assert(elems.size() > 9);
    auto const valve = elems[1];
    auto const flow_rate = std::stoi(elems[4].substr(5));
    std::vector<std::string> new_valves(elems.size() - 9);
    std::copy(std::next(std::cbegin(elems), 9), std::cend(elems),
              std::begin(new_valves));

    inputValues.names.push_back(valve);
    inputValues.flow_rates.push_back(flow_rate);
    connections.push_back(new_valves);
  }

  inputValues.connections.resize(connections.size());

  for (int i = 0; i < static_cast<int>(connections.size()); i++) {
    std::transform(std::cbegin(connections[i]), std::cend(connections[i]),
                   std::back_inserter(inputValues.connections[i]),
                   [&](auto const &connection_name) {
                     return std::distance(
                         std::cbegin(inputValues.names),
                         std::find_if(std::cbegin(inputValues.names),
                                      std::cend(inputValues.names),
                                      [&](auto const &valve_name) {
                                        return valve_name == connection_name;
                                      }));
                   });
  }

  return inputValues;
}

struct ValveData {
  ll index = 0;
  ll minutes = 0;
  ll pressure = 0;
  std::vector<bool> open_valves;

  bool operator<(ValveData const &other) const {
    return std::tie(pressure, other.minutes, index) <
           std::tie(other.pressure, minutes, other.index);
  }
};

ll dfs(input_t const &aInputData, GraphData const &graph,
       ll const total_minutes, std::vector<bool> const &unaccessible_valves) {
  std::deque<ValveData> process;

  auto const valve_index = std::distance(
      std::cbegin(aInputData.names),
      std::find_if(std::cbegin(aInputData.names), std::cend(aInputData.names),
                   [&](auto const &valve_name) { return valve_name == "AA"; }));
  for (int i = 0; i < static_cast<int>(graph.names_index.size()); i++) {
    if (unaccessible_valves[i]) {
      continue;
    }
    auto const path_length =
        find_path_length(valve_index, graph.names_index[i], aInputData);
    std::vector<bool> open_valves(graph.names_index.size(), false);
    open_valves[i] = true;
    process.push_back(ValveData{i, path_length + 1, 0, open_valves});
  }

  ll best_pressure = 0;
  while (!process.empty()) {
    auto const p = process.front();
    process.pop_front();

    if (p.minutes > total_minutes) {
      continue;
    }
    if (p.minutes == total_minutes) {
      if (p.pressure > best_pressure) {
        // std::cout << "Best pressure: " << p.pressure << std::endl;
        best_pressure = p.pressure;
      }
    }

    ll pressure = 0;
    for (int i = 0; i < static_cast<int>(p.open_valves.size()); i++) {
      if (p.open_valves[i]) {
        pressure += graph.flow_rates[i];
      }
    }
    auto const final_pressure =
        p.pressure + pressure * (total_minutes - p.minutes);
    if (final_pressure > best_pressure) {
      // std::cout << "Best pressure: " << final_pressure << std::endl;
      best_pressure = final_pressure;
    }

    auto const ovc1 =
        std::count(std::cbegin(p.open_valves), std::cend(p.open_valves), true);
    auto const ovc2 = std::count(std::cbegin(unaccessible_valves),
                                 std::cend(unaccessible_valves), true);
    if (ovc1 + ovc2 == static_cast<int>(p.open_valves.size())) {
      continue;
    }

    for (int j = 0; j < static_cast<int>(graph.names_index.size()); j++) {
      if (j == p.index) {
        continue;
      }

      if (!p.open_valves[j] && !unaccessible_valves[j]) {
        auto new_open_valves = p.open_valves;
        new_open_valves[j] = true;
        process.push_back(
            ValveData{j, p.minutes + graph.edges[p.index][j] + 1,
                      p.pressure + pressure * (graph.edges[p.index][j] + 1),
                      new_open_valves});
      }
    }
  }

  return best_pressure;
}

ll solve1(input_t const &aInputData, GraphData const &graph) {
  constexpr ll total_minutes = 30;
  return dfs(aInputData, graph, total_minutes,
             std::vector<bool>(graph.names_index.size(), false));
}

struct ValveData2 {
  ll index = 0;
  ll minutes = 0;
  ll pressure = 0;
  std::vector<bool> open_valves;

  bool operator<(ValveData2 const &other) const {
    return std::tie(pressure, other.minutes, index) <
           std::tie(other.pressure, minutes, other.index);
  }
};

std::unordered_map<std::vector<bool>, ll> cache;

ll solve2(input_t const &aInputData, GraphData const &graph) {
  constexpr ll total_minutes = 26;
  std::deque<ValveData2> process;

  auto const valve_index = std::distance(
      std::cbegin(aInputData.names),
      std::find_if(std::cbegin(aInputData.names), std::cend(aInputData.names),
                   [&](auto const &valve_name) { return valve_name == "AA"; }));
  for (int i = 0; i < static_cast<int>(graph.names_index.size()); i++) {
    auto const path_length =
        find_path_length(valve_index, graph.names_index[i], aInputData);
    std::vector<bool> open_valves(graph.names_index.size(), false);
    open_valves[i] = true;
    process.push_back(ValveData2{i, path_length + 1, 0, open_valves});
  }

  ll best_pressure = 0;
  while (!process.empty()) {
    auto const p = process.front();
    process.pop_front();

    if (p.minutes > total_minutes) {
      continue;
    }
    if (p.minutes == total_minutes) {
      if (p.pressure > best_pressure) {
        // std::cout << "Best pressure: " << p.pressure << std::endl;
        best_pressure = p.pressure;
      }
    }

    ll pressure = 0;
    for (int i = 0; i < static_cast<int>(p.open_valves.size()); i++) {
      if (p.open_valves[i]) {
        pressure += graph.flow_rates[i];
      }
    }
    auto const final_pressure =
        p.pressure + pressure * (total_minutes - p.minutes);

    auto const ovc =
        std::count(std::cbegin(p.open_valves), std::cend(p.open_valves), true);
    auto const cache_it = cache.find(p.open_valves);
    ll best_other = 0;
    if (cache_it != std::cend(cache)) {
      best_other = cache_it->second;
    } else if (ovc > p.open_valves.size() / 3) {
      best_other = dfs(aInputData, graph, total_minutes, p.open_valves);
      cache.emplace(p.open_valves, best_other);
    }
    if (final_pressure + best_other > best_pressure) {
      // std::cout << "Best pressure: " << final_pressure << std::endl;
      best_pressure = final_pressure + best_other;
    }

    if (ovc == static_cast<int>(p.open_valves.size())) {
      continue;
    }
    if (ovc > p.open_valves.size() / 2) {
      continue;
    }

    for (int j = 0; j < static_cast<int>(graph.names_index.size()); j++) {
      if (j == p.index) {
        continue;
      }

      if (!p.open_valves[j]) {
        auto new_open_valves = p.open_valves;
        new_open_valves[j] = true;
        process.push_back(
            ValveData2{j, p.minutes + graph.edges[p.index][j] + 1,
                       p.pressure + pressure * (graph.edges[p.index][j] + 1),
                       new_open_valves});
      }
    }
  }

  return best_pressure;
}

int main() {
  auto const inputData = readInput();
  auto const graphData = reduce_graph(inputData);

  auto const s1 = solve1(inputData, graphData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData, graphData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}