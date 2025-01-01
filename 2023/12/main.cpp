#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;

struct Records {
  std::string uncomplete_state;
  std::vector<ll> complete_state;
};

using input_t = std::vector<Records>;

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
    auto const parts = split(line, " ");
    assert(parts.size() == 2);

    inputValues.push_back(Records{parts.front()});
    auto const checksums = split(parts.back(), ",");
    std::transform(std::cbegin(checksums), std::cend(checksums),
                   std::back_inserter(inputValues.back().complete_state),
                   [](auto const &elem) { return std::stoll(elem); });
  }
  return inputValues;
}

struct CacheData {
  ll index;
  ll consecutive;
  ll index_complete;

  bool operator<(CacheData const &other) const {
    return std::tie(index, consecutive, index_complete) <
           std::tie(other.index, other.consecutive, other.index_complete);
  }
};

std::map<CacheData, ll> cache;

ll calculate_possible_arrengements(Records const &record, ll index,
                                   ll consecutive, ll index_complete) {
  if (index == static_cast<ll>(record.uncomplete_state.size())) {
    auto const c =
        (index_complete + 1 == static_cast<ll>(record.complete_state.size()) &&
         record.complete_state[index_complete] == consecutive) ||
                (index_complete ==
                     static_cast<ll>(record.complete_state.size()) &&
                 consecutive == 0)
            ? 1
            : 0;
    return c;
  }
  if (consecutive != 0 &&
      index_complete == static_cast<ll>(record.complete_state.size())) {
    return 0;
  }

  if (consecutive > record.complete_state[index_complete]) {
    return 0;
  }

  auto const key = CacheData{index, consecutive, index_complete};
  auto const it = cache.find(key);
  if (it != std::cend(cache)) {
    return it->second;
  }

  ll res = 0;
  if (record.uncomplete_state[index] == '#') {
    res = calculate_possible_arrengements(record, index + 1, consecutive + 1,
                                          index_complete);
  } else if (record.uncomplete_state[index] == '.') {
    if (consecutive == 0) {
      res =
          calculate_possible_arrengements(record, index + 1, 0, index_complete);
    } else if (consecutive != record.complete_state[index_complete]) {
      res = 0;
    } else {
      res = calculate_possible_arrengements(record, index + 1, 0,
                                            index_complete + 1);
    }
  } else {
    if (consecutive == 0) {
      res +=
          calculate_possible_arrengements(record, index + 1, 0, index_complete);
    }
    if (consecutive == record.complete_state[index_complete]) {
      res += calculate_possible_arrengements(record, index + 1, 0,
                                             index_complete + 1);
    }
    if (consecutive < record.complete_state[index_complete]) {
      res += calculate_possible_arrengements(record, index + 1, consecutive + 1,
                                             index_complete);
    }
  }
  cache[key] = res;

  return res;
}

ll calculate_possible_arrengements(Records const &record) {
  cache.clear();
  return calculate_possible_arrengements(record, 0, 0, 0);
}

ll solve1(input_t const &aInputData) {
  return std::accumulate(std::cbegin(aInputData), std::cend(aInputData), 0ll,
                         [](auto const &base, auto const &elem) {
                           return base + calculate_possible_arrengements(elem);
                         });
}

ll solve2(input_t const &aInputData) {
  return std::accumulate(
      std::cbegin(aInputData), std::cend(aInputData), 0ll,
      [](auto const &base, auto const &elem) {
        auto uncomplete_state = elem.uncomplete_state;
        auto complete_state = elem.complete_state;
        for (ll i = 0; i < 5 - 1; i++) {
          uncomplete_state += '?' + elem.uncomplete_state;
          complete_state.insert(complete_state.end(),
                                std::cbegin(elem.complete_state),
                                std::cend(elem.complete_state));
        }
        return base + calculate_possible_arrengements(
                          Records{uncomplete_state, complete_state});
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