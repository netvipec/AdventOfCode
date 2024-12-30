#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::vector<std::string>;
using result_t = ll;

struct mask_data {
    uint64_t mask_and;
    uint64_t mask_or;
    std::vector<uint64_t> mask_floating;
};

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

mask_data get_mask(std::string const& mask) {
    uint64_t mask_and = std::numeric_limits<uint64_t>::max();
    uint64_t mask_or = 0;
    std::vector<uint64_t> mask_floating;

    uint64_t power2 = 1ull << (mask.size() - 1);
    for (auto const& bit : mask) {
        if (bit == '0') {
            mask_and ^= power2;
        } else if (bit == '1') {
            mask_or ^= power2;
        } else {
            mask_floating.emplace_back(power2);
        }
        power2 >>= 1;
    }

    return { mask_and, mask_or, mask_floating };
}

result_t solve1(input_t const& input_data) {
    result_t ans = -1;
    mask_data mask;
    std::map<uint64_t, uint64_t> mem;
    for (auto const& inst : input_data) {
        if (inst[1] == 'a') {
            // mask = 10011XXXX01010011000010X1X11100XXX00
            auto const mask_str = inst.substr(7);
            mask = get_mask(mask_str);
        } else {
            // mem[65524] = 4129175
            auto const idx1 = inst.find("]");
            assert(idx1 != std::string::npos);
            auto const address = std::stoi(inst.substr(4, idx1 - 4));

            auto const idx2 = inst.find("=");
            assert(idx2 != std::string::npos);
            auto const value = std::stoi(inst.substr(idx2 + 2));

            mem[address] = (value & mask.mask_and) | mask.mask_or;
        }
    }

    uint64_t const all_mask = (1ull << 36) - 1;
    ans = std::accumulate(std::cbegin(mem), std::cend(mem), 0ull, [=](auto const& base, auto const& elem) {
        return base + (all_mask & elem.second);
    });
    return ans;
}

std::vector<uint64_t> get_addresses(uint64_t address, mask_data const& mask) {
    std::vector<uint64_t> addresses;

    if (mask.mask_floating.size() > 0) {
        std::vector<bool> v(mask.mask_floating.size());
        for (size_t r = 0; r <= v.size(); r++) {
            std::fill(v.begin(), v.begin() + r, true);

            do {
                uint64_t ad_and = std::numeric_limits<uint64_t>::max();
                uint64_t ad_or = 0;
                for (size_t i = 0; i < v.size(); ++i) {
                    if (v[i]) {
                        ad_or ^= mask.mask_floating[i];
                    } else {
                        ad_and ^= mask.mask_floating[i];
                    }
                }
                addresses.emplace_back(((address | mask.mask_or) & ad_and) | ad_or);
            } while (std::prev_permutation(v.begin(), v.end()));
        }
    } else {
        addresses.emplace_back(address | mask.mask_or);
    }

    return addresses;
}

result_t solve2(input_t const& input_data) {
    result_t ans = -1;
    mask_data mask;
    std::map<uint64_t, uint64_t> mem;
    for (auto const& inst : input_data) {
        if (inst[1] == 'a') {
            // mask = 10011XXXX01010011000010X1X11100XXX00
            auto const mask_str = inst.substr(7);
            mask = get_mask(mask_str);
        } else {
            // mem[65524] = 4129175
            auto const idx1 = inst.find("]");
            assert(idx1 != std::string::npos);
            auto const address = std::stoi(inst.substr(4, idx1 - 4));

            auto const idx2 = inst.find("=");
            assert(idx2 != std::string::npos);
            auto const value = std::stoi(inst.substr(idx2 + 2));

            auto const addresses = get_addresses(address, mask);

            for (auto const& addr : addresses) {
                mem[addr] = value;
            }
        }
    }

    uint64_t const all_mask = (1ull << 36) - 1;
    ans = std::accumulate(std::cbegin(mem), std::cend(mem), 0ull, [=](auto const& base, auto const& elem) {
        return base + (all_mask & elem.second);
    });
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