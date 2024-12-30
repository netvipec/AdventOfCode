#include <bits/stdc++.h>

using ll = int64_t;
using input_t = ll;
using result1_t = ll;
using result2_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        ll a;
        if (!(iss >> a)) {
            break;
        } // error

        input_values = a;
    }
    return input_values;
}

result1_t solve1(input_t const& input_data) {
    std::vector<ll> circle;
    std::string const id = std::to_string(input_data);
    std::transform(std::cbegin(id), std::cend(id), std::back_inserter(circle), [](auto const& elem) {
        return std::stoi(std::string{ elem });
    });

    for (ll m = 0; m < 100; m++) {
        ll curr_value = circle[0];

        ll max_value = std::numeric_limits<ll>::min();
        ll max_value_pos = -1;
        ll closest_value = std::numeric_limits<ll>::max();
        ll closest_value_pos = -1;
        for (ll i = 4; i < static_cast<int>(circle.size()); i++) {
            if (max_value < circle[i]) {
                max_value = circle[i];
                max_value_pos = i;
            }
            if (curr_value > circle[i] && curr_value - circle[i] - 1 < closest_value) {
                closest_value = curr_value - circle[i] - 1;
                closest_value_pos = i;
            }
        }

        ll destination = ((closest_value_pos != -1) ? closest_value_pos : max_value_pos) + 1;
        std::vector<ll> c(std::cbegin(circle) + 1, std::cbegin(circle) + 4);
        std::vector<ll> nc(std::cbegin(circle) + 4, std::cend(circle));
        nc.push_back(curr_value);
        nc.insert(std::cbegin(nc) + (destination - 4), std::cbegin(c), std::cend(c));
        std::swap(nc, circle);

        // std::cout << "Move : " << m + 2 << ", current: " << curr_value << std::endl;
        // std::for_each(std::cbegin(circle), std::cend(circle), [](auto const& elem) {
        //     std::cout << elem << ", ";
        // });
        // std::cout << std::endl;
    }

    auto it1 = std::find(std::cbegin(circle), std::cend(circle), 1);
    std::string ans_str =
        std::accumulate(it1 + 1,
                        std::cend(circle),
                        std::string{},
                        [](auto const& base, auto const& elem) {
                            return base + std::to_string(elem);
                        })
        + std::accumulate(std::cbegin(circle), it1, std::string{}, [](auto const& base, auto const& elem) {
              return base + std::to_string(elem);
          });

    result1_t ans = std::stoi(ans_str);
    return ans;
}

struct node_t {
    ll value;
    node_t* next;
};

result2_t solve2(input_t const& input_data) {
    constexpr ll target = 1'000'000ll;
    constexpr ll move_target = 10'000'000ll;
    // constexpr ll target = 9;

    std::vector<std::unique_ptr<node_t>> iterators(target);
    std::string const id = std::to_string(input_data);
    ll first_value = std::stoi(std::string{ id[0] });
    ll last_value = -1;
    for (auto const& i : id) {
        std::unique_ptr<node_t> n(new node_t{ std::stoi(std::string{ i }), nullptr });
        if (last_value != -1) {
            iterators[last_value - 1]->next = n.get();
        }
        last_value = n->value;
        iterators[n->value - 1].reset(n.release());
    }
    for (ll i = 10; i <= target; i++) {
        std::unique_ptr<node_t> n(new node_t{ i, nullptr });
        iterators[last_value - 1]->next = n.get();
        last_value = n->value;
        iterators[n->value - 1].reset(n.release());
    }
    iterators[last_value - 1]->next = iterators[first_value - 1].get();

    node_t* current = iterators[first_value - 1].get();
    std::array<std::pair<ll, bool>, 5> destination_candidates;
    for (ll m = 0; m < move_target; m++) {
        auto value = current->value - 1;
        for (int i = 0; i < static_cast<int>(destination_candidates.size()); i++, value--) {
            if (value == 0) {
                value = target;
            }
            destination_candidates[i] = { value, false };
        }

        auto cp = current;
        auto c = current->next;
        for (int i = 0; i < 3; i++, c = c->next, cp = cp->next) {
            auto it = std::find_if(std::begin(destination_candidates),
                                   std::end(destination_candidates),
                                   [&](auto const& elem) {
                                       return elem.first == c->value;
                                   });
            if (it != std::end(destination_candidates)) {
                it->second = true;
            }
        }

        auto dest_it =
            std::find_if(std::begin(destination_candidates), std::end(destination_candidates), [](auto const& elem) {
                return elem.second == false;
            });

        assert(dest_it != std::cend(destination_candidates));

        cp->next = iterators[dest_it->first - 1]->next;
        iterators[dest_it->first - 1]->next = current->next;
        current->next = c;

        current = current->next;

        // auto it1 = current;
        // auto it1_end = current;
        // do {
        //     std::cout << it1->value << " ";
        //     it1 = it1->next;
        // } while (it1 != it1_end);
        // std::cout << std::endl;
    }

    std::cout << iterators[0]->next->value << " * " << iterators[0]->next->next->value << std::endl;

    return iterators[0]->next->value * iterators[0]->next->next->value;
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