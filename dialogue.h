#ifndef DIALOGUE_H
#define DIALOGUE_H

/*================================================================================================================
Reading the dialogue from text files and fancy printing it onto the screen
==================================================================================================================*/

#define DIALOGUE_PATH "dialogue/"

void playDialogue(const char* fileName);
char* replaceKeywords(char* string, char* variableNames[][2], int variableCount);

#endif