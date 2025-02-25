#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 256
#define ECHO_Arguments 2
#define CP_Arguments 3
// Structure to store local variables

typedef struct {
	char name[100];
	char value[100];

} local_var;

// Array to store local variables
local_var local_vars[100];
int var_count = 0;

/*
 * Function to check if the input contains an '=' sign, indicating a variable assignment
 * inputs: pointer to char(*input)
 * outputs: integer(position )
 */

int is_variable_assignment(char *input)
{
	return strchr(input, '=') != NULL;
}

/*
 * Function to set a local variable.
 * Inputs: Variable name (char*), Variable value (char*)
 * Output: None
 */
void set_local_variable(char *name, char *value)
{
	char flag = 0;
	for (int i = 0; i < var_count && flag == 0; i++) {
		if (strcmp(local_vars[i].name, name) == 0) {
			strcpy(local_vars[i].value, value);	// Update value if variable exists
			flag = 1;
		}
	}
	if (flag == 0) {
		// Otherwise, add a new variable
		strcpy(local_vars[var_count].name, name);
		strcpy(local_vars[var_count].value, value);
		var_count++;
	}
}

/*
 * Function to return the value of a local variable.
 * Input: Variable name (char*)
 * Output: Variable value (char*)
 */
char *get_local_variable(char *name)
{
	for (int i = 0; i < var_count; i++) {
		if (strcmp(local_vars[i].name, name) == 0) {
			return local_vars[i].value;
		}
	}
	return NULL;
}

/*
 * Function to set an environment variable.
 * Inputs: Variable name (char*), Variable value (char*)
 * Output: None
 */
void set_env_variable(char *name, char *value)
{
	if (setenv(name, value, 1) == 0) {
	} else {
		perror("setenv failed");
		exit(-1);
	}
}

/*
 * Function to replace variables in command arguments with their values.
 * Inputs: Arguments array (char**), Number of arguments (int)
 * Output: None
 */

void replace_variables(char **args, int args_count) {
    for (int i = 0; i < args_count; i++) {
        char *pos = strchr(args[i], '$');  // Find `$`
        if (pos != NULL) {
            char *var_name = pos + 1;  // Variable name after `$`
            char *value = getenv(var_name);  // Get value from environment

            if (!value) {
                value = get_local_variable(var_name);  // Get value from local variables
            }

            if (value) {
                size_t prefix_len = pos - args[i];  // Length of the part before `$`
                size_t value_len = strlen(value);
                size_t total_len = prefix_len + value_len + 1;

                // Allocate new memory for the replaced value
                char *new_arg = (char *)malloc(total_len);
                if (!new_arg) {
                    perror("malloc failed");
                    exit(1);
                }

                // Copy the part before `$`
                strncpy(new_arg, args[i], prefix_len);
                new_arg[prefix_len] = '\0';  // Ensure null-termination
                strcat(new_arg, value);  // Append the variable value

                // Replace the original argument with the new one
                args[i] = new_arg;
            }
        }
    }
}
/*
 * Function to implement 'echo' command
 * inputs: integer , pointer to array of char  
 * outputs: nothing
 * */
void echo(int count, char **str)
{
	int NoOfBytes;
	if (count < ECHO_Arguments) {
		printf("No Arguments Provieded for echo utility\n");

	}

	for (int i = 1; i < count; i++) {
		NoOfBytes = write(1, str[i], strlen(str[i]));
		if (NoOfBytes < 0) {
			printf("Could not write the word : %s\n", str[i]);
			exit(-1);
		}
		if (i < count - 1) {
			NoOfBytes = write(1, " ", 1);
			if (NoOfBytes < 0) {
				printf("Could not write the space \n");
				exit(-2);
			}
		}
	}
	NoOfBytes = write(1, "\n", 1);
	if (NoOfBytes < 0) {
		printf("Could not write the new line \n");
		exit(-3);
	}
}

/* 
 * Function to represent 'pwd' utility
 * inputs: integer 
 * outputs : nothing
 * */
void pwd(int count)
{

	char buf[100];
	char *ptr;
	if (count > 1) {
		printf("Usage: pwd only \n");
	}

	ptr = getwd(buf);
	if (ptr == NULL) {
		perror("Could not get currnet working directory\n");
		exit(-6);
	}
	int NoOfBytes = write(1, buf, strlen(buf));
	if (NoOfBytes < 0) {
		perror("Could not display current working directory\n");
		exit(-7);
	}
	NoOfBytes = write(1, "\n", 1);
}

/* 
 * Function to represent 'cp' utility
 * inputs: integer , pointer to array of char  
 * outputs : nothing
 * */
void cp(int count, char **str)
{
	char buf[BUF_SIZE];
	int num_read;
	int source_fd;
	int dest_fd;
	int NoOfBytes;
	if (count != CP_Arguments) {
		printf("Usage: %s <source-file> <destination-file>\n", str[0]);
	}
	// OPEN SOURCE
	source_fd = open(str[1], O_RDONLY);
	if (source_fd < 0) {
		printf("Could not open the file\n");
		exit(-8);
	}
	// OPEN DeSTNATION

	dest_fd = open(str[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (dest_fd < 0) {
		printf("Could not open the destantion.\n");
		exit(-9);
	}

	while ((num_read = read(source_fd, buf, BUF_SIZE)) > 0) {
		//2. write info in distnation.
		NoOfBytes = write(dest_fd, buf, num_read);
		if (NoOfBytes < 0) {
			// error message.
			printf("Write Failed\n");
			exit(-10);
		}

	}
	close(source_fd);
	close(dest_fd);

}

/* 
 * Function to represent 'cd' command
 * inputs: integer , pointer to array of char  
 * outputs : nothing
 * */
void cd(int count, char **str)
{

	int val;
	if (count > 2) {
		printf("Usage:%s <path>\n", str[0]);
	}
	val = chdir(str[1]);
	if (val < 0) {
		printf("No such directory\n");
	}

}
 /*
 * Function to handle input and output redirection
 * Inputs: 
 *   - args: pointer to an array of strings (command arguments)
 *   - args_count: pointer to an integer (number of arguments)
 * Outputs: nothing.
 */

void handle_redirection(char **args, int *args_count) {
    for (int i = 0; i < *args_count; i++) {
        if (strcmp(args[i], ">") == 0) {  // Redirect stdout
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open failed for output file");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO); // Redirect stdout to file
            close(fd);
            args[i] = NULL;  // Remove `>` and file from arguments
            *args_count = i;
        } 
        else if (strcmp(args[i], "<") == 0) {  // Redirect stdin
            int fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("open failed for input file");
                exit(1);
            }
            dup2(fd, STDIN_FILENO); // Redirect stdin from file
            close(fd);
            args[i] = NULL;
            *args_count = i;
        } 
        else if (strcmp(args[i], "2>") == 0) {  // Redirect stderr
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open failed for error file");
                exit(1);
            }
            dup2(fd, STDERR_FILENO); // Redirect stderr to file
            close(fd);
            args[i] = NULL;
            *args_count = i;
        }
    }
}


int main()
{
	char buf[BUF_SIZE];
	char *ReturnOffegts;
	int args_count = 0;
	char *args[BUF_SIZE];

	while (1) {
		// Reset argument count and clear argument array

		args_count = 0;
		memset(args, 0, sizeof(args));
		// Display shell prompt
		printf("Micro Shell Prompt > ");
		// Read user input from stdin

		ReturnOffegts = fgets(buf, BUF_SIZE, stdin);
		// Check if fgets failed to read input

		if (strcmp(ReturnOffegts, "NULL") == 0) {
			perror("Could not get inputs\n");
			exit(-1);
		}
		// Remove newline character from input buffer

		buf[strlen(buf) - 1] = 0;
		// Parse input into tokens and count arguments

		char *token = strtok(buf, " ");
		while (token != NULL && args_count < BUF_SIZE - 1) {
			args[args_count++] = token;
			token = strtok(NULL, " ");
		}
		args[args_count] = NULL;
		// If user just pressed 'Enter', continue to next iteration

		if (strlen(buf) == 0)
			continue;
		// Replace variables with their values if present

		replace_variables(args, args_count);
		// Check for invalid commands involving variable assignment

		if ((args_count > 1 && is_variable_assignment(args[1])
		     && strcmp(args[0], "export") != 0)
		    || (args_count > 1 && strcmp(args[0], "export") != 0
			&& is_variable_assignment(args[0]))) {
			printf("InValid command\n");
		}
		// Handle local variable assignment

		else if (args[0] && is_variable_assignment(args[0])) {
			char input_copy[BUF_SIZE];
			strncpy(input_copy, buf, BUF_SIZE);
			input_copy[BUF_SIZE - 1] = '\0';
			char *name = strtok(buf, "=");
			char *value = strtok(NULL, "");

			if (name && value) {
				set_local_variable(name, value);
			}
			continue;
		}
		// Handle built-in commands

		else if (strcmp(args[0], "echo") == 0) {
			echo(args_count, args);
		} else if (strcmp(args[0], "pwd") == 0) {
			pwd(args_count);
		} else if (strcmp(args[0], "cp") == 0) {
			cp(args_count, args);
		} else if (strcmp(args[0], "exit") == 0) {
			exit(0);
		} else if (strcmp(args[0], "cd") == 0) {
			cd(args_count, args);
		}
		// Handle export command

		else if (strcmp(args[0], "export") == 0 && args_count > 1) {
			char *name, *value;

			if (strchr(args[1], '=')) {
				name = strtok(args[1], "=");
				value = strtok(NULL, "");

				if (name && value) {
					set_local_variable(name, value);
					set_env_variable(name, value);
				} else {
					printf
					    ("Invalid export format. Use: export VAR=value\n");
				}
			} else {

				name = args[1];
				value = get_local_variable(name);

				if (value) {
					set_env_variable(name, value);
				} else {
					printf("No such local variable: %s\n",
					       name);
				}
			}
		}
		// Handle external commands

		else {

			pid_t pid = fork();

			if (pid > 0) {
				// Parent process waits for child process to complete
				int status;
				wait(&status);
			} else if (pid == 0) {
				// Execute external command

				handle_redirection(args, &args_count);
				execvp(args[0], args);
				printf
				    ("Exec failed, kernel is not the mode of executing programs\n");
				exit(-1);
			} else {	// Fork failed

				printf("Parent: faild to fork\n");
			}
		}
		// Clean args array and reset argument count for next iteration

		args_count = 0;
		memset(args, 0, sizeof(args));

	}

	return 0;
}
