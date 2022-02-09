#include "solver.hpp"

#include <algorithm>
#include <cstring>
#include <execution>

namespace wordle_solver {

int wordle(const char* word, const char* guess) {
    int len = strlen(word);
    int result = 0;

    std::array<int, 26> cnt{};
    for (int i = 0; i < len; ++i) {
        if (guess[i] != word[i]) cnt[word[i] - 'a']++;
    }

    int b = 1;
    for (int i = 0; i < len; ++i) {
        if (guess[i] == word[i]) {
            result += 2 * b;
        } else if (cnt[guess[i] - 'a'] > 0) {
            cnt[guess[i] - 'a']--;
            result += b;
        }
        b *= 3;
    }

    return result;
}

int result_to_int(const std::string& result) {
    int ans = 0;
    int b = 1;

    for (char c : result) {
        if (c == 'g') {
            ans += 2 * b;
        } else if (c == 'y') {
            ans += b;
        }
        b *= 3;
    }

    return ans;
}

bool is_possible_word(const char* word, const char* guess, int result) {
    int len = strlen(word);
    std::array<int, 26> cnt{};
    std::array<bool, 26> has_b{};
    for (int i = 0; i < len; ++i) {
        char ch = guess[i];
        if (result % 3 == 2) {
            if (word[i] != ch) return false;
        } else {
            if (word[i] == ch) return false;
            cnt[word[i] - 'a']--;
            if (result % 3 == 1) {
                cnt[ch - 'a']++;
            } else if (result % 3 == 0) {
                has_b[ch - 'a'] = true;
            }
        }
        result /= 3;
    }

    for (int i = 0; i < len; ++i) {
        char ch = guess[i];
        if (cnt[ch - 'a'] > 0) return false;
        if (has_b[ch - 'a'] && cnt[ch - 'a'] != 0) return false;
    }
    return true;
}

Solver::Solver(const dict::Dict& d) {
    possible.insert(possible.end(), d.possible.begin(), d.possible.end());
    guessable.insert(guessable.end(), d.guessable.begin(), d.guessable.end());

    word_len = strlen(guessable[0]);

    word_len3 = 1;
    for (int i = 0; i < word_len; ++i)
        word_len3 *= 3;
};

bool Solver::has_solution() { return !possible.empty(); }

int Solver::num_solutions() { return possible.size(); }

const char* Solver::guess() {
    if (possible.empty()) return nullptr;

    std::vector<int> score(guessable.size());

    transform(std::execution::par_unseq, guessable.begin(), guessable.end(),
        score.begin(), [&](const char* guess) {
            std::vector<int> cnt(word_len3);
            for (const char* target : possible)
                cnt[wordle(target, guess)]++;

            int s = 0;
            for (int i = 0; i < word_len3 - 1; ++i)
                s += cnt[i] * cnt[i];

            return s;
        });
    auto it =
        min_element(std::execution::par_unseq, score.begin(), score.end());

    return guessable[it - score.begin()];
}

void Solver::update(const char* last_guess, int result) {
    auto it = remove_if(std::execution::par_unseq, possible.begin(),
        possible.end(), [&](const char* word) {
            return !is_possible_word(word, last_guess, result);
        });
    possible.erase(it, possible.end());
};

} // namespace wordle_solver
