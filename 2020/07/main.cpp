#include <bits/stdc++.h>

using ll = int64_t;
using bag_dep_t = std::pair<ll, std::string>;
using bags_dep_t = std::vector<bag_dep_t>;
using input_t = std::vector<std::pair<std::string, bags_dep_t>>;
using result_t = ll;

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(),
                         s.rend(),
                         [](int ch) {
                             return !std::isspace(ch);
                         })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

static std::vector<std::string> split(std::string const& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        // wavy turquoise bags contain 4 vibrant magenta bags, 4 light violet bags, 5 bright gold bags, 2 faded black
        // bags.
        std::istringstream iss(line);
        if (line.empty()) {
            break;
        } // error

        auto const elems = split(line, ' ');

        assert(elems.size() >= 7);
        bags_dep_t bd;
        if (elems[4] != "no") {
            for (unsigned i = 4; i < elems.size(); i += 4) {
                auto const bag_c = std::stoi(elems[i]);
                bd.emplace_back(bag_c, elems[i + 1] + ' ' + elems[i + 2]);
            }
            // std::cout << elems[0] << " -> " << bd.front().first << " -> " << line << std::endl;
        }
        input_values.emplace_back(elems[0] + ' ' + elems[1], bd);
    }
    return input_values;
}

ll get_index_bag(std::string const& bag_color, input_t const& input_data) {
    auto const bag_it = std::find_if(std::cbegin(input_data), std::cend(input_data), [&](auto const& elem) {
        return elem.first == bag_color;
    });
    assert(bag_it != std::cend(input_data));
    auto const bag_idx = std::distance(std::cbegin(input_data), bag_it);
    return bag_idx;
}

result_t solve1(input_t const& input_data) {
    auto const shiny_gold_idx = get_index_bag("shiny gold", input_data);

    std::set<ll> all_container_bags, actual_container_bags, new_container_bags;
    actual_container_bags.insert(shiny_gold_idx);
    for (;;) {
        std::for_each(std::cbegin(actual_container_bags), std::cend(actual_container_bags), [&](auto const& bag_idx) {
            for (auto const id : input_data) {
                auto const it = std::find_if(std::cbegin(id.second), std::cend(id.second), [&](auto const& elem) {
                    return elem.second == input_data[bag_idx].first;
                });

                if (it != std::cend(id.second)) {
                    auto const idx = get_index_bag(id.first, input_data);
                    if (all_container_bags.find(idx) == std::cend(all_container_bags)) {
                        new_container_bags.insert(idx);
                    }
                }
            }
        });
        if (new_container_bags.empty()) {
            break;
        }
        all_container_bags.insert(new_container_bags.cbegin(), new_container_bags.cend());
        std::swap(new_container_bags, actual_container_bags);
        new_container_bags.clear();
    };
    return static_cast<ll>(all_container_bags.size());
}

ll get_inside_bags(input_t const& input_data, bags_dep_t const& bd) {
    ll ans = 0;
    for (auto const& b : bd) {
        auto const it = std::find_if(std::cbegin(input_data), std::cend(input_data), [&](auto const& elem) {
            return elem.first == b.second;
        });
        assert(it != std::cend(input_data));
        bags_dep_t bdn;
        for (auto const& nb : it->second) {
            bdn.emplace_back(b.first * nb.first, nb.second);
        }
        ans += b.first;
        ans += get_inside_bags(input_data, bdn);
    }
    return ans;
}

result_t solve2(input_t const& input_data) {
    bags_dep_t bd{ { 1, "shiny gold" } };
    ll ans = get_inside_bags(input_data, bd);
    return ans - 1;
}

int main() {
    auto const input_data = read_input();
    // std::for_each(std::cbegin(input_data), std::cend(input_data), [](auto const& elem) {
    //     std::cout << elem << ", ";
    // });
    // std::cout << std::endl;

    auto const s1 = solve1(input_data);
    std::cout << "Solution Part1: " << s1 << std::endl;

    auto const s2 = solve2(input_data);
    std::cout << "Solution Part2: " << s2 << std::endl;

    return 0;
}