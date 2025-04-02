#include <conio.h>
#include <stdio.h>

int main() {
    char key;
    printf("please enter a character >> ");
    key = getch();
    if(key == '1') {
        printf("integers work!");
    }
    printf("the key you pressed was: %c", key);
    return 0;
}