#include <fstream>
#include <iostream>
#include <string>

#include "custom_dict.hpp"
#include "solver.hpp"
#include "wordle_dict.hpp"

using namespace std;

int loop(bool verbose, dict::Dict d) {
    using namespace wordle_solver;
    Solver solver(d);

    for (int t = 0;; ++t) {
        if (!solver.has_solution()) {
            cerr << "No solutions possible" << endl;
            return 1;
        } else if (solver.num_solutions() == 1) {
            cout << solver.possible[0] << endl;
            return 0;
        } else if (verbose) {
            cerr << solver.num_solutions() << " word";
            if (solver.num_solutions() != 1) cerr << "s";
            cerr << " remaining:";
            for (int i = 0; i < min(solver.num_solutions(), 5); ++i)
                cerr << ' ' << solver.possible[i];
            if (solver.num_solutions() > 5) cerr << " ...";
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
}

int main(int argc, char* argv[]) {
    bool verbose = false;
    bool custom = false;
    string pfile, gfile;
    int word_len = 5;
    for (int i = 1; i < argc; ++i) {
        string arg(argv[i]);
        if (arg == "-v") {
            verbose = true;
        } else if (arg.substr(0, 3) == "-p=") {
            pfile = arg.substr(3);
            custom = true;
        } else if (arg.substr(0, 3) == "-g=") {
            gfile = arg.substr(3);
            custom = true;
        } else if (arg.substr(0, 3) == "-n=") {
            word_len = stoi(arg.substr(3));
        }
    }

    if (custom) {
        try {
            dict::CustomDict dict(pfile, gfile, word_len);
            return loop(verbose, dict);
        } catch (string e) {
            cerr << "Exception: " << e << endl;
            return 1;
        }
    }
    return loop(verbose, dict::wordle_common);
}
