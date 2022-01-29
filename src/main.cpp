#include <iostream>
#include <string>

#include "solver.hpp"
#include "wordle_dict.hpp"

using namespace std;
using namespace wordle_solver;

int main(int argc, char* argv[]) {
    bool verbose = false;
    for (int i = 1; i < argc; ++i) {
        string arg(argv[i]);
        if (arg == "-v") verbose = true;
    }

    Solver solver(wordle_dict::possible, wordle_dict::guessable);

    for (int t = 0;; ++t) {
        if (!solver.has_solution()) {
            cerr << "No solutions possible" << endl;
            return 1;
        } else if (solver.possible.size() == 1) {
            cout << solver.possible[0] << endl;
            return 0;
        } else if (verbose) {
            cerr << solver.possible.size() << " word";
            if (solver.possible.size() != 1) cerr << "s";
            cerr << " remaining:";
            for (int i = 0; i < min((int)solver.possible.size(), 5); ++i)
                cerr << ' ' << solver.possible[i];
            if (solver.possible.size() > 5) cerr << " ...";
            cerr << endl;
        }

        const char* guess = solver.guess();

        cout << guess << endl;

        int result = [] {
            string tmp;
            cin >> tmp;
            return result_to_int(tmp);
        }();

        solver.update(guess, result);
    }

    return 0;
}
