# Unix Utilities Assignment

This repository contains implementations of four basic Unix utilities in C:

1. **pwd** - Prints the current working directory.
2. **echo** - Prints a user-provided string to standard output.
3. **cp** - Copies a source file to a specified destination.
4. **mv** - Moves (renames) a file to another location.

Each utility is implemented as a separate C program.

---

## Compilation

To compile all utilities, use the following commands:

```bash
gcc -o pwd pwd.c
gcc -o echo echo.c
gcc -o cp cp.c
gcc -o mv mv.c

## Usage

### `pwd`

Prints the current working directory.

```bash
./pwd
