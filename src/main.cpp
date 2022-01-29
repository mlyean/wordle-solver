#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool read_data(const string& filename, vector<string>& data) {
    fstream file(filename, ios::in);

    if (!file.is_open()) return false;

    string word;

    while (getline(file, word))
        data.push_back(word);

    file.close();

    return true;
}

int wordle(const string& word, const string& guess) {
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

bool is_possible_word(const string& word, const string& guess, int result) {
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

    vector<string> possible;
    if (!read_data("./data/solutions.txt", possible)) {
        cerr << "Unable to read file" << endl;
        return 1;
    }
    vector<string> guessable(possible.begin(), possible.end());
    if (!read_data("./data/nonsolutions.txt", guessable)) {
        cerr << "Unable to read file" << endl;
        return 1;
    }

    for (int t = 0;; ++t) {
        if (possible.empty()) {
            cerr << "No solutions possible" << endl;
            return 1;
        } else if (possible.size() == 1) {
            cout << possible[0] << endl;
            return 0;
        }

        vector<int> score(guessable.size());

        transform(execution::par_unseq, guessable.begin(), guessable.end(),
            score.begin(), [&](const string& guess) {
                array<int, 243> cnt{};
                for (const string& target : possible) {
                    cnt[wordle(target, guess)]++;
                }
                int s = 0;
                for (int i : cnt) {
                    s += i * i;
                }
                return s;
            });
        auto it1 =
            min_element(execution::par_unseq, score.begin(), score.end());
        string best = guessable[it1 - score.begin()];

        cout << best << endl;

        int result = []{
            string tmp;
            cin >> tmp;
            return result_to_int(tmp);
        }();

        auto it2 = remove_if(execution::par_unseq, possible.begin(),
            possible.end(), [&](const string& word) {
                return !is_possible_word(word, best, result);
            });
        possible.erase(it2, possible.end());

        if (verbose) {
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
    }

    return 0;
}
