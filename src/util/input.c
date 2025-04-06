#include "input.h"
#include <stdio.h>

// Checks if its compiled with windows
// This is because keyboard input is handled differently in different operating systems
// Windows uses conio.h, whereas Mac and Linux uses termios.h and unistd.h.
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>  // Windows has _getch()
    int getKeyPress() {
        return _getch();
    }
    int kbhit() {
        return _kbhit();
    }
#else
    #include <termios.h>
    #include <unistd.h>

    int getKeyPress() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);  // Get current terminal settings
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);  // Disable echo & buffering
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();  // Read one character
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Restore settings
        return ch;
    }

    int kbhit() {
        struct timeval tv = {0, 0};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
    }

#endif