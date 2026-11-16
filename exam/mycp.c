#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int source_fd, destination_fd;
    char buffer[1024];
    ssize_t bytes_read;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s source_file destination_file\n", argv[0]);
        return 1;
    }

    source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1) {
        perror("Error opening source file");
        return 1;
    }

    destination_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destination_fd == -1) {
        perror("Error opening destination file");
        close(source_fd);
        return 1;
    }

    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
        ssize_t total_written = 0;

        while (total_written < bytes_read) {
            ssize_t bytes_written = write(destination_fd, buffer + total_written,
                                          bytes_read - total_written);

            if (bytes_written == -1) {
                if (errno == EINTR) {
                    continue;
                }
                perror("Error writing to destination file");
                close(source_fd);
                close(destination_fd);
                return 1;
            }

            total_written += bytes_written;
        }
    }

    if (bytes_read == -1) {
        perror("Error reading source file");
        close(source_fd);
        close(destination_fd);
        return 1;
    }

    if (close(source_fd) == -1) {
        perror("Error closing source file");
        close(destination_fd);
        return 1;
    }

    if (close(destination_fd) == -1) {
        perror("Error closing destination file");
        return 1;
    }

    printf("File copied successfully.\n");
    return 0;
}
