#include <fstream>
#include <string>
#include <vector>

namespace custom_dict {

struct Dict {

    int word_len;
    std::vector<const char*> possible, guessable;

    Dict(const std::string& p, const std::string& g, int n);

    ~Dict();
};

} // namespace custom_dict
