# Poker Game (C, Texas Hold'em)

Small learning project in C with a modular layout:
- src: implementation files (.c)
- include: header files (.h)
- bin: compiled output

## Quick Start

From the project root, run:

```bash
mingw32-make run
```

That will build and run the game.

## Common Commands

Build only:

```bash
mingw32-make
```

Run (builds first if needed):

```bash
mingw32-make run
```

Clean build artifacts:

```bash
mingw32-make clean
```

## Manual Build (without Makefile)

```bash
gcc -Wall -Wextra -Wpedantic -Werror -std=c11 -Iinclude src/poker.c src/card.c src/deck.c src/hand_eval.c src/holdem.c -o bin/poker.exe
./bin/poker.exe
```
