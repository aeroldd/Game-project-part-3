#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "input.h"

// CODE FOR TEXT SCROLLING
// In RPG games when text is displayed it goes character by character
// Its more of an aesthetic thing that makes the game cool

// Time delay code. This can also be used for other sections of code when a time delay is required.
// Code taken from https://www.geeksforgeeks.org/time-delay-c/
void delay(int noMilliseconds)
{
    // Converting time into milli_seconds
    int milliseconds = noMilliseconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milliseconds)
        ;
}

// This is basically the printf function but makes it go char by char
void fancyPrint(const char* format, ...) {
    va_list args; // list of args so that we can use the print arguments. the ellipsis in the parameter means multiple arguements
    va_start(args, format);

    // Allocate a buffer large enough to store the formatted string
    char buffer[1024]; 
    vsnprintf(buffer, sizeof(buffer), format, args); // This does the printf formatting of the entered message and saves it in the buffer
    va_end(args);

    // This will now print the formatted string from the buffer char by char
    int i = 0;
    while (buffer[i] != '\0') {
        if (kbhit()) { // Skip animation if key is pressed
            printf("%s", buffer + i);
            return;
        }
        putchar(buffer[i]);
        fflush(stdout);
        delay(20);
        i++;
    }
}

void pressAnyKey() {
    fancyPrint("[press any key to continue]\n");
    getKeyPress();
}

// Dice rolling!
int roll(int max) {
    return (rand() % max) + 1;
}

// Rolls a d20 with checking for critical success or failure
int rolld20() {
    int r = roll(20);
    if (r == 20) return 100;  // critical
    if (r == 1) return 101;   // critical failure
    return r;
}