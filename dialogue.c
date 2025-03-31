#include "dialogue.h"
#include "util.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>

// Variable names will be a 2 array with {<word to replace>, <value>}

char* variableNames[1][2]; //todo:better keyword swapping

void playDialogue(const char* filename) {
    char filenamePath[256];
    sprintf(filenamePath, "%s%s", DIALOGUE_PATH, filename);

    //printf("Dialogue file name: %s\n", filenamePath);

    FILE *file;
    FILE *f2;

    /* for testing if files are there*/
    if((file = fopen(filenamePath, "r"))==NULL) {
        printf("ERROR IT DOESNT EXIST!");
        //f2 = fopen(filenamePath, "w");
        return;
    }
    
    char dialogue[1024];

    while(fgets(dialogue, sizeof(dialogue), file)) {         ;
        fancyPrint("%s", replaceKeywords(dialogue, variableNames, 1));
        getch();
    }
}

// Returns the dialogue with the keywords replaced with their values
char* replaceKeywords(char* string,char* variableNames[][2], int variableCount) {
    variableNames[0][0] = "{username}";
    variableNames[0][1] = "username!";
    char buffer[1024] = "";
    char* replace; // this one js holds the substring to replace
    for(int i = 0; i < variableCount; i++) {
        while ((replace = strstr(string, variableNames[i][0])) != NULL) {
            strncat(buffer, string, replace - string); // copies the string before the keyword onto the buffer
            strcat(buffer, variableNames[i][1]); // adds the value of the keyword onto the buffer
            string = replace + strlen(variableNames[i][0]); // move the pointer after the keyword
        }
    }
    strcat(buffer, string); // resut of the string is added onto the buffer
    strcpy(string, buffer);

    //it prints properly here when it printf("%s", stringToReplace)
    return string;
}