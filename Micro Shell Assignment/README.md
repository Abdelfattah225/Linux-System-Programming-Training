# Micro Shell

Micro Shell is a simple command-line shell implementation in C that supports basic commands, local and environment variables, and I/O redirection.

## Features
- Execution of built-in commands (`echo`, `pwd`, `cp`, `cd`, `export`, `exit`)
- Execution of external commands using `execvp()`
- Local and environment variable handling
- Support for I/O redirection:
  - `<` : Redirects stdin from a file
  - `>` : Redirects stdout to a file
  - `2>` : Redirects stderr to a file
- Supports redirections

## Compilation
To compile the Micro Shell, run the following command:
```sh
gcc -o micro_shell micro_shell.c
```

## Usage
Run the shell by executing:
```sh
./micro_shell
```

### Example Commands
#### Running Built-in Commands
```sh
Micro Shell Prompt > echo Hello World
Hello World
Micro Shell Prompt > pwd
/home/user
```

#### Variable Handling
```sh
Micro Shell Prompt > VAR=123
Micro Shell Prompt > echo $VAR
123
Micro Shell Prompt > export VAR
Micro Shell Prompt > echo $VAR
123
```

#### I/O Redirection
```sh
Micro Shell Prompt > ls nonexistent_dir 2> error.log
```

## Error Handling
If an error occurs during file redirection (e.g., file cannot be opened), the command execution will be aborted with an error message.

## Repository Structure
```
├── micro_shell.c    # Source code
├── README.md       # Documentation
```

## Notes
- The shell does not support background execution (`&`) or pipes (`|`).
- Ensure the necessary permissions are set for files when redirecting output.
- Use `exit` to terminate the shell.

## Author
Abdelfattah Moawed

