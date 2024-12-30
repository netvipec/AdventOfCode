#include <bits/stdc++.h>

using ll = int64_t;
using input_t = std::pair<std::vector<ll>, std::vector<ll>>;
using result1_t = ll;
using result2_t = ll;

input_t read_input() {
    input_t input_values;

    std::string line;
    std::getline(std::cin, line);
    assert(line.find("Player ") == 0);
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        input_values.first.emplace_back(std::stoi(line));
    }
    std::getline(std::cin, line);
    assert(line.find("Player ") == 0);
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        input_values.second.emplace_back(std::stoi(line));
    }
    return input_values;
}

result1_t solve1(input_t const& input_data) {
    std::deque<ll> player1(std::cbegin(input_data.first), std::cend(input_data.first));
    std::deque<ll> player2(std::cbegin(input_data.second), std::cend(input_data.second));

    while (!player1.empty() && !player2.empty()) {
        assert(player1.front() != player2.front());
        if (player1.front() > player2.front()) {
            player1.push_back(player1.front());
            player1.push_back(player2.front());
        } else {
            player2.push_back(player2.front());
            player2.push_back(player1.front());
        }
        player1.pop_front();
        player2.pop_front();
    }

    auto const& winner = player1.empty() ? player2 : player1;

    // std::for_each(std::cbegin(winner), std::cend(winner), [](auto const& elem) {
    //     std::cout << elem << ", ";
    // });
    // std::cout << std::endl;

    ll idx = 1;
    result1_t ans =
        std::accumulate(std::crbegin(winner), std::crend(winner), 0ll, [&](auto const& base, auto const& elem) {
            return base + (idx++) * elem;
        });

    return ans;
}

void solve2_helper(std::deque<ll>& player1, std::deque<ll>& player2) {
    static int depth = 0;
    depth++;
    std::set<std::pair<std::deque<ll>, std::deque<ll>>> played_games;

    while (!player1.empty() && !player2.empty()) {
        assert(player1.front() != player2.front());

        auto const key = std::make_pair(player1, player2);
        auto const it = played_games.find(key);
        bool player1_won = false;
        if (it != std::cend(played_games)) {
            player1_won = true;
            // std::cout << "game stop from recursion loop: " << player1_won << ", d: " << depth << std::endl;
            player2.clear();
            break;
        } else {
            played_games.insert(key);
            if (player1.front() <= static_cast<ll>(player1.size()) - 1
                && player2.front() <= static_cast<ll>(player2.size()) - 1) {
                // std::cout << "new sub game: " << player1.front() << " - " << player2.front() << ", d: " << depth
                //           << std::endl;
                std::deque<ll> new_player1(std::cbegin(player1) + 1, std::cbegin(player1) + 1 + player1.front());
                std::deque<ll> new_player2(std::cbegin(player2) + 1, std::cbegin(player2) + 1 + player2.front());
                solve2_helper(new_player1, new_player2);
                player1_won = new_player2.empty();
            } else {
                // std::cout << "normal game: " << player1.front() << " - " << player2.front() << ", d: " << depth
                //           << std::endl;
                player1_won = player1.front() > player2.front();
            }
        }

        if (player1_won) {
            player1.push_back(player1.front());
            player1.push_back(player2.front());
        } else {
            player2.push_back(player2.front());
            player2.push_back(player1.front());
        }
        player1.pop_front();
        player2.pop_front();
    }

    // std::cout << "finish sub game with result: " << player2.empty() << ", d: " << depth << std::endl;
    depth--;
}

result2_t solve2(input_t const& input_data) {
    std::deque<ll> player1(std::cbegin(input_data.first), std::cend(input_data.first));
    std::deque<ll> player2(std::cbegin(input_data.second), std::cend(input_data.second));

    solve2_helper(player1, player2);
    auto const& winner = player1.empty() ? player2 : player1;

    // std::cout << "Player 1's deck: ";
    // std::for_each(std::cbegin(player1), std::cend(player1), [](auto const& elem) {
    //     std::cout << elem << ", ";
    // });
    // std::cout << std::endl;
    // std::cout << "Player 2's deck: ";
    // std::for_each(std::cbegin(player2), std::cend(player2), [](auto const& elem) {
    //     std::cout << elem << ", ";
    // });
    // std::cout << std::endl;

    ll idx = 1;
    result1_t ans =
        std::accumulate(std::crbegin(winner), std::crend(winner), 0ll, [&](auto const& base, auto const& elem) {
            return base + (idx++) * elem;
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