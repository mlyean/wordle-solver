#include <string>
#include <vector>

#include "dict.hpp"

namespace wordle_solver {

int wordle(const char* word, const char* guess);

int result_to_int(const std::string& result);

bool is_possible_word(const char* word, const char* guess, int result);

struct Solver {

    int word_len, word_len3;
    std::vector<const char*> possible;
    const std::vector<const char*>& guessable;

    Solver(const dict::Dict& d);

    bool has_solution();

    int num_solutions();

    int eval(const char* guess);

    const char* guess();

    void update(const char* last_guess, int result);
};

} // namespace wordle_solver
