#include <bits/stdc++.h>

using ll = int64_t;

struct point {
    ll x = 0;
    ll y = 0;
    ll z = 0;
    ll w = 0;

    inline bool operator==(const point& e) const { return x == e.x && y == e.y && z == e.z && w == e.w; }
};

inline void hash_combine(std::size_t& seed) {}

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    hash_combine(seed, rest...);
}

namespace std {
    template <>
    struct hash<point> {
        using argument_type = point;
        using result_type = std::size_t;
        result_type operator()(argument_type const& a) const {
            result_type const h1(std::hash<int>()(a.x));
            result_type const h2(std::hash<int>()(a.y));
            result_type const h3(std::hash<int>()(a.z));
            result_type const h4(std::hash<int>()(a.w));
            result_type seed = 0;
            hash_combine(seed, h1, h2, h3, h4);
            return seed;
        }
    };
} // namespace std

using input_t = std::unordered_set<point>;
using result_t = ll;

input_t read_input() {
    input_t input_values;

    ll idx = 0;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        ll inner_idx = 0;
        std::for_each(std::cbegin(line), std::cend(line), [&](auto const& c) {
            if (c == '#') {
                input_values.insert(point{ idx, inner_idx });
            }
            inner_idx++;
        });
        idx++;
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

void evolve1(input_t& cube) {
    input_t new_cube;

    auto const mm_x = std::minmax_element(std::cbegin(cube), std::cend(cube), [](auto const& lhs, auto const& rhs) {
        return lhs.x < rhs.x;
    });
    auto const mm_y = std::minmax_element(std::cbegin(cube), std::cend(cube), [](auto const& lhs, auto const& rhs) {
        return lhs.y < rhs.y;
    });
    auto const mm_z = std::minmax_element(std::cbegin(cube), std::cend(cube), [](auto const& lhs, auto const& rhs) {
        return lhs.z < rhs.z;
    });

    for (ll i = mm_x.first->x - 1; i <= mm_x.second->x + 1; i++) {
        for (ll j = mm_y.first->y - 1; j <= mm_y.second->y + 1; j++) {
            for (ll k = mm_z.first->z - 1; k <= mm_z.second->z + 1; k++) {
                ll active = 0;
                for (auto const& n : neighbors_3d) {
                    point np;
                    np.x = i + n.x;
                    np.y = j + n.y;
                    np.z = k + n.z;
                    if (cube.find(np) != std::cend(cube)) {
                        active++;
                    }
                }

                point const pp{ i, j, k };
                if (cube.find(pp) != std::cend(cube)) {
                    if (active == 2 || active == 3) {
                        new_cube.insert(pp);
                    }
                } else if (active == 3) {
                    new_cube.insert(pp);
                }
            }
        }
    }

    std::swap(cube, new_cube);
}

void evolve2(input_t& cube) {
    input_t new_cube;

    auto const mm_x = std::minmax_element(std::cbegin(cube), std::cend(cube), [](auto const& lhs, auto const& rhs) {
        return lhs.x < rhs.x;
    });
    auto const mm_y = std::minmax_element(std::cbegin(cube), std::cend(cube), [](auto const& lhs, auto const& rhs) {
        return lhs.y < rhs.y;
    });
    auto const mm_z = std::minmax_element(std::cbegin(cube), std::cend(cube), [](auto const& lhs, auto const& rhs) {
        return lhs.z < rhs.z;
    });
    auto const mm_w = std::minmax_element(std::cbegin(cube), std::cend(cube), [](auto const& lhs, auto const& rhs) {
        return lhs.w < rhs.w;
    });

    for (ll i = mm_x.first->x - 1; i <= mm_x.second->x + 1; i++) {
        for (ll j = mm_y.first->y - 1; j <= mm_y.second->y + 1; j++) {
            for (ll k = mm_z.first->z - 1; k <= mm_z.second->z + 1; k++) {
                for (ll l = mm_w.first->w - 1; l <= mm_w.second->w + 1; l++) {
                    ll active = 0;
                    for (auto const& n : neighbors_4d) {
                        point np;
                        np.x = i + n.x;
                        np.y = j + n.y;
                        np.z = k + n.z;
                        np.w = l + n.w;
                        if (cube.find(np) != std::cend(cube)) {
                            active++;
                        }
                    }

                    point const pp{ i, j, k, l };
                    if (cube.find(pp) != std::cend(cube)) {
                        if (active == 2 || active == 3) {
                            new_cube.insert(pp);
                        }
                    } else if (active == 3) {
                        new_cube.insert(pp);
                    }
                }
            }
        }
    }

    std::swap(cube, new_cube);
}

result_t solve1(input_t const& input_data) {
    input_t cube = input_data;
    for (int cycle = 1; cycle <= 6; cycle++) {
        evolve1(cube);
    }
    return cube.size();
}

result_t solve2(input_t const& input_data) {
    input_t cube = input_data;
    for (int cycle = 1; cycle <= 6; cycle++) {
        evolve2(cube);
    }
    return cube.size();
}

int main() {
    auto begin_input = std::chrono::steady_clock::now();
    auto const input_data = read_input();
    auto end_input = std::chrono::steady_clock::now();
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