#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "wordle_dict.hpp"

using namespace std;

int wordle(const char* word, const char* guess) {
    int result = 0;

    array<int, 26> cnt{};
    for (int i = 0; i < 5; ++i) {
        if (guess[i] != word[i]) cnt[word[i] - 'a']++;
    }

    int b = 1;
    for (int i = 0; i < 5; ++i) {
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

int result_to_int(const string& result) {
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
    array<int, 26> cnt{};
    array<bool, 26> has_b{};
    for (int i = 0; i < 5; ++i) {
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

    for (int i = 0; i < 5; ++i) {
        char ch = guess[i];
        if (cnt[ch - 'a'] > 0) return false;
        if (has_b[ch - 'a'] && cnt[ch - 'a'] != 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    bool verbose = false;
    for (int i = 1; i < argc; ++i) {
        string arg(argv[i]);
        if (arg == "-v") verbose = true;
    }

    vector<const char*> possible(
        wordle_dict::possible.begin(), wordle_dict::possible.end());
    vector<const char*> guessable(possible.begin(), possible.end());
    guessable.insert(guessable.end(), wordle_dict::guessable.begin(),
        wordle_dict::guessable.end());

    for (int t = 0;; ++t) {
        if (possible.empty()) {
            cerr << "No solutions possible" << endl;
            return 1;
        } else if (possible.size() == 1) {
            cout << possible[0] << endl;
            return 0;
        } else if (verbose) {
            cerr << possible.size() << " word";
            if (possible.size() != 1) cerr << "s";
            cerr << " remaining:";
            for (int i = 0; i < (int)possible.size(); ++i) {
                if (i >= 5) break;
                cerr << ' ' << possible[i];
            }
            if (possible.size() > 5) {
                cerr << " ...";
            }
            cerr << endl;
        }

        vector<int> score(guessable.size());

        transform(execution::par_unseq, guessable.begin(), guessable.end(),
            score.begin(), [&](const char* guess) {
                array<int, 243> cnt{};
                for (const char* target : possible) {
                    cnt[wordle(target, guess)]++;
                }
                int s = 0;
                for (int i = 0; i < 242; ++i) {
                    s += cnt[i] * cnt[i];
                }
                return s;
            });
        auto it1 =
            min_element(execution::par_unseq, score.begin(), score.end());
        const char* best = guessable[it1 - score.begin()];

        cout << best << endl;

        int result = [] {
            string tmp;
            cin >> tmp;
            return result_to_int(tmp);
        }();

        auto it2 = remove_if(execution::par_unseq, possible.begin(),
            possible.end(), [&](const char* word) {
                return !is_possible_word(word, best, result);
            });
        possible.erase(it2, possible.end());
    }

    return 0;
}
