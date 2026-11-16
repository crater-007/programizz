#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DISK_SIZE 100
#define MAX_FILES 100

typedef struct {
    int file_id;
    int index_block;
    int block_count;
    int data_blocks[DISK_SIZE];
} FileEntry;

int disk[DISK_SIZE];
FileEntry directory[MAX_FILES];
int file_count = 0;

void create_file(void) {
    int file_id, required_blocks;
    int free_blocks[DISK_SIZE];
    int free_count = 0;
    int i, j, temp;

    if (file_count >= MAX_FILES) {
        printf("Directory is full. Cannot create another file.\n");
        return;
    }

    printf("Enter File ID: ");
    if (scanf("%d", &file_id) != 1) {
        printf("Invalid File ID.\n");
        return;
    }

    for (i = 0; i < file_count; i++) {
        if (directory[i].file_id == file_id) {
            printf("A file with this ID already exists.\n");
            return;
        }
    }

    printf("Enter number of blocks required: ");
    if (scanf("%d", &required_blocks) != 1 || required_blocks <= 0 ||
        required_blocks >= DISK_SIZE) {
        printf("Enter a valid block count between 1 and 99.\n");
        return;
    }

    for (i = 0; i < DISK_SIZE; i++) {
        if (disk[i] == 0) {
            free_blocks[free_count++] = i;
        }
    }

    if (free_count < required_blocks + 1) {
        printf("Insufficient space on disk.\n");
        return;
    }

    for (i = free_count - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = free_blocks[i];
        free_blocks[i] = free_blocks[j];
        free_blocks[j] = temp;
    }

    directory[file_count].file_id = file_id;
    directory[file_count].index_block = free_blocks[0];
    directory[file_count].block_count = required_blocks;

    disk[free_blocks[0]] = 1;

    for (i = 0; i < required_blocks; i++) {
        directory[file_count].data_blocks[i] = free_blocks[i + 1];
        disk[free_blocks[i + 1]] = 1;
    }

    printf("File created successfully.\n");
    printf("Index Block: %d\n", directory[file_count].index_block);
    printf("Allocated Data Blocks: ");

    for (i = 0; i < required_blocks; i++) {
        printf("%d", directory[file_count].data_blocks[i]);
        if (i < required_blocks - 1) {
            printf(", ");
        }
    }

    printf("\n");
    file_count++;
}

void display_directory(void) {
    int i, j;

    if (file_count == 0) {
        printf("Directory is empty.\n");
        return;
    }

    printf("\n--- Indexed Allocation Directory ---\n");

    for (i = 0; i < file_count; i++) {
        printf("File ID: %d\n", directory[i].file_id);
        printf("Index Block: %d\n", directory[i].index_block);
        printf("Data Blocks: ");

        for (j = 0; j < directory[i].block_count; j++) {
            printf("%d", directory[i].data_blocks[j]);
            if (j < directory[i].block_count - 1) {
                printf(", ");
            }
        }

        printf("\n\n");
    }
}

int main(void) {
    int choice;

    for (int i = 0; i < DISK_SIZE; i++) {
        disk[i] = 0;
    }

    srand((unsigned int)time(NULL));

    while (1) {
        printf("\n--- Indexed File Allocation ---\n");
        printf("1. Create a File\n");
        printf("2. Display Directory\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                create_file();
                break;
            case 2:
                display_directory();
                break;
            case 3:
                printf("Program terminated.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
