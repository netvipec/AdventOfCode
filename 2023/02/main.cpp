#include <bits/stdc++.h>
#include <ranges>

using ll = int64_t;
using data_t = std::unordered_map<std::string, int>;
using input_t = std::vector<std::vector<data_t>>;

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
    const auto gameParts = split(line, ": ");
    assert(gameParts.size() == 2);
    const auto subsetColorParts = split(gameParts[1], "; ");
    assert(subsetColorParts.size() > 0);

    std::vector<data_t> subsetGames;
    std::transform(std::cbegin(subsetColorParts), std::cend(subsetColorParts),
                   std::back_inserter(subsetGames), [](const auto &colors) {
                     const auto subColorParts = split(colors, ", ");
                     assert(subColorParts.size() > 0);

                     data_t game;
                     std::transform(
                         std::cbegin(subColorParts), std::cend(subColorParts),
                         std::inserter(game, game.end()),
                         [](const auto &subColors) {
                           const auto subColorNumbers = split(subColors, " ");
                           assert(subColorNumbers.size() == 2);

                           return std::make_pair(subColorNumbers[1],
                                                 std::stoi(subColorNumbers[0]));
                         });
                     return game;
                   });
    inputValues.push_back(subsetGames);
  }
  return inputValues;
}

bool possible(const data_t &game, const data_t &maxGame) {
  return std::all_of(std::cbegin(game), std::cend(game), [&](const auto &elem) {
    auto it = maxGame.find(elem.first);
    return it != std::cend(maxGame) && elem.second <= it->second;
  });
}

void print(const std::vector<data_t> &game, int number) {
  std::cout << "Game " << number << ": ";
  std::for_each(std::cbegin(game), std::cend(game), [&](auto const &elem1) {
    std::for_each(std::cbegin(elem1), std::cend(elem1), [](auto const &game) {
      std::cout << game.second << " " << game.first << ", ";
    });
    std::cout << "; ";
  });
  std::cout << std::endl;
}

ll solve1(input_t const &aInputData) {
  const data_t kMaxGame = {{"red", 12}, {"green", 13}, {"blue", 14}};
  int i = 0;
  return std::accumulate(std::cbegin(aInputData), std::cend(aInputData), 0,
                         [&](const auto &base, const auto &game) {
                           auto res =
                               std::all_of(std::cbegin(game), std::cend(game),
                                           [&](const auto &subGame) {
                                             return possible(subGame, kMaxGame);
                                           });
                           i++;
                           return base + (res ? i : 0);
                         });
}

data_t maximalGame(const std::vector<data_t> &game) {
  const data_t init = {{{"red", 0}, {"green", 0}, {"blue", 0}}};
  return std::accumulate(
      std::cbegin(game), std::cend(game), init,
      [](const auto &base, const auto &subGame) {
        data_t maximal;
        std::transform(std::cbegin(base), std::cend(base),
                       std::inserter(maximal, maximal.end()),
                       [&](const auto &color) {
                         auto it = subGame.find(color.first);
                         if (it != std::cend(subGame)) {
                           return std::make_pair(
                               color.first, std::max(color.second, it->second));
                         } else {
                           return std::make_pair(color.first, color.second);
                         }
                       });
        return maximal;
      });
}

ll solve2(input_t const &aInputData) {
  const data_t kMaxGame = {{"red", 12}, {"green", 13}, {"blue", 14}};
  return std::accumulate(std::cbegin(aInputData), std::cend(aInputData), 0,
                         [&](const auto &base, const auto &game) {
                           auto res = maximalGame(game);
                           auto n = std::accumulate(
                               std::cbegin(res), std::cend(res), 1,
                               [](const auto &base, const auto &elem) {
                                 return base * elem.second;
                               });
                           return base + n;
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