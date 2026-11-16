#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    const char *directory_path = ".";
    int show_hidden = 0;
    DIR *directory;
    struct dirent *entry;

    if (argc > 3) {
        fprintf(stderr, "Usage: %s [-a] [directory]\n", argv[0]);
        return 1;
    }

    if (argc == 2) {
        if (strcmp(argv[1], "-a") == 0) {
            show_hidden = 1;
        } else {
            directory_path = argv[1];
        }
    } else if (argc == 3) {
        if (strcmp(argv[1], "-a") == 0) {
            show_hidden = 1;
            directory_path = argv[2];
        } else if (strcmp(argv[2], "-a") == 0) {
            show_hidden = 1;
            directory_path = argv[1];
        } else {
            fprintf(stderr, "Usage: %s [-a] [directory]\n", argv[0]);
            return 1;
        }
    }

    directory = opendir(directory_path);
    if (directory == NULL) {
        perror("Error opening directory");
        return 1;
    }

    while ((entry = readdir(directory)) != NULL) {
        if (!show_hidden && entry->d_name[0] == '.') {
            continue;
        }

        printf("%s\n", entry->d_name);
    }

    if (closedir(directory) == -1) {
        perror("Error closing directory");
        return 1;
    }

    return 0;
}
