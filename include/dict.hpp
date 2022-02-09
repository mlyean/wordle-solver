#ifndef DICT_H
#define DICT_H

#include <vector>

namespace dict {

struct Dict {

    int word_len;
    std::vector<const char*> possible, guessable;
};

} // namespace dict

#endif
