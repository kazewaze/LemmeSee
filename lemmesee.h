/*
 * ========================================
 *                lemmesee.h
 * ========================================
 *  Utilities for DIRECTORIES, FILES, ETC.
 *    Author: Kaycee Ingram <kazewaze>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#define IS_NONE 0
#define IS_FILE 8
#define IS_DIR 16
#define CURRENT_ROOT '.'
#define MAX_LENGTH 25 // Max-Length for DIR Name.

void list_dir(const char *path);
int isDirOrFile(const char *path);
int isDirectoryExists(const char *path);
int isFileExists(const char *path);
int isFileExistsAccess(const char *path);
int isFileExistsStats(const char *path);
char* combine_path(const char *path, const char *current_child);

/*
 *  Lists contents of chosen DIR and prints to Console.
 */
void list_dir(const char *path) {
    DIR *current_root;
    struct dirent *dir;
    current_root = opendir(path);

    if (current_root) {
        while ((dir = readdir(current_root)) != NULL) {
            // Check if DIR, FILE, or NONE.
            switch (isDirOrFile(combine_path(path, dir->d_name))) {
                case (IS_FILE): printf("%s - <FILE>\n\n", dir->d_name);
                    break;
                case (IS_DIR): printf("%s - <DIR>\n\n", dir->d_name);
                    break;
                default: printf("ERROR\n");
            }
        }

        closedir(current_root);
    }
}

/*
 *  Checks if DIR or FILE.
 *  Returns 8 if FILE and 16 if DIR, otherwise returns 0.
 */
int isDirOrFile(const char *path) {
    int result;

    if (isDirectoryExists(path)) { // EXISTS - DIR.
        result = 16;
    } else {
        if (isFileExistsAccess(path)) { // EXISTS - FILE.
            result = 8;
        } else { // DOES NOT EXIST.
            result = 0;
        }
    }

    return result;
}

/*
 *  Checks if a DIR exists or not.
 *  Returns 1 if DIR exists, otherwise returns 0.
 */
int isDirectoryExists(const char *path) {
    struct stat stats;

    stat(path, &stats);

    // Checks for FILE existence.
    if (S_ISDIR(stats.st_mode)) {
        return 1;
    }

    return 0;
}

/*
 *  Checks if a FILE exists or not.
 *  Returns 1 if FILE exists, otherwise returns 0.
 */
int isFileExists(const char *path) {
    // Try to open FILE.
    FILE *fptr = fopen(path, "r");

    // If FILE does not exist.
    if (fptr == NULL) {
        return 0;
    }

    // FILE exists - Close FILE and return true.
    fclose(fptr);

    return 1;
}

/*
 *  Check if a FILE exists using access().
 *  Returns 1 if FILE exists, otherwise returns 0.
 */
int isFileExistsAccess(const char *path) {
    // Check for FILE existence.
    if (access(path, F_OK) == -1) {
        return 0;
    }

    return 1;
}

/*
 *  Check if a FILE exists using stat().
 *  Returns 1 if FILE exists, otherwise returns 0.
 */
int isFileExistsStats(const char *path) {
    struct stat stats;

    stat(path, &stats);

    // Check for FILE existence.
    if (stats.st_mode & F_OK) {
        return 1;
    }

    return 0;
}

/*
 *  Combines the Current Path with the chosen DIR.
 *  This function fixed an issue when user chose a nested DIR.
 */
char* combine_path(const char *path, const char *current_child) {
    int len_path = strlen(path);
    int len_child = strlen(current_child);

    // Using Heap over Stack.
    char* complete_path = (char*)malloc((len_path+len_child+1) * sizeof(char));

    strcpy(complete_path, path);
    strcat(complete_path, "/");
    strcat(complete_path, current_child);

    return complete_path;
}