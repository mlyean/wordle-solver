# Wordle Solver

A bot / solver for the game [Wordle](https://www.powerlanguage.co.uk/wordle/). The solver's strategy is to make guesses which on average narrow down the number of possibilities at much as possible.

## Compiling

Run `make` to compile.

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

To test the program on a word of your choice, run
```
./profile.py <word>
```
where `<word>` is replaced by a valid word.

### Example

An example session solved in 3 guesses:
```
$ ./bin/main -v
2315 words remaining: aback abase abate abbey abbot ...
roate
bgbbb
71 words remaining: bobby bongo bonus booby boozy ...
bludy
bygyb
2 words remaining: could would
could
ggggg
could
```

## Statistics

`./profile.py` yields the following results:

| Attempts | Number of words |
| --------:| ---------------:|
| 2        | 55              |
| 3        | 1130            |
| 4        | 1090            |
| 5        | 40              |

Mean, `μ = 3.48164`.

Standard deviation, `σ = 0.57609`.
