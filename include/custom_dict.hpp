#include <string>
#include <vector>

#include "dict.hpp"

namespace dict {

struct CustomDict : Dict {

    CustomDict(const std::string& p, const std::string& g, int n);

    ~CustomDict();
};

} // namespace dict
