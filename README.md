# Wordle Solver

## Compiling

```
g++ main.cpp -std=gnu++17 -ltbb -O2 -o main
```

## Usage

Upon execution, the program provides its guess. The user then needs to input the result of guess, in the form of a string of 5 characters, consisting of `g`, `y`, and `b` (corresponding to the colors):

| Character | Meaning                    |
|:---------:| -------------------------- |
| `g`       | Occurs in the correct spot |
| `y`       | Occurs in the wrong spot   |
| `b`       | Does not occur             |

This is then repeated until the answer is obtained.

**Note:** The list of valid answers are in `solutions.txt`, the other words are located in `nonsolutions.txt`.

To test the program on a word of your choice, run

```
./profile.py <word>
```
where `<word>` is replaced by a valid word.

## Statistics

`./profile.py` yields the following output:
```
N = 2315, μ = 3.65486, σ = 0.55906, max = 5
```
This demonstrates that at most 5 attempts are needed for each valid word.
