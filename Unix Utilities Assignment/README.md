# Unix Utilities Implementation

This repository contains implementations of the following simple Unix utilities in C:

- `pwd`: Print the working directory.
- `echo`: Print a user input string on stdout.
- `cp`: Create a copy from a given source file to a given destination directory.
- `mv`: Move a file to another place (might be with a different name).

## Compilation

Each utility is implemented in a separate C file. To compile the utilities, use the following commands:

```sh
gcc -o pwd pwd.c
gcc -o echo echo.c
gcc -o cp cp.c
gcc -o mv mv.c
```

## Usage

### pwd

Print the current working directory.

```sh
./pwd
```
You can see the implementation [here](./pwd).

**Example Output:**

```
/home/user/projects/unix-utilities
```

### echo

Print a user input string on stdout.

```sh
./echo "Hello World"
```



**Example Output:**

```
Hello World
```
You can see the implementation [here](./echo).
### cp

Create a copy from a given source file to a given destination directory.

```sh
./cp source.txt /path/to/destination/
```

**Example Output:**

```
File 'source.txt' copied to '/path/to/destination/'
```
You can see the implementation [here](./cp).
### mv

Move a file to another place (might be with a different name).

```sh
./mv source.txt /path/to/destination/newname.txt
```

**Example Output:**

```
File 'source.txt' moved to '/path/to/destination/newname.txt'
```
You can see the implementation [here](./mv).
