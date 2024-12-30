#include <bits/stdc++.h>

using ll = int64_t;

const static std::string empty_literal{ "error literal" };

struct simple_rule_t {
    ll subrule_index = -1;
    std::string literal;
    bool is_subrule = false;
};

struct expression_t {
    std::vector<simple_rule_t> expression_list;
};

using input_t = std::pair<std::map<ll, std::vector<expression_t>>, std::vector<std::string>>;
using result_t = ll;

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
        if (line.empty()) {
            break;
        }

        auto const idx = line.find(':');
        assert(idx != std::string::npos);
        auto const index = std::stoi(line.substr(0, idx));
        auto const expr_str = line.substr(idx + 2);
        auto const expr_elems = split(expr_str, '|');

        std::vector<expression_t> checks;
        for (auto const& expr : expr_elems) {
            checks.push_back(expression_t{});
            auto const check_elems = split(expr, ' ');
            for (auto const& check : check_elems) {
                if (check.empty() || check == " ") {
                    continue;
                }
                if (check[0] == '"') {
                    checks.back().expression_list.push_back(
                        simple_rule_t{ -1, check.substr(1, check.size() - 2), false });
                } else {
                    auto const subrule_idx = std::stoi(check);
                    checks.back().expression_list.push_back(simple_rule_t{ subrule_idx, empty_literal, true });
                }
            }
        }

        input_values.first.insert(std::make_pair(index, checks));
    }

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        input_values.second.push_back(line);
    }

    return input_values;
}

std::unordered_set<std::string> expand_rule(ll rule_id, std::vector<expression_t> const& rule, input_t const& rules) {
    std::unordered_set<std::string> result;
    std::vector<std::string> one;
    for (auto const& r : rule) {
        one = { "" };
        for (auto const& e : r.expression_list) {
            if (!e.is_subrule) {
                std::for_each(std::begin(one), std::end(one), [&](auto& o) {
                    o += e.literal;
                });
            } else {
                auto const subrule_it = rules.first.find(e.subrule_index);
                assert(subrule_it != std::cend(rules.first));
                auto const inner_result = expand_rule(subrule_it->first, subrule_it->second, rules);
                std::vector<std::string> oo;
                std::for_each(std::begin(one), std::end(one), [&](auto& o) {
                    std::for_each(std::begin(inner_result), std::end(inner_result), [&](auto& ir) {
                        oo.emplace_back(o + ir);
                    });
                });
                one.swap(oo);
            }
        }

        result.insert(std::cbegin(one), std::cend(one));
    }

    return result;
}

std::vector<ll> match_rule1(std::pair<ll, std::vector<expression_t>> const& rule,
                            input_t const& rules,
                            std::string const& line,
                            std::vector<ll> const& idx) {
    std::vector<ll> outer;
    for (auto const& r : rule.second) {
        std::vector<ll> inner(idx);
        for (auto const& e : r.expression_list) {
            if (!e.is_subrule) {
                for (auto& index : inner) {
                    if (index != -1 && index < static_cast<ll>(line.size()) && e.literal[0] == line[index]) {
                        index++;
                    } else {
                        index = -1;
                    }
                }
            } else {
                auto const subrule_it = rules.first.find(e.subrule_index);
                assert(subrule_it != std::cend(rules.first));
                inner = match_rule1(*subrule_it, rules, line, inner);
            }

            if (inner.empty()) {
                break;
            }
            auto const c = std::count(std::cbegin(inner), std::cend(inner), -1);
            if (c == static_cast<int>(inner.size())) {
                inner.clear();
                break;
            }
        }

        outer.insert(outer.end(), std::cbegin(inner), std::cend(inner));
        auto const it = std::find(std::cbegin(inner), std::cend(inner), line.size());
        if (it != std::cend(inner)) {
            return outer;
        }
    }

    return outer;
}

result_t solve1(input_t const& input_data) {
    auto r = input_data.first.find(0);
    assert(r != std::cend(input_data.first));

#if 1
    result_t ans =
        std::accumulate(std::cbegin(input_data.second),
                        std::cend(input_data.second),
                        0ll,
                        [&](auto const& base, auto const& elem) {
                            auto const result = match_rule1(*r, input_data, elem, { 0 });
                            auto const it =
                                std::find(std::cbegin(result), std::cend(result), static_cast<ll>(elem.size()));
                            return base + ((it != std::cend(result)) ? 1 : 0);
                        });
    return ans;
#else
    auto const expand_r = expand_rule(r->first, r->second, input_data);

    // std::for_each(std::cbegin(expand_r), std::cend(expand_r), [&](auto const& elem) {
    //     std::cout << elem << std::endl;
    // });
    // std::cout << std::endl;

    result_t ans1 = std::count_if(std::cbegin(input_data.second), std::cend(input_data.second), [&](auto const& elem) {
        return expand_r.find(elem) != std::cend(expand_r);
    });
    return ans1;
#endif
}

std::unordered_set<std::string> expand42;
std::unordered_set<std::string> expand31;

std::vector<ll> parse_rule8(std::vector<ll> const& inner, std::string const& line) {
    std::vector<ll> all_inn;
    std::vector<ll> new_inn;
    std::vector<ll> inn = inner;
    std::vector<std::pair<ll, std::string>> s;
    for (;;) {
        s.clear();
        for (auto const& i : inn) {
            for (auto const& sr : expand42) {
                if (sr.size() <= line.size() - i) {
                    assert(i <= static_cast<ll>(line.size()));
                    auto it = std::mismatch(std::cbegin(sr), std::cend(sr), line.cbegin() + i, line.cend());
                    if (it.first == std::cend(sr)) {
                        s.emplace_back(i, sr);
                    }
                }
            }
        }
        if (!s.empty()) {
            for (auto const& ss : s) {
                new_inn.emplace_back(ss.first + static_cast<ll>(ss.second.size()));
            }
            all_inn.insert(all_inn.end(), std::cbegin(new_inn), std::cend(new_inn));
            std::swap(inn, new_inn);
            new_inn.clear();
        } else {
            return all_inn;
        }
    }
}

std::vector<ll> parse_rule11(std::vector<ll> const& inner, std::string const& line) {
    std::vector<std::pair<ll, ll>> all_inn;
    std::vector<std::pair<ll, ll>> new_inn;
    std::vector<std::pair<ll, ll>> inn(inner.size());
    std::transform(std::cbegin(inner), std::cend(inner), inn.begin(), [](auto const& elem) {
        return std::make_pair(elem, 0);
    });
    for (;;) {
        new_inn.clear();
        for (auto const& i : inn) {
            for (auto const& sr : expand42) {
                if (sr.size() <= line.size() - i.first) {
                    assert(i.first <= static_cast<ll>(line.size()));
                    auto it = std::mismatch(std::cbegin(sr), std::cend(sr), line.cbegin() + i.first, line.cend());
                    if (it.first == std::cend(sr)) {
                        new_inn.emplace_back(i.first + static_cast<ll>(sr.size()), i.second + 1);
                    }
                }
            }
        }
        if (new_inn.empty()) {
            break;
        }
        all_inn.insert(all_inn.end(), std::cbegin(new_inn), std::cend(new_inn));
        std::swap(inn, new_inn);
        new_inn.clear();
    }

    inn = all_inn;
    all_inn.clear();
    for (;;) {
        new_inn.clear();
        for (auto const& i : inn) {
            for (auto const& sr : expand31) {
                if (sr.size() <= line.size() - i.first) {
                    assert(i.first <= static_cast<ll>(line.size()));
                    auto it = std::mismatch(std::cbegin(sr), std::cend(sr), line.cbegin() + i.first, line.cend());
                    if (it.first == std::cend(sr)) {
                        new_inn.emplace_back(i.first + static_cast<ll>(sr.size()), i.second - 1);
                    }
                }
            }
        }

        if (new_inn.empty()) {
            break;
        }

        std::copy_if(std::cbegin(new_inn), std::cend(new_inn), std::back_inserter(all_inn), [](auto const& elem) {
            return elem.second == 0;
        });
        std::swap(inn, new_inn);
    }
    std::vector<ll> index;
    std::transform(std::cbegin(all_inn), std::cend(all_inn), std::back_inserter(index), [](auto const& elem) {
        return elem.first;
    });
    return index;
}

std::vector<ll> match_rule2(std::pair<ll, std::vector<expression_t>> const& rule,
                            input_t const& rules,
                            std::string const& line,
                            std::vector<ll> const& idx) {
    std::vector<ll> outer;
    for (auto const& r : rule.second) {
        std::vector<ll> inner(idx);
        for (auto const& e : r.expression_list) {
            if (!e.is_subrule) {
                for (auto& index : inner) {
                    if (index != -1 && index < static_cast<ll>(line.size()) && e.literal[0] == line[index]) {
                        index++;
                    } else {
                        index = -1;
                    }
                }
            } else {
                auto const subrule_it = rules.first.find(e.subrule_index);
                assert(subrule_it != std::cend(rules.first));
                if (subrule_it->first == 8) {
                    inner = parse_rule8(inner, line);
                } else if (subrule_it->first == 11) {
                    inner = parse_rule11(inner, line);
                } else {
                    inner = match_rule2(*subrule_it, rules, line, inner);
                }
            }

            if (inner.empty()) {
                break;
            }
            auto const c = std::count(std::cbegin(inner), std::cend(inner), -1);
            if (c == static_cast<int>(inner.size())) {
                inner.clear();
                break;
            }
        }

        outer.insert(outer.end(), std::cbegin(inner), std::cend(inner));
        auto const it = std::find(std::cbegin(inner), std::cend(inner), line.size());
        if (it != std::cend(inner)) {
            return outer;
        }
    }

    return outer;
}

result_t solve2(input_t const& input_data) {
    auto it42 = input_data.first.find(42);
    auto it31 = input_data.first.find(31);
    expand42 = expand_rule(it42->first, it42->second, input_data);
    expand31 = expand_rule(it31->first, it31->second, input_data);

    // 8: 42 | 42 8
    // 11: 42 31 | 42 11 31
    auto input_data_copy = input_data;
    auto it8 = input_data_copy.first.find(8);
    auto it11 = input_data_copy.first.find(11);
    std::vector<expression_t> new8{ expression_t{ { simple_rule_t{ 42, empty_literal, true } } },
                                    expression_t{ { simple_rule_t{ 42, empty_literal, true },
                                                    { simple_rule_t{ 8, empty_literal, true } } } } };
    it8->second = new8;
    std::vector<expression_t> new11{ expression_t{ { simple_rule_t{ 42, empty_literal, true },
                                                     simple_rule_t{ 31, empty_literal, true } } },
                                     expression_t{ { simple_rule_t{ 42, empty_literal, true },
                                                     simple_rule_t{ 11, empty_literal, true },
                                                     simple_rule_t{ 31, empty_literal, true } } } };
    it11->second = new11;
    auto r0 = input_data_copy.first.find(0);
    assert(r0 != std::cend(input_data_copy.first));
    std::vector<ll> idx;
    std::vector<bool> matched(input_data.second.size(), false);
    for (size_t i = 0; i < matched.size(); i++) {
        if (!matched[i]) {
            idx = { 0 };
            auto const r = match_rule2(*r0, input_data_copy, input_data_copy.second[i], idx);
            auto const it = std::find(std::cbegin(r), std::cend(r), static_cast<ll>(input_data_copy.second[i].size()));
            matched[i] = it != std::cend(r);
            // if (matched[i]) {
            //     std::cout << i << " -> " << input_data.second[i] << std::endl;
            // }
        }
    }
    result_t ans = std::count(std::cbegin(matched), std::cend(matched), true);
    return ans;
}

int main() {
    auto begin_input = std::chrono::steady_clock::now();
    auto const input_data = read_input();
    auto end_input = std::chrono::steady_clock::now();

    // std::for_each(std::cbegin(input_data.first), std::cend(input_data.first), [](auto const& elem) {
    //     std::cout << elem.first << ": ";
    //     std::for_each(std::cbegin(elem.second), std::cend(elem.second), [](auto const& e) {
    //         std::for_each(std::cbegin(e.expression_list), std::cend(e.expression_list), [](auto const& sr) {
    //             if (sr.is_subrule) {
    //                 std::cout << sr.subrule_index << " ";
    //             } else {
    //                 std::cout << sr.literal << " ";
    //             }
    //         });
    //         std::cout << " | ";
    //     });
    //     std::cout << std::endl;
    // });
    // std::cout << std::endl;
    // std::for_each(std::cbegin(input_data.second), std::cend(input_data.second), [](auto const& elem) {
    //     std::cout << elem << std::endl;
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