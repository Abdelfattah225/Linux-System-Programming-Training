#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NoOfArguments 3
#define Count 1024

int main(int argc, char **argv) {
    char buf[Count];
    int num_read;
    int source_fd;
    int dest_fd;
    int NoOfBytes;
    
    struct stat path_stat;
    char new_path[1024];
    char *destination = argv[2]; // Default destination

    if (argc != NoOfArguments) {
        printf("Usage: %s <source-file> <destination>\n", argv[0]);
        exit(-1);
    }

    // Open source file
    source_fd = open(argv[1], O_RDONLY);
    if (source_fd < 0) {
        perror("Could not open the source file");
        exit(-2);
    }

    // Check if destination is a directory
    if (stat(argv[2], &path_stat) == 0 && S_ISDIR(path_stat.st_mode)) {
        // Extract only the filename from argv[1]
        char *filename = strrchr(argv[1], '/');
        if (filename) {
            filename++; // Move past the '/'
        } else {
            filename = argv[1]; // If no '/' found, use original string
        }

        // Safety check in case filename is NULL
        if (filename == NULL || *filename == '\0') {
            printf("Error: Source filename is invalid.\n");
            exit(-6);
        }

        // Construct new path inside the directory
        snprintf(new_path, sizeof(new_path), "%s/%s", argv[2], filename);
        destination = new_path; // Update destination path
    }

    // Open destination file
    dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Could not open the destination");
        close(source_fd);
        exit(-3);
    }

    // Copy data from source to destination
    while ((num_read = read(source_fd, buf, Count)) > 0) {
        NoOfBytes = write(dest_fd, buf, num_read);
        if (NoOfBytes < 0) {
            perror("Write Failed");
            close(source_fd);
            close(dest_fd);
            exit(-4);
        }
    }

    close(source_fd);
    close(dest_fd);

    // Delete source file
    if (unlink(argv[1]) < 0) {
        perror("Error deleting source file");
        exit(-5);
    }

    printf("File moved successfully from %s to %s\n", argv[1], destination);

    return 0;
}

