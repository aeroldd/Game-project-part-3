#ifndef DIALOGUE_H
#define DIALOGUE_H

/*================================================================================================================
Reading the dialogue from text files and fancy printing it onto the screen
==================================================================================================================*/

#define DIALOGUE_PATH "dialogue/"

// Plays dialogue stored onto a text file, and replaces each keyword in the file with the replacements
void playDialogue(const char* fileName, char **keywords, char **replacements, int keywordCount);

// Replaces the keywords in a string with its correseponding replacements
char* replaceKeywords(const char* string, char **keywords, char **replacements, int variableCount);

#endif