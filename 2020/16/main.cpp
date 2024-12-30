#include <bits/stdc++.h>

using ll = int64_t;

struct range_info {
    std::pair<ll, ll> range1;
    std::pair<ll, ll> range2;
    std::string fieldname;
};
struct ticket_info {
    std::vector<range_info> ranges;

    std::vector<ll> my_ticket;

    std::vector<std::vector<ll>> other_tickets;
};

using input_t = ticket_info;
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
    std::string line;
    ticket_info ti;
    range_info ri;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        auto const idx = line.find(':');
        assert(idx != std::string::npos);
        ri.fieldname = line.substr(0, idx);

        auto const ranges = line.substr(idx + 2);
        auto const ranges_v = split(ranges, ' ');
        assert(ranges_v.size() == 3);
        assert(ranges_v[1] == "or");

        auto const r1 = split(ranges_v[0], '-');
        auto const r2 = split(ranges_v[2], '-');
        assert(r1.size() == 2);
        assert(r2.size() == 2);
        ri.range1.first = std::stoi(r1[0]);
        ri.range1.second = std::stoi(r1[1]);
        ri.range2.first = std::stoi(r2[0]);
        ri.range2.second = std::stoi(r2[1]);

        // std::cout << ri.fieldname << ": " << ri.range1.first << "-" << ri.range1.second << " or " << ri.range2.first
        //           << "-" << ri.range2.second << std::endl;

        ti.ranges.push_back(ri);
    }

    std::getline(std::cin, line);
    assert(line == "your ticket:");
    std::getline(std::cin, line);

    auto const nn = split(line, ',');
    for (auto const& n : nn) {
        ti.my_ticket.push_back(std::stoi(n));
    }

    // std::for_each(std::cbegin(ti.my_ticket), std::cend(ti.my_ticket), [](auto const& elem) {
    //     std::cout << elem << ",";
    // });
    // std::cout << std::endl;

    std::getline(std::cin, line);
    std::getline(std::cin, line);
    assert(line == "nearby tickets:");

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        auto const on = split(line, ',');
        ti.other_tickets.push_back(std::vector<ll>{});
        for (auto const& n : on) {
            ti.other_tickets.back().push_back(std::stoi(n));
        }

        // std::for_each(std::cbegin(ti.other_tickets.back()), std::cend(ti.other_tickets.back()), [](auto const& elem)
        // {
        //     std::cout << elem << ",";
        // });
        // std::cout << std::endl;
    }

    return ti;
}

bool is_valid(std::pair<ll, ll> const& r, ll value) { return r.first <= value && value <= r.second; }

bool is_valid(input_t const& input_data, ll value) {
    return std::any_of(std::cbegin(input_data.ranges), std::cend(input_data.ranges), [=](auto const& r) {
        return is_valid(r.range1, value) || is_valid(r.range2, value);
    });
}

result_t solve1(input_t const& input_data) {
    return std::accumulate(
        std::cbegin(input_data.other_tickets),
        std::cend(input_data.other_tickets),
        0ll,
        [&](auto const& b1, auto const& otv) {
            return b1 + std::accumulate(std::cbegin(otv), std::cend(otv), 0ll, [&](auto const& b2, auto const& v) {
                       return b2 + (is_valid(input_data, v) ? 0 : v);
                   });
        });
}

result_t solve2(input_t const& input_data) {
    std::vector<bool> valid_tickets(input_data.other_tickets.size(), true);
    ll idx = 0;
    for (auto const& otv : input_data.other_tickets) {
        valid_tickets[idx] = std::none_of(std::cbegin(otv), std::cend(otv), [&](auto const& v) {
            return !is_valid(input_data, v);
        });
        idx++;
    }

    std::vector<std::vector<bool>> possibilities(input_data.my_ticket.size(),
                                                 std::vector<bool>(input_data.my_ticket.size(), true));

    for (size_t i = 0; i < input_data.other_tickets.size(); i++) {
        if (!valid_tickets[i]) {
            continue;
        }

        for (size_t j = 0; j < input_data.other_tickets[i].size(); j++) {
            for (size_t k = 0; k < input_data.ranges.size(); k++) {
                auto const value = input_data.other_tickets[i][j];
                if (!is_valid(input_data.ranges[k].range1, value) && !is_valid(input_data.ranges[k].range2, value)) {
                    possibilities[j][k] = false;
                }
            }
        }
    }

    ll retry_count = 0;
    for (;;) {
        retry_count++;
        for (size_t j = 0; j < possibilities.size(); j++) {
            auto const p = std::count(std::cbegin(possibilities[j]), std::cend(possibilities[j]), true);
            if (p == 1) {
                auto const p_it = std::find(std::cbegin(possibilities[j]), std::cend(possibilities[j]), true);
                assert(p_it != std::cend(possibilities[j]));
                auto const p_idx = std::distance(std::cbegin(possibilities[j]), p_it);
                std::for_each(std::begin(possibilities), std::begin(possibilities) + j, [=](auto& elem) {
                    elem[p_idx] = false;
                });
                std::for_each(std::begin(possibilities) + j + 1, std::end(possibilities), [=](auto& elem) {
                    elem[p_idx] = false;
                });
            }
        }

        // std::cout << "retry: " << retry_count << std::endl;
        bool retry = std::any_of(std::cbegin(possibilities), std::cend(possibilities), [](auto const& elem) {
            return std::count(std::cbegin(elem), std::cend(elem), true) > 1;
        });
        if (!retry) {
            break;
        }
    }

    result_t ans = 1;
    for (size_t i = 0; i < input_data.ranges.size(); i++) {
        if (input_data.ranges[i].fieldname.find("departure") == 0) {
            auto const it = std::find_if(std::cbegin(possibilities), std::cend(possibilities), [=](auto const& elem) {
                return elem[i];
            });

            auto const it_idx = std::distance(std::cbegin(possibilities), it);
            ans *= input_data.my_ticket[it_idx];
            // std::cout << i << ", " << it_idx << ", " << input_data.my_ticket[it_idx] << std::endl;
        }
    }

    return ans;
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