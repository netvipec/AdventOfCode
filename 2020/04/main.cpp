#include <bits/stdc++.h>

using ll = int64_t;
using input_line_t = std::map<std::string, std::string>;
using input_t = std::vector<input_line_t>;

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
    input_line_t p;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            input_values.push_back(p);
            p.clear();
        }

        auto const elems = split(line, ' ');
        for (auto const& e : elems) {
            auto const b = split(e, ':');
            assert(b.size() == 2);
            p.insert(make_pair(b[0], b[1]));
        }
    }
    input_values.push_back(p);
    return input_values;
}

ll solve1(input_t const& input_data) {
    ll ans = 0;
    std::vector<std::string> required{ "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
    for (auto const& p : input_data) {
        auto const valid = std::all_of(std::cbegin(required), std::cend(required), [&](auto const& r) {
            return p.find(r) != p.cend();
        });
        if (valid) {
            ans++;
        }
    }
    return ans;
}

bool byr_check(std::string const& d) {
    auto const n = std::stoi(d);
    return 1920 <= n && n <= 2002;
}
bool iyr_check(std::string const& d) {
    auto const n = std::stoi(d);
    return 2010 <= n && n <= 2020;
}
bool eyr_check(std::string const& d) {
    auto const n = std::stoi(d);
    return 2020 <= n && n <= 2030;
}
bool hgt_check(std::string const& d) {
    if (d.size() < 2) {
        return false;
    }

    if (d[d.size() - 2] == 'c' && d[d.size() - 1] == 'm') {
        auto const v = std::stoi(d.substr(0, d.size() - 2));
        return 150 <= v && v <= 193;
    } else if (d[d.size() - 2] == 'i' && d[d.size() - 1] == 'n') {
        auto const v = std::stoi(d.substr(0, d.size() - 2));
        return 59 <= v && v <= 76;
    }

    return false;
}
bool hcl_check(std::string const& d) {
    if (d.size() != 7 || d[0] != '#') {
        return false;
    }
    auto const invalid_char = std::find_if(d.cbegin() + 1, d.cend(), [](auto const& c) {
        return !(('0' <= c && c <= '9') || ('a' <= c && c <= 'f'));
    });
    return invalid_char == d.cend();
}
bool ecl_check(std::string const& d) {
    static std::set<std::string> color{ "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
    return color.find(d) != color.cend();
}
bool pid_check(std::string const& d) {
    if (d.size() != 9)
        return false;
    auto const invalid_number = std::find_if(d.cbegin(), d.cend(), [](auto const& c) {
        return !('0' <= c && c <= '9');
    });
    return invalid_number == d.cend();
}

ll solve2(input_t const& input_data) {
    using passport_element_t = std::pair<std::string, std::function<bool(std::string const&)>>;
    std::vector<passport_element_t> const required{ { "byr", byr_check }, { "iyr", iyr_check }, { "eyr", eyr_check },
                                                    { "hgt", hgt_check }, { "hcl", hcl_check }, { "ecl", ecl_check },
                                                    { "pid", pid_check } };
    ll ans = 0;
    for (auto const& p : input_data) {
        auto const valid = std::all_of(std::cbegin(required), std::cend(required), [&](auto const& r) {
            auto const e = p.find(r.first);
            if (e == p.cend()) {
                return false;
            }
            return r.second(e->second);
        });
        if (valid) {
            ans++;
        }
    }
    return ans;
}

int main() {
    auto const input_data = read_input();
    // std::for_each(std::cbegin(input_data.back()), std::cend(input_data.back()), [](auto const& elem) {
    //     std::cout << elem.first << ", " << elem.second << std::endl;
    // });

    auto const s1 = solve1(input_data);
    std::cout << "Solution Part1: " << s1 << std::endl;

    auto const s2 = solve2(input_data);
    std::cout << "Solution Part2: " << s2 << std::endl;

    return 0;
}