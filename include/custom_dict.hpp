#include <fstream>
#include <string>
#include <vector>

namespace custom_dict {

struct Dict {

    std::vector<const char*> possible, guessable;

    Dict(const std::string& p, const std::string& g);

    ~Dict();
};

} // namespace custom_dict
