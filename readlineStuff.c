#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void append(char* str, char ch){
    int length = strlen(str);
    str[length] = ch;
    str[length+1] = '\0';
}

int main(){
    char command[100] = "\0";
    char ch;
    //do{int c = getchar(); printf("c=%d\n", c);}while(1);
    //Get key strokes directly and stop echo of every key stroke
    system ("/bin/stty raw -echo");
    //while return is not hit
    while((ch = getchar())!='\r'){
	    if(ch == '\t'){
            //do tab complete
            printf("%s", command);
        }
        //if backspace is hit
        else if(ch == 127){
            //remove last char from screen
            printf("\b \b");
            //remove last char from behind the scenes
            command[strlen(command)-1] = '\0';
        }
        //failed attempt
        //if up arrow get previous command
        else if(ch == 27){
            if(ch == 65)
                printf("up");
            else if(ch == 66)
                printf("down");
        }
        //else if(ch == 65);
        //if dowm arrow get next command
        //else if(ch == 66){
            //printf("down");
        //}
        //put the char on the string and on the screen
        else{
            putchar(ch);
            append(command, ch);
        }
    }
    system("/bin/stty cooked echo");
    //to make sure we have the right string in the end
    printf("%s\n", command);
    return 0;
}
