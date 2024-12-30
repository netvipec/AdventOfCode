#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::pair<std::set<std::string>, std::set<std::string>>>;
using result_t = ll;

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(),
                         s.rend(),
                         [](unsigned char ch) {
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
        auto const idx = line.find('(');
        assert(idx != std::string::npos);
        auto const ingredients_list = split(line.substr(0, idx - 1), ' ');
        auto allergents_list = split(line.substr(idx + 10, line.size() - 1 - idx - 10), ',');

        std::set<std::string> ingredients;
        std::for_each(std::begin(ingredients_list), std::end(ingredients_list), [&](auto& il) {
            ingredients.emplace(il);
        });

        std::set<std::string> allergents;
        std::for_each(std::begin(allergents_list), std::end(allergents_list), [&](auto& al) {
            trim(al);
            allergents.emplace(al);
        });

        input_values.emplace_back(ingredients, allergents);
    }
    return input_values;
}

void solve(std::vector<std::pair<ll, std::string>> const& allergent_c_ordered,
           std::unordered_map<std::string, std::pair<ll, std::vector<ll>>> const& allergent_count,
           std::unordered_map<std::string, std::pair<ll, std::vector<ll>>> const& ingredient_count,
           input_t const& input_data,
           std::unordered_map<std::string, std::string>& ingredient_allergent,
           std::vector<std::pair<ll, std::string>>::const_iterator begin_it) {
    if (begin_it == allergent_c_ordered.cend()) {
        return;
    }
    std::set<std::string> common_ingredients;
    std::set<std::string> new_common_ingredients;
    auto const allergent_index_it = allergent_count.find(begin_it->second);
    assert(allergent_index_it != std::cend(allergent_count));

    for (auto const& ai : allergent_index_it->second.second) {
        if (common_ingredients.empty()) {
            common_ingredients = input_data[ai].first;
        } else {
            std::set_intersection(std::cbegin(common_ingredients),
                                  std::cend(common_ingredients),
                                  std::cbegin(input_data[ai].first),
                                  std::cend(input_data[ai].first),
                                  std::inserter(new_common_ingredients, new_common_ingredients.end()));
            std::swap(common_ingredients, new_common_ingredients);
            new_common_ingredients.clear();
        }
    }

    for (auto const& ci : common_ingredients) {
        auto const ccii_it = ingredient_count.find(ci);
        if (ccii_it->second.first < allergent_index_it->second.first) {
            std::vector<ll> out;
            auto const fff = std::find(std::cbegin(allergent_index_it->second.second),
                                       std::cend(allergent_index_it->second.second),
                                       ccii_it->second.second.front());
            std::set_intersection(std::cbegin(ccii_it->second.second),
                                  std::cend(ccii_it->second.second),
                                  fff,
                                  std::cend(allergent_index_it->second.second),
                                  std::back_inserter(out));
            if (out.size() != ccii_it->second.second.size()) {
                continue;
            }
        }

        auto const ci_it = ingredient_allergent.find(ci);
        if (ci_it == std::cend(ingredient_allergent)) {
            ingredient_allergent.emplace(ci, begin_it->second);
            if (ingredient_allergent.size() == allergent_count.size()) {
                return;
            }
            solve(allergent_c_ordered,
                  allergent_count,
                  ingredient_count,
                  input_data,
                  ingredient_allergent,
                  begin_it + 1);
            if (ingredient_allergent.size() == allergent_count.size()) {
                return;
            }
            ingredient_allergent.erase(ci);
        }
    }
}

std::pair<std::unordered_map<std::string, std::string>, std::unordered_map<std::string, std::pair<ll, std::vector<ll>>>>
assign_ingredient_allergens(input_t const& input_data) {
    std::unordered_map<std::string, std::pair<ll, std::vector<ll>>> allergent_count;
    std::unordered_map<std::string, std::pair<ll, std::vector<ll>>> ingredient_count;
    ll index = 0;
    std::for_each(std::cbegin(input_data), std::cend(input_data), [&](auto const& elem) {
        std::for_each(std::cbegin(elem.first), std::cend(elem.first), [&](auto const& e) {
            auto& ic = ingredient_count[e];
            ic.first++;
            ic.second.emplace_back(index);
        });
        std::for_each(std::cbegin(elem.second), std::cend(elem.second), [&](auto const& e) {
            auto& ac = allergent_count[e];
            ac.first++;
            ac.second.emplace_back(index);
        });
        index++;
    });

    std::vector<std::pair<ll, std::string>> allergent_c_ordered;
    std::for_each(std::cbegin(allergent_count), std::cend(allergent_count), [&](auto const& elem) {
        allergent_c_ordered.emplace_back(elem.second.first, elem.first);
    });

    std::sort(std::begin(allergent_c_ordered), std::end(allergent_c_ordered), std::greater<>{});

    std::unordered_map<std::string, std::string> ingredient_allergent;
    solve(allergent_c_ordered,
          allergent_count,
          ingredient_count,
          input_data,
          ingredient_allergent,
          allergent_c_ordered.cbegin());

    return { ingredient_allergent, ingredient_count };
}

result_t solve1(input_t const& input_data) {
    auto const [ingredient_allergent, ingredient_count] = assign_ingredient_allergens(input_data);

    result_t ans = 0;
    std::for_each(std::cbegin(ingredient_count), std::cend(ingredient_count), [&](auto const& elem) {
        if (ingredient_allergent.find(elem.first) == std::cend(ingredient_allergent)) {
            ans += elem.second.first;
        }
    });

    return ans;
}

std::string solve2(input_t const& input_data) {
    auto const [ingredient_allergent, ingredient_count] = assign_ingredient_allergens(input_data);

    std::map<std::string, std::string> allergent_ingredient;
    std::for_each(std::cbegin(ingredient_allergent), std::cend(ingredient_allergent), [&](auto const& elem) {
        allergent_ingredient.emplace(elem.second, elem.first);
    });

    std::stringstream str;
    std::for_each(std::cbegin(allergent_ingredient), std::cend(allergent_ingredient), [&](auto const& elem) {
        str << elem.second << ",";
    });
    auto res = str.str();
    return res.substr(0, res.size() - 1);
}

int main() {
    auto begin_input = std::chrono::steady_clock::now();
    auto const input_data = read_input();
    auto end_input = std::chrono::steady_clock::now();

    // std::for_each(std::cbegin(input_data), std::cend(input_data), [](auto const& elem) {
    //     std::for_each(std::cbegin(elem.first), std::cend(elem.first), [](auto const& e) {
    //         std::cout << e << " ";
    //     });
    //     std::cout << "(contains ";
    //     std::for_each(std::cbegin(elem.second), std::cend(elem.second), [](auto const& e) {
    //         std::cout << e << ", ";
    //     });
    //     std::cout << ")" << std::endl;
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