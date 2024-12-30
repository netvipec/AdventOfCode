#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
#include <numeric>
#include <utility>

using ll = int64_t;

struct Command {
  char D;
  int size;
};

struct Pos {
  int x = 0;
  int y = 0;

  bool operator<(Pos const &other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
};

using input_t = std::vector<Command>;

input_t readInput() {
  input_t inputValues;

  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    char com;
    int size;
    if (!(iss >> com >> size)) {
      break;
    } // error

    inputValues.push_back(Command{com, size});
  }
  return inputValues;
}

void moveHead(Command const &command, Pos &head) {
  switch (command.D) {
  case 'U':
    head.x += 1;
    break;
  case 'D':
    head.x -= 1;
    break;
  case 'L':
    head.y -= 1;
    break;
  case 'R':
    head.y += 1;
    break;
  default:
    assert(false);
  }
}
void moveTail(Pos &tail, Pos const &head) {
  if (head.x == tail.x) {
    if (head.y - tail.y > 1) {
      tail.y += 1;
    } else if (tail.y - head.y > 1) {
      tail.y -= 1;
    }
  } else if (head.y == tail.y) {
    if (head.x - tail.x > 1) {
      tail.x += 1;
    } else if (tail.x - head.x > 1) {
      tail.x -= 1;
    }
  } else if (std::abs(head.x - tail.x) > 1 || std::abs(head.y - tail.y) > 1) {
    if (head.y - tail.y > 0) {
      tail.y += 1;
    } else if (tail.y - head.y > 0) {
      tail.y -= 1;
    }
    if (head.x - tail.x > 0) {
      tail.x += 1;
    } else if (tail.x - head.x > 0) {
      tail.x -= 1;
    }
  }
}

ll solve1(input_t const &aInputData) {
  Pos head;
  Pos tail;

  std::set<Pos> visited;
  visited.insert(tail);

  std::for_each(std::cbegin(aInputData), std::cend(aInputData),
                [&](auto const &command) {
                  for (int i = 0; i < command.size; i++) {
                    moveHead(command, head);
                    moveTail(tail, head);

                    visited.insert(tail);
                  }
                });
  return visited.size();
}

ll solve2(input_t const &aInputData) {
  Pos head;
  std::vector<Pos> tails(9);

  std::set<Pos> visited;
  visited.insert(tails.front());

  std::for_each(std::cbegin(aInputData), std::cend(aInputData),
                [&](auto const &command) {
                  for (int i = 0; i < command.size; i++) {
                    moveHead(command, head);

                    moveTail(tails.front(), head);
                    for (int j = 1; j < static_cast<int>(tails.size()); j++) {
                      moveTail(tails[j], tails[j - 1]);
                    }

                    visited.insert(tails.back());
                  }
                });
  return visited.size();
}

int main() {
  auto const inputData = readInput();

  auto const s1 = solve1(inputData);
  std::cout << "Solution Part1: " << s1 << std::endl;

  auto const s2 = solve2(inputData);
  std::cout << "Solution Part2: " << s2 << std::endl;

  return 0;
}