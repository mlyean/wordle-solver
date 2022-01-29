#include <string>
#include <vector>

namespace wordle_solver {

int wordle(const char* word, const char* guess);

int result_to_int(const std::string& result);

bool is_possible_word(const char* word, const char* guess, int result);

struct Solver {

    std::vector<const char*> possible, guessable;

    Solver(
        const std::vector<const char*>& p, const std::vector<const char*>& g);

    bool has_solution();

    int num_solutions();

    const char* guess();

    void update(const char* last_guess, int result);
};

} // namespace wordle_solver
