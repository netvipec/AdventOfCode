#include <bits/stdc++.h>

using ll = int64_t;

struct point {
    ll x = 0;
    ll y = 0;
    ll z = 0;
    ll w = 0;
};

using row_t = std::vector<bool>;
using input_t = std::vector<row_t>;
using result_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        std::vector<bool> row;
        std::transform(std::cbegin(line), std::cend(line), std::back_inserter(row), [](auto const& c) {
            return c == '#';
        });

        input_values.emplace_back(row);
    }
    return input_values;
}

std::vector<point> get_neighbors(bool three_dimention = true) {
    std::vector<point> n;
    for (ll i = -1; i <= 1; i++) {
        for (ll j = -1; j <= 1; j++) {
            for (ll k = -1; k <= 1; k++) {
                if (three_dimention) {
                    if (i == 0 && j == 0 && k == 0) {
                        continue;
                    }
                    n.emplace_back(point{ i, j, k });
                } else {
                    for (ll l = -1; l <= 1; l++) {
                        if (i == 0 && j == 0 && k == 0 && l == 0) {
                            continue;
                        }
                        n.emplace_back(point{ i, j, k, l });
                    }
                }
            }
        }
    }
    return n;
}

auto const neighbors_3d = get_neighbors();
auto const neighbors_4d = get_neighbors(false);

inline bool is_valid_3d(point const& p, ll rows, ll columns) {
    return (1 <= p.x && p.x <= rows) && (1 <= p.y && p.y <= columns) && (1 <= p.z && p.z <= columns);
}

inline bool get_cell(std::vector<input_t> const& cube, point const& p, ll displacement = 0) {
    return cube[p.x - displacement][p.y - displacement][p.z - displacement];
}

void evolve(std::vector<input_t>& cube) {
    auto const cube_rows = cube.size();
    auto const cube_columns = cube.front().size();
    std::vector<input_t> new_cube(cube_rows + 2, input_t(cube_columns + 2, std::vector<bool>(cube_columns + 2, false)));

    for (ll i = 0; i < (ll)new_cube.size(); i++) {
        for (ll j = 0; j < (ll)new_cube[i].size(); j++) {
            for (ll k = 0; k < (ll)new_cube[i][j].size(); k++) {
                ll active = 0;
                for (auto const& n : neighbors_3d) {
                    point np;
                    np.x = i + n.x;
                    np.y = j + n.y;
                    np.z = k + n.z;
                    if (is_valid_3d(np, cube_rows, cube_columns)) {
                        active += (get_cell(cube, np, 1) ? 1 : 0);
                    }
                }

                point const pp{ i, j, k };
                if (is_valid_3d(pp, cube_rows, cube_columns)) {
                    auto const cell_value = get_cell(cube, pp, 1);
                    if (cell_value && (active == 2 || active == 3)) {
                        new_cube[i][j][k] = true;
                    } else if (!cell_value && active == 3) {
                        new_cube[i][j][k] = true;
                    }
                } else if (active == 3) {
                    new_cube[i][j][k] = true;
                }
            }
        }
    }

    std::swap(cube, new_cube);
}

void print_cube_3d(std::vector<input_t> const& cube) {
    std::for_each(std::cbegin(cube), std::cend(cube), [](auto const& l) {
        std::for_each(std::cbegin(l), std::cend(l), [](auto const& r) {
            std::for_each(std::cbegin(r), std::cend(r), [](auto const& c) {
                std::cout << (c ? '#' : '.');
            });
            std::cout << std::endl;
        });
        std::cout << std::endl << std::endl;
    });
    std::cout << std::endl;
}

result_t solve1(input_t const& input_data) {
    std::vector<input_t> cube{ input_data };
    for (int cycle = 1; cycle <= 6; cycle++) {
        evolve(cube);
        // if (cycle < 3)
        //     print_cube_3d(cube);
    }
    result_t ans = std::accumulate(std::cbegin(cube), std::cend(cube), 0ll, [](auto const& b1, auto const& l) {
        return b1 + std::accumulate(std::cbegin(l), std::cend(l), 0ll, [](auto const& b2, auto const& r) {
                   return b2 + std::count(std::cbegin(r), std::cend(r), true);
               });
    });

    return ans;
}

inline bool is_valid_4d(point const& p, ll rows, ll columns) {
    return (1 <= p.x && p.x <= rows) && (1 <= p.y && p.y <= rows) && (1 <= p.z && p.z <= columns)
           && (1 <= p.w && p.w <= columns);
}

inline bool get_cell(std::vector<std::vector<input_t>> const& cube, point const& p, ll displacement = 0) {
    return cube[p.x - displacement][p.y - displacement][p.z - displacement][p.w - displacement];
}

void evolve2(std::vector<std::vector<input_t>>& cube) {
    auto const cube_rows = cube.size();
    auto const cube_columns = cube.front().front().size();
    std::vector<std::vector<input_t>> new_cube(
        cube_rows + 2,
        std::vector<input_t>(cube_rows + 2, input_t(cube_columns + 2, std::vector<bool>(cube_columns + 2, false))));

    for (ll i = 0; i < (ll)new_cube.size(); i++) {
        for (ll j = 0; j < (ll)new_cube[i].size(); j++) {
            for (ll k = 0; k < (ll)new_cube[i][j].size(); k++) {
                for (ll l = 0; l < (ll)new_cube[i][j].size(); l++) {
                    ll active = 0;
                    for (auto const& n : neighbors_4d) {
                        point np;
                        np.x = i + n.x;
                        np.y = j + n.y;
                        np.z = k + n.z;
                        np.w = l + n.w;
                        if (is_valid_4d(np, cube_rows, cube_columns)) {
                            active += (get_cell(cube, np, 1) ? 1 : 0);
                        }
                    }

                    point const pp{ i, j, k, l };
                    if (is_valid_4d(pp, cube_rows, cube_columns)) {
                        auto const cell_value = get_cell(cube, pp, 1);
                        if (cell_value && (active == 2 || active == 3)) {
                            new_cube[i][j][k][l] = true;
                        } else if (!cell_value && active == 3) {
                            new_cube[i][j][k][l] = true;
                        }
                    } else if (active == 3) {
                        new_cube[i][j][k][l] = true;
                    }
                }
            }
        }
    }

    std::swap(cube, new_cube);
}

void print_cube_4d(std::vector<std::vector<input_t>> const& cube) {
    std::for_each(std::cbegin(cube), std::cend(cube), [](auto const& d) {
        std::for_each(std::cbegin(d), std::cend(d), [](auto const& l) {
            std::for_each(std::cbegin(l), std::cend(l), [](auto const& r) {
                std::for_each(std::cbegin(r), std::cend(r), [](auto const& c) {
                    std::cout << (c ? '#' : '.');
                });
                std::cout << std::endl;
            });
            std::cout << std::endl << std::endl;
        });
        std::cout << std::endl << std::endl;
    });
    std::cout << std::endl;
}

result_t solve2(input_t const& input_data) {
    std::vector<std::vector<input_t>> cube{ { input_data } };
    for (int cycle = 1; cycle <= 6; cycle++) {
        evolve2(cube);
        // if (cycle < 3)
        //     print_cube_4d(cube);
    }
    result_t ans = std::accumulate(std::cbegin(cube), std::cend(cube), 0ll, [](auto const& b1, auto const& c) {
        return b1 + std::accumulate(std::cbegin(c), std::cend(c), 0ll, [](auto const& b2, auto const& l) {
                   return b2 + std::accumulate(std::cbegin(l), std::cend(l), 0ll, [](auto const& b3, auto const& r) {
                              return b3 + std::count(std::cbegin(r), std::cend(r), true);
                          });
               });
    });

    return ans;
}

int main() {
    auto begin_input = std::chrono::steady_clock::now();
    auto const input_data = read_input();
    auto end_input = std::chrono::steady_clock::now();
    // std::for_each(std::cbegin(input_data), std::cend(input_data), [](auto const& r) {
    //     std::for_each(std::cbegin(r), std::cend(r), [](auto const& c) {
    //         std::cout << c << ", ";
    //     });
    //     std::cout << std::endl;
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