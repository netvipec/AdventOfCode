#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::string>;
using result1_t = ll;
using result2_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        input_values.emplace_back(line);
    }
    return input_values;
}

const std::array<ll, 6> DX = { -1, 1, 2, 1, -1, -2 };
const std::array<ll, 6> DY = { -1, -1, 0, 1, 1, 0 };

std::pair<ll, ll> walk(std::string const& dir) {
    std::pair<ll, ll> p;
    for (size_t i = 0; i < dir.size(); i++) {
        switch (dir[i]) {
            case 'e':
                p.first += 2;
                break;
            case 's':
                switch (dir[i + 1]) {
                    case 'e':
                        p.first += 1;
                        p.second += 1;
                        break;
                    case 'w':
                        p.first -= 1;
                        p.second += 1;
                        break;
                    default:
                        assert(false);
                        break;
                }
                i++;
                break;
            case 'w':
                p.first -= 2;
                break;
            case 'n':
                switch (dir[i + 1]) {
                    case 'e':
                        p.first += 1;
                        p.second -= 1;
                        break;
                    case 'w':
                        p.first -= 1;
                        p.second -= 1;
                        break;
                    default:
                        assert(false);
                        break;
                }
                i++;
                break;
            default:
                assert(false);
                break;
        }
    }
    return p;
}

result1_t solve1(input_t const& input_data) {
    std::set<std::pair<ll, ll>> black_tiles;
    for (auto const& ti : input_data) {
        auto const point = walk(ti);
        auto const it = black_tiles.find(point);
        if (it == std::cend(black_tiles)) {
            // std::cout << point.first << "," << point.second << ": flip to black" << std::endl;
            black_tiles.insert(point);
        } else {
            // std::cout << point.first << "," << point.second << ": flip to white again" << std::endl;
            black_tiles.erase(it);
        }
    }

    return black_tiles.size();
}

std::set<std::pair<ll, ll>> evolve(std::set<std::pair<ll, ll>> const& black_tiles) {
    std::set<std::pair<ll, ll>> new_black_tiles;

    auto const mmx =
        std::minmax_element(std::cbegin(black_tiles), std::cend(black_tiles), [](auto const& lhs, auto const& rhs) {
            return lhs.first < rhs.first;
        });
    auto const mmy =
        std::minmax_element(std::cbegin(black_tiles), std::cend(black_tiles), [](auto const& lhs, auto const& rhs) {
            return lhs.second < rhs.second;
        });

    for (ll x = mmx.first->first - 2; x <= mmx.second->first + 2; x++) {
        for (ll y = mmy.first->second - 1; y <= mmy.second->second + 1; y++) {
            ll blacks = 0;
            ll whites = 0;
            for (ll d = 0; d < static_cast<ll>(DX.size()); d++) {
                auto const new_x = x + DX[d];
                auto const new_y = y + DY[d];

                auto const it = black_tiles.find({ new_x, new_y });
                if (it != std::cend(black_tiles)) {
                    blacks++;
                } else {
                    whites++;
                }
            }

            auto const it_curr = black_tiles.find({ x, y });
            if (it_curr != std::cend(black_tiles)) {
                if (!(blacks == 0 || blacks > 2)) {
                    new_black_tiles.insert({ x, y });
                }
            } else {
                if (blacks == 2) {
                    new_black_tiles.insert({ x, y });
                }
            }
        }
    }
    return new_black_tiles;
}

result2_t solve2(input_t const& input_data) {
    std::set<std::pair<ll, ll>> black_tiles;
    for (auto const& ti : input_data) {
        auto const point = walk(ti);
        auto const it = black_tiles.find(point);
        if (it == std::cend(black_tiles)) {
            black_tiles.insert(point);
        } else {
            black_tiles.erase(it);
        }
    }

    std::set<std::pair<ll, ll>> new_day;
    for (ll day = 1; day <= 100; day++) {
        new_day = evolve(black_tiles);
        std::swap(black_tiles, new_day);
    }

    return black_tiles.size();
}

int main() {
    auto begin_input = std::chrono::steady_clock::now();
    auto const input_data = read_input();
    auto end_input = std::chrono::steady_clock::now();
    // std::for_each(std::cbegin(input_data), std::cend(input_data), [](auto const& elem) {
    //     std::cout << elem << ", ";
    // });
    // std::cout << std::endl;
    std::cout << "Read Input Time = "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_input - begin_input).count() << "[ms]"
              << std::endl;

    auto begin_s1 = std::chrono::steady_clock::now();
    auto const s1 = solve1(input_data);
    auto end_s1 = std::chrono::steady_clock::now();
    std::cout << "Solution Part1: " << s1
              << "   -> Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_s1 - begin_s1).count()
              << "[ms]" << std::endl;

    auto begin_s2 = std::chrono::steady_clock::now();
    auto const s2 = solve2(input_data);
    auto end_s2 = std::chrono::steady_clock::now();
    std::cout << "Solution Part2: " << s2
              << "   -> Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_s2 - begin_s2).count()
              << "[ms]" << std::endl;

    return 0;
}