/*
 * ========================================
 *                 main.c
 * ========================================
 *     Main File for lemmesee program.
 */

#include "lemmesee.h"

int main(void) {
    char path[MAX_LENGTH];

    printf("Enter target path: ");
    scanf("%s", path);

    list_dir(path); // Chosen Path to list.

    return 0;
}