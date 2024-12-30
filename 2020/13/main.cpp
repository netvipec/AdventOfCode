#include "BigInteger.hpp"

#include <bits/stdc++.h>

using input_t = std::pair<ll, std::map<ll, ll>>;
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
    std::getline(std::cin, line);
    std::istringstream iss(line);
    ll timestamp = -1;
    if (!(iss >> timestamp)) {
        return input_t{};
    } // error

    std::getline(std::cin, line);
    auto const bus = split(line, ',');

    std::map<ll, ll> bus_data;
    for (unsigned int i = 0; i < bus.size(); i++) {
        if (bus[i] != "x") {
            auto const bd_id = std::stoi(bus[i]);
            bus_data.emplace(i, bd_id);
        }
    }

    input_values.first = timestamp;
    input_values.second = bus_data;
    return input_values;
}

result_t solve1(input_t const& input_data) {
    result_t min_departed_offset = std::numeric_limits<ll>::max();
    ll best_bus_id = -1;
    for (auto const& bus_data : input_data.second) {
        auto const depart_offset = bus_data.second - input_data.first % bus_data.second;
        if (depart_offset < min_departed_offset) {
            min_departed_offset = depart_offset;
            best_bus_id = bus_data.second;
        }
    }
    return min_departed_offset * best_bus_id;
}

ll inverse(ll n, ll mod) {
    ll mod_bak = mod;
    n = n % mod;
    ll d = 0;
    if (n == 0 && d == 1)
        return 0;
    else if (n == 0 && d != 1) {
        std::cout << n << " is not inversibe mod " << mod << std::endl;
        return 9999;
    } else {
        ll x1 = 1;
        ll x2 = 0;
        ll y1 = 0;
        ll y2 = 1;
        ll q3 = 0;
        ll r = 0;
        ll y3 = 0;
        ll x3 = 0;
        while (n > 0) {
            q3 = mod / n;
            r = mod - q3 * n;
            x3 = x1 - q3 * x2;
            y3 = y1 - q3 * y2;
            mod = n;
            n = r;
            x1 = x2;
            y1 = y2;
            x2 = x3;
            y2 = y3;
        }
        if (mod == 1) {
            if (y1 < 0)
                y1 += mod_bak;
            return y1;
        } else {
            std::cout << n << " is not inversibe mod " << mod << std::endl;
            return 9999;
        }
    }
}

ll CRT(std::vector<ll> const& a, std::vector<ll> const& n) {
    if (a.size() != n.size()) {
        std::cout << "length don't match" << std::endl;
    }

    ll N = 1;
    std::for_each(n.begin(), n.end(), [&](ll i) {
        N *= i;
    });
    std::vector<ll> n_comp;
    std::vector<ll> M;
    std::for_each(n.begin(), n.end(), [&](ll i) {
        n_comp.push_back(N / i);
        M.push_back(inverse(N / i, i));
    });

    ll x = 0;
    for (size_t i = 0; i < n.size(); ++i) {
#if 0
        x += (ll)((Int(a[i]) * Int(n_comp[i]) * Int(M[i])) % N);
#else
        __int128_t ai(a[i]);
        ai *= n_comp[i];
        ai %= N;
        ai *= M[i];
        ai %= N;
        x += ai;
#endif
        x %= N;
    }

    return x % N;
}

result_t solve2(input_t const& input_data) {
    result_t ans = -1;
    std::vector<ll> n;
    std::vector<ll> a;
    std::for_each(std::cbegin(input_data.second), std::cend(input_data.second), [&](auto const& bus_data) {
        n.emplace_back(bus_data.second);
        a.emplace_back(bus_data.second - bus_data.first % bus_data.second);
    });
    ans = CRT(a, n);
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