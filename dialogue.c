#include <stdio.h>

#include <string.h>
#include <stdlib.h>

#include "dialogue.h"
#include "util.h"
#include "input.h"

// Variable names will be a 2 array with {<word to replace>, <value>}

char* variableNames[1][2]; //todo:better keyword swapping

// Returns a dynamically allocated string with keywords replaced
char* replaceKeywords(const char* string, char **keywords, char **replacements, int variableCount) {
    if (!keywords || !replacements || variableCount == 0) {
        return strdup(string); // Return a duplicate of the original string if no replacements are needed
    }
    
    char *buffer = malloc(1024); // Allocate memory
    if (!buffer) return NULL; // Check for allocation failure
    buffer[0] = '\0'; // Initialize buffer as empty

    const char *current = string;
    char *replace;

    for (int i = 0; i < variableCount; i++) {
        while ((replace = strstr(current, keywords[i])) != NULL) {
            strncat(buffer, current, replace - current); // Copy text before keyword
            strcat(buffer, replacements[i]); // Add replacement
            current = replace + strlen(keywords[i]); // Move pointer past keyword
        }
    }
    
    strcat(buffer, current); // Add remaining text
    return buffer; // Return new string (must be freed by caller)
}

void playDialogue(const char* filename, char **keywords, char **replacements, int keywordCount) {
    char filenamePath[256];
    sprintf(filenamePath, "%s%s", DIALOGUE_PATH, filename);

    FILE *file = fopen(filenamePath, "r");
    if (!file) {
        printf("ERROR: File does not exist!\n");
        return;
    }

    char dialogue[1024];

    while (fgets(dialogue, sizeof(dialogue), file)) {
        // Check if the line contains "{wait}"
        char *waitToken = strstr(dialogue, "{wait}");
        if (waitToken) {
            memmove(waitToken, waitToken + 6, strlen(waitToken + 6) + 1);  // Remove "{wait}"
        }

        char *modified = replaceKeywords(dialogue, keywords, replacements, keywordCount);
        
        if (modified && *modified != '\0') {
            fancyPrint("> %s", modified);
            free(modified); // Free allocated memory
        } 

        if (waitToken) {
            getKeyPress(); // Only wait for input if {wait} was found
        }
    }

    fclose(file);
}

