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

/*
 * function to represent 'echo' utility
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
 * function to represent 'pwd' utility
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
 * function to represent 'cp' utility
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
 * function to represent 'cd' utility
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

int main()
{
	char buf[BUF_SIZE];
	char *ReturnOffegts;
	int args_count = 0;
	char *args[BUF_SIZE];
	while (1) {

		printf("pico shell $ ");

		ReturnOffegts = fgets(buf, BUF_SIZE, stdin);

		// check return of fgets 
		if (strcmp(ReturnOffegts, "NULL") == 0) {
			perror("Could not get inputs\n");
			exit(-1);
		}

		buf[strlen(buf) - 1] = 0;	// indicate NULL

		// Parse input and count arguments
		char *token = strtok(buf, " ");
		while (token != NULL && args_count < BUF_SIZE - 1) {
			args[args_count++] = token;
			token = strtok(NULL, " ");
		}
		args[args_count] = NULL;

		//if Entering 'Enter'
		if (strlen(buf) == 0)
			continue;
		if (strcmp(args[0], "echo") == 0) {
			echo(args_count, args);
		} else if (strcmp(args[0], "pwd") == 0) {
			pwd(args_count);
		} else if (strcmp(args[0], "cp") == 0) {
			cp(args_count, args);
		} else if (strcmp(args[0], "exit") == 0) {
			exit(0);
		} else if (strcmp(args[0], "cd") == 0) {
			cd(args_count, args);
		} else {

			pid_t pid = fork();

			if (pid > 0) {
				int status;
				wait(&status);
			} else if (pid == 0) {

				execvp(args[0], args);
				printf
				    ("Exec failed, kernel is not the mode of executing programs\n");
				exit(-1);
			} else {
				printf("Parent: faild to fork\n");
			}
		}

		// clean args[] and args_count
		args_count = 0;
		memset(args, 0, sizeof(args));

	}

	return 0;
}
