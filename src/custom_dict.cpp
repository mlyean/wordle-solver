#include "custom_dict.hpp"

#include <sstream>

namespace custom_dict {

Dict::Dict(const std::string& p, const std::string& g, int n) : word_len(n) {
    std::string word;

    std::fstream pfile(p, std::ios::in);
    if (!pfile.is_open()) {
        std::stringstream ss;
        ss << "Failed to read file '" << p << "'";
        throw ss.str();
    }
    while (getline(pfile, word)) {
        if (word.size() != (size_t)n) {
            std::stringstream ss;
            ss << "Length of '" << word << "' in " << p << " does not match "
               << n;
            throw ss.str();
        };
        char* a = new char[n + 1];
        copy(word.begin(), word.end(), a);
        a[n] = '\0';
        possible.push_back(a);
    }
    pfile.close();

    std::fstream gfile(g, std::ios::in);
    if (!gfile.is_open()) {
        std::stringstream ss;
        ss << "Failed to read file '" << g << "'";
        throw ss;
    }
    while (getline(gfile, word)) {
        if (word.size() != (size_t)n) {
            std::stringstream ss;
            ss << "Length of '" << word << "' in " << g << " does not match "
               << n;
            throw ss.str();
        };
        char* a = new char[n + 1];
        copy(word.begin(), word.end(), a);
        a[n] = '\0';
        guessable.push_back(a);
    }
    gfile.close();
}

Dict::~Dict() {
    for (const char* a : possible)
        delete[] a;
    for (const char* a : guessable)
        delete[] a;
}

} // namespace custom_dict
