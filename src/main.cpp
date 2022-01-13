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

string wordle(const string& word, const string& guess) {
    string result(5, '?');

    array<int, 26> cnt {};
    for (int i = 0; i < 5; ++i) {
        if (guess[i] != word[i]) cnt[word[i] - 'a']++;
    }

    for (int i = 0; i < 5; ++i) {
        if (guess[i] == word[i]) {
            result[i] = 'g';
        } else if (cnt[guess[i] - 'a'] > 0) {
            result[i] = 'y';
            cnt[guess[i] - 'a']--;
        } else {
            result[i] = 'b';
        }
    }

    return result;
}

bool is_possible_word(
    const string& word, const string& guess, const string& result) {

    array<int, 26> cnt {};
    array<bool, 26> has_b {};
    for (int i = 0; i < 5; ++i) {
        char ch = guess[i];
        if (result[i] == 'g') {
            if (word[i] != ch) return false;
        } else {
            if (word[i] == ch) return false;
            cnt[word[i] - 'a']--;
            if (result[i] == 'y') {
                cnt[ch - 'a']++;
            } else if (result[i] == 'b') {
                has_b[ch - 'a'] = true;
            }
        }
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

    vector<string> words, wordles;
    if (!(read_data("./data/solutions.txt", words) &&
            read_data("./data/nonsolutions.txt", wordles))) {
        cerr << "Unable to read file" << endl;
        return 1;
    }

    wordles.insert(wordles.end(), words.begin(), words.end());

    for (int t = 0;; ++t) {
        if (words.empty()) {
            cerr << "No solutions possible" << endl;
            return 1;
        }
        if (words.size() == 1) {
            cout << words[0] << endl;
            return 0;
        }

        vector<int> score(wordles.size());

        string best = "roate";
        if (t > 0) {
            transform(execution::par_unseq, wordles.begin(), wordles.end(),
                score.begin(), [&](const string& word) {
                    int s = 0;
                    for (const string& target : words) {
                        s += count_if(
                            words.begin(), words.end(), [&](const string& w) {
                                return is_possible_word(
                                    w, word, wordle(target, word));
                            });
                    }
                    return s;
                });
            auto it =
                min_element(execution::par_unseq, score.begin(), score.end());
            best = wordles[it - score.begin()];
        }

        cout << best << endl;

        string result;
        cin >> result;

        auto it = remove_if(execution::par_unseq, words.begin(), words.end(),
            [&](const string& word) {
                return !is_possible_word(word, best, result);
            });
        words.erase(it, words.end());

        if (verbose) {
            cerr << words.size() << " word";
            if (words.size() != 1) cerr << "s";
            cerr << " remaining" << endl;
        }
    }

    return 0;
}
