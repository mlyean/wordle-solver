# Wordle Solver

## Compiling

Simply run `make` to compile.

## Usage

From the project root, run `./bin/main`.

Upon execution, the program provides its guess. The user then needs to input the result of guess, in the form of a string of 5 characters, consisting of `g`, `y`, and `b` (corresponding to the colors):

| Character | Meaning                    |
|:---------:| -------------------------- |
| `g`       | Occurs in the correct spot |
| `y`       | Occurs in the wrong spot   |
| `b`       | Does not occur             |

This is then repeated until the answer is obtained.

To print the number of possibilities remaining, pass the `-v` flag.

**Note:** The directory in which you run the program must contain `data/`. The list of valid answers are in `solutions.txt`, the other words are located in `nonsolutions.txt`.

To test the program on a word of your choice, run
```
./profile.py <word>
```
where `<word>` is replaced by a valid word.

## Statistics

`./profile.py` yields the following output:
```
N = 2315, μ = 3.48164, σ = 0.57609, max = 5
1: 0, 2: 55, 3: 1130, 4: 1090, 5: 40, 6: 0, fail: 0
```
This demonstrates that at most 5 attempts are needed for each valid word.
