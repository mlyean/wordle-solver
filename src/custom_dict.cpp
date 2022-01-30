#include "custom_dict.hpp"

namespace custom_dict {

Dict::Dict(const std::string& p, const std::string& g) {
    std::string word;

    std::fstream pfile(p, std::ios::in);
    while (getline(pfile, word)) {
        char* a = new char[12];
        copy(word.begin(), word.end(), a);
        a[11] = '\0';
        possible.push_back(a);
    }
    pfile.close();

    std::fstream gfile(g, std::ios::in);
    while (getline(gfile, word)) {
        char* a = new char[12];
        copy(word.begin(), word.end(), a);
        a[11] = '\0';
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
