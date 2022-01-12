#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <execution>

using namespace std;

string wordle(const string& word, const string& guess) {
    string result(5, '?');

    for (int i = 0; i < 5; ++i) {
        if (guess[i] == word[i]) {
            result[i] = 'g';
        } else {
            int match = 0, perm = 0, total = 0;
            for (int j = 0; j < 5; ++j) {
                if (guess[i] == guess[j] && word[j] == guess[j]) match++;
                if (word[j] == guess[i]) total++;
                if (j < i && guess[i] == guess[j] && word[j] != guess[j])
                    perm++;
            }
            result[i] = (match + perm < total) ? 'y' : 'b';
        }
    }

    return result;
}

bool is_possible_word(const string& word, const string& guess,
                      const string& result) {
    for (int i = 0; i < 5; ++i) {
        char ch = guess[i];
        int mc = 0, cnt = 0;
        if (result[i] == 'g') {
            if (word[i] != ch) return false;
        } else {
            if (word[i] == ch) return false;
            for (int j = 0; j < 5; ++j) {
                if (guess[j] == ch && (result[j] == 'g' || result[j] == 'y'))
                    mc++;
                if (word[j] == ch) ++cnt;
            }
            if (result[i] == 'y') {
                if (!(cnt >= mc)) return false;
            } else if (result[i] == 'b') {
                if (!(cnt <= mc)) return false;
            }
        }
    }
    return true;
}

int main() {
    vector<string> solutions, nonsolutions;

    fstream file;
    file.open("solutions.txt", ios::in);
    if (file.is_open()) {
        string word;
        while (getline(file, word)) {
            solutions.push_back(word);
        }
        file.close();
    }
    file.open("nonsolutions.txt", ios::in);
    if (file.is_open()) {
        string word;
        while (getline(file, word)) {
            nonsolutions.push_back(word);
        }
        file.close();
    }

    vector<string> words(solutions.begin(), solutions.end());
    vector<pair<int, string>> wordles;
    for (auto& word : solutions) {
        int i = wordles.size();
        wordles.emplace_back(i, word);
    }
    for (auto& word : nonsolutions) {
        int i = wordles.size();
        wordles.emplace_back(i, word);
    }

    for (int t = 0;; ++t) {
        if (words.empty()) {
            cout << "No solutions" << endl;
            return 1;
        }
        if (words.size() == 1) {
            cout << words[0] << endl;
            return 0;
        }

        vector<double> score(wordles.size());

        string best = "roate";
        if (t > 0) {
            for_each(execution::par_unseq, wordles.begin(), wordles.end(), [&](const auto& param){
                const auto& [i, word] = param;
                for (auto& target : words) {
                    for (auto& w : words) {
                        if (is_possible_word(w, word, wordle(target, word))) score[i]++;
                    }
                }
            });
            double best_score = words.size() * words.size();
            for (auto& [i, word] : wordles) {
                if (score[i] < best_score) {
                    best = word;
                    best_score = score[i];
                }
            }
        }

        cout << best << endl;

        string result;
        cin >> result;

        vector<string> words_new;
        for (auto& word : words) {
            if (is_possible_word(word, best, result)) words_new.push_back(word);
        }
        words = words_new;
    }

    return 0;
}
