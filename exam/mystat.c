#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

void print_permissions(mode_t mode) {
    char permissions[11];

    permissions[0] = S_ISREG(mode) ? '-' :
                     S_ISDIR(mode) ? 'd' :
                     S_ISLNK(mode) ? 'l' : '?';

    permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';

    printf("File Permissions       : %s\n", permissions);
}

int main(int argc, char *argv[]) {
    struct stat file_info;
    struct stat link_info;
    const char *file_type;
    char *modified_time;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s file_or_directory\n", argv[0]);
        return 1;
    }

    if (stat(argv[1], &file_info) == -1) {
        perror("Error retrieving file information");
        return 1;
    }

    if (lstat(argv[1], &link_info) == -1) {
        perror("Error checking file type");
        return 1;
    }

    if (S_ISLNK(link_info.st_mode)) {
        file_type = "Symbolic Link";
    } else if (S_ISREG(file_info.st_mode)) {
        file_type = "Regular File";
    } else if (S_ISDIR(file_info.st_mode)) {
        file_type = "Directory";
    } else {
        file_type = "Other";
    }

    modified_time = ctime(&file_info.st_mtime);

    printf("\n--- File Metadata ---\n");
    printf("Path                   : %s\n", argv[1]);
    printf("File Type              : %s\n", file_type);
    printf("File Size              : %lld bytes\n", (long long)file_info.st_size);
    printf("Inode Number           : %llu\n", (unsigned long long)file_info.st_ino);
    printf("Number of Hard Links   : %llu\n", (unsigned long long)file_info.st_nlink);
    print_permissions(link_info.st_mode);
    printf("Last Modification Time : %s", modified_time ? modified_time : "Unavailable\n");

    return 0;
}
