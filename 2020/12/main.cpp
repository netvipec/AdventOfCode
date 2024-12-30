#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::pair<char, ll>>;
using result_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        std::istringstream iss(line);
        char d;
        ll lenght;
        if (!(iss >> d >> lenght)) {
            break;
        } // error

        input_values.emplace_back(d, lenght);
    }
    return input_values;
}

const std::array<char, 4> D{ 'N', 'E', 'S', 'W' };
const std::array<ll, 4> DX{ 0, 1, 0, -1 };
const std::array<ll, 4> DY{ 1, 0, -1, 0 };

void move(std::pair<ll, ll>& pos, char d, ll units) {
    auto const it_d = std::find(std::cbegin(D), std::cend(D), d);
    assert(it_d != std::cend(D));
    auto const idx_d = std::distance(std::cbegin(D), it_d);
    pos.first += DX[idx_d] * units;
    pos.second += DY[idx_d] * units;
}

void move(std::pair<ll, ll>& pos, std::pair<ll, ll> const& wp, char d, ll units) {
    auto const it_d = std::find(std::cbegin(D), std::cend(D), d);
    assert(it_d != std::cend(D));
    auto const idx_d = std::distance(std::cbegin(D), it_d);
    pos.first += (DX[idx_d] + DY[idx_d]) * units * wp.first;
    pos.second += (DX[idx_d] + DY[idx_d]) * units * wp.second;
}

ll rotate90_helper(ll d, ll degree, ll mult) {
    assert(degree % 90 == 0);

    auto const it = std::find(std::cbegin(D), std::cend(D), d);
    auto idx = std::distance(std::cbegin(D), it);
    idx = (idx + mult * degree / 90) % 4;
    d = D[idx];
    return d;
}
ll rotate90_left(ll d, ll degree) { return rotate90_helper(d, degree, 3); }
ll rotate90_right(ll d, ll degree) { return rotate90_helper(d, degree, 1); }

void rotate_point_helper(std::pair<ll, ll>& p, ll degree) {
    auto const angle = 2 * M_PI * degree / 360;
    auto nx = p.first * cos(angle) - p.second * sin(angle);
    auto ny = p.first * sin(angle) + p.second * cos(angle);

    p.first = static_cast<int>(std::round(nx));
    p.second = static_cast<int>(std::round(ny));
}

void rotate_point_left(std::pair<ll, ll>& p, ll degree) { rotate_point_helper(p, degree); }
void rotate_point_right(std::pair<ll, ll>& p, ll degree) { rotate_point_helper(p, 360 - degree); }

void rotate_point90_helper(std::pair<ll, ll>& p, ll degree, bool left) {
    for (ll i = 0; i < degree / 90; i++) {
        if (left) {
            p = std::make_pair(-p.second, p.first);
        } else {
            p = std::make_pair(p.second, -p.first);
        }
    }
}
void rotate_point90_left(std::pair<ll, ll>& p, ll degree) { rotate_point90_helper(p, degree, true); }
void rotate_point90_right(std::pair<ll, ll>& p, ll degree) { rotate_point90_helper(p, degree, false); }

void move1(std::pair<ll, ll>& pos, char& d, input_t::value_type const& w) {
    switch (w.first) {
        case 'N':
        case 'S':
        case 'E':
        case 'W':
            move(pos, w.first, w.second);
            break;
        case 'L':
            d = rotate90_left(d, w.second);
            break;
        case 'R':
            d = rotate90_right(d, w.second);
            break;
        case 'F':
            move(pos, d, w.second);
            break;
        default:
            assert(false);
            break;
    }
}

void move2(std::pair<ll, ll>& pos, std::pair<ll, ll>& wp, char& d, input_t::value_type const& w) {
    switch (w.first) {
        case 'N':
        case 'S':
        case 'E':
        case 'W':
            move(wp, w.first, w.second);
            break;
        case 'L': {
            rotate_point90_left(wp, w.second);
            break;
        }
        case 'R': {
            rotate_point90_right(wp, w.second);
            break;
        }
        case 'F':
            move(pos, wp, d, w.second);
            break;
        default:
            assert(false);
            break;
    }
}

result_t solve1(input_t const& input_data) {
    result_t ans = -1;
    std::pair<ll, ll> pos{ 0, 0 };
    char d = 'E';

    for (auto const& w : input_data) {
        move1(pos, d, w);
    }

    ans = std::abs(pos.first) + std::abs(pos.second);

    return ans;
}

result_t solve2(input_t const& input_data) {
    result_t ans = -1;
    std::pair<ll, ll> pos{ 0, 0 };
    char d = 'E';
    std::pair<ll, ll> wp{ 10, 1 };

    for (auto const& w : input_data) {
        move2(pos, wp, d, w);
    }

    ans = std::abs(pos.first) + std::abs(pos.second);
    return ans;
}

int main() {
    auto begin_input = std::chrono::steady_clock::now();
    auto const input_data = read_input();
    auto end_input = std::chrono::steady_clock::now();
    // std::for_each(std::cbegin(input_data), std::cend(input_data), [](auto const& elem) {
    //     std::cout << elem.first << elem.second << std::endl;
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