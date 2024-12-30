#include <algorithm>
#include <bits/stdc++.h>
#include <functional>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;
using input_t = std::vector<std::string>;

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

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    inputValues.push_back(line);
  }
  return inputValues;
}

struct Files {
  std::string name;
  ll size;
};

std::map<std::vector<std::string>, std::vector<Files>>
get_fs(input_t const &aInputData) {
  std::map<std::vector<std::string>, std::vector<Files>> fs;
  std::vector<std::string> cwd;
  std::for_each(
      std::cbegin(aInputData), std::cend(aInputData), [&](auto const &line) {
        if (line[0] == '$') {
          // Command
          auto const elems = split(line, ' ');
          if (elems[1] == "cd") {
            assert(elems.size() == 3);
            if (elems[2] == "..") {
              cwd.pop_back();
            } else {
              cwd.push_back(elems[2]);
            }
          } else if (elems[1] == "ls") {
          }
        } else {
          // Output
          auto const elems = split(line, ' ');
          if (elems[0] == "dir") {
            assert(elems.size() == 2);
          } else {
            assert(elems.size() == 2);
            auto cwd_copy = cwd;
            while (!cwd_copy.empty()) {
              fs[cwd_copy].push_back(Files{elems[1], std::stoi(elems[0])});
              cwd_copy.pop_back();
            }
          }
        }
      });
  return fs;
}

ll solve1(input_t const &aInputData) {
  auto const fs = get_fs(aInputData);

  std::vector<ll> folder_size;
  std::transform(std::cbegin(fs), std::cend(fs),
                 std::back_inserter(folder_size), [](auto const &folder) {
                   return std::accumulate(
                       std::cbegin(folder.second), std::cend(folder.second),
                       0LL, [](auto const &base, auto const &file) {
                         return base + file.size;
                       });
                 });

  return std::accumulate(std::cbegin(folder_size), std::cend(folder_size), 0LL,
                         [](auto const &base, auto const &file_size) {
                           if (file_size <= 100000) {
                             return base + file_size;
                           }
                           return base;
                         });
}

ll solve2(input_t const &aInputData) {
  auto const fs = get_fs(aInputData);

  std::vector<ll> folder_size;
  std::transform(std::cbegin(fs), std::cend(fs),
                 std::back_inserter(folder_size), [](auto const &folder) {
                   return std::accumulate(
                       std::cbegin(folder.second), std::cend(folder.second),
                       0LL, [](auto const &base, auto const &file) {
                         return base + file.size;
                       });
                 });

  std::sort(std::begin(folder_size), std::end(folder_size));

  auto const total_space = 70000000LL;
  auto const free_space = total_space - folder_size.back();
  auto const required_update_free_space = 30000000LL;
  auto it = std::find_if(
      std::cbegin(folder_size), std::cend(folder_size), [=](auto const &fsize) {
        return fsize + free_space >= required_update_free_space;
      });

  return *it;
}

int main() {
  auto const inputData = readInput();
  std::cout << std::endl;

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}