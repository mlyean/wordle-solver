#!/usr/bin/env python
import sys
from subprocess import Popen, PIPE
from random import shuffle
from math import sqrt

def wordle(word, guess):
    result = []
    for i in range(5):
        if guess[i] == word[i]:
            result.append('g')
        elif sum(guess[j] == word[j] == guess[i] for j in range(5)) + sum(guess[j] == guess[i] and guess[j] != word[j] for j in range(i)) < word.count(guess[i]):
            result.append('y')
        else:
            result.append('b')
    return "".join(result)


if __name__ == "__main__":
    if len(sys.argv) == 1:
        with open("./data/solutions.txt", 'r') as f:
            solutions = list(map(str.strip, f.readlines()))

        shuffle(solutions)

        n = 0
        s = 0
        s2 = 0
        mx = 0
        stats = [0] * 7

        for word in solutions:
            attempts = 0
            with Popen(["./bin/main"], stdin=PIPE, stdout=PIPE, text=True) as proc:
                while True:
                    attempts += 1
                    guess = proc.stdout.readline().strip()

                    result = wordle(word, guess)

                    if result == "ggggg":
                        proc.kill()
                        break

                    proc.stdin.write(result)
                    proc.stdin.write('\n')
                    proc.stdin.flush()

            n += 1
            s += attempts
            s2 += attempts ** 2
            mx = max(mx, attempts)
            if attempts <= 6:
                stats[attempts] += 1
            else:
                stats[0] += 1

            if n >= 2:
                sys.stdout.write('\r')
                sys.stdout.write(f"N = {n}, μ = {s / n:.5f}, σ = {sqrt((s2 - (s ** 2) / n) / (n - 1)):.5f}, max = {mx}")
                sys.stdout.flush()

        print()
        print(f"1: {stats[1]}, 2: {stats[2]}, 3: {stats[3]}, 4: {stats[4]}, 5: {stats[5]}, 6: {stats[6]}, fail: {stats[0]}")

    else:
        word = sys.argv[1]
        with Popen(["./bin/main", "-v"], stdin=PIPE, stdout=PIPE, stderr=sys.stderr, text=True) as proc:
            attempts = 0
            while True:
                attempts += 1
                guess = proc.stdout.readline().strip()
                print(f"Guess : {guess}")

                result = wordle(word, guess)
                print(f"Result: {result}")

                proc.stdin.write(result)
                proc.stdin.write('\n')
                proc.stdin.flush()

                if result == "ggggg":
                    break

            print(f"Finished in {attempts} attempts")
