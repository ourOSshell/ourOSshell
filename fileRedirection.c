#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//typedef int bool;
//#define true 1
//#define false 0

//Adds a character to the end of a string
void append(char* str, char ch){
    int length = strlen(str);
    str[length] = ch;
    str[length+1] = '\0';
}

int main(){
    //whole command which must latter be parsed.
    //Should be an array of strings

    bool inFound = false;
    bool outFound = false;

    char command[100] = "\0";
    //last character read in
    char ch;
    //will hold parsed string
    char *args[10];
    //temporary values to test exec() call
    args[0] = strdup("ls");
    args[1] = NULL;
    //return code for fork()
    int rc;
    //do{int c = getchar(); printf("c=%d\n", c);}while(1);
    while(1){
        printf("prompt-> ");
        //Get key strokes directly and stop echo of every key stroke
        system ("/bin/stty raw -echo");
        command[0] = '\0';
        //while return is not hit
        while((ch = getchar())!='\r'){
    	    if(ch == '\t'){
                //do tab complete
                printf("%s", command);
            }
            //if backspace is hit
            else if(ch == 127){
                if(strlen(command)>0){
                    //remove last char from screen
                    printf("\b \b");
                    //remove last char from behind the scenes
                    command[strlen(command)-1] = '\0';
                }
            }
            //failed attempt
            //if up arrow get previous command
            /*else if(ch == 27){
                if(ch == 65)
                    printf("up");
                else if(ch == 66)
                    printf("down");
            }*/
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
        //Return to normal capturing of keystrokes
        system("/bin/stty cooked echo");
        //to make sure we have the right string in the end
        printf("\n");
        
        if(strstr(command, "<"))
        {
            printf("inFound has been found\n");
            //input = 
        }
        else if(strstr(command, ">"))
        {
            printf("outFound has been found\n");
            //output = 
        }

        //parse(command);
        int argsIndex = 0;
        char* token = strtok(command, " ");
        while(token){
            //printf("token: %s\n", token);
            strcpy(args[argsIndex], token); // move into our arguments array
            token = strtok(NULL, " ");
            argsIndex++;
        }
        
        //Exit loop if command is exit
        //Should eventually be args[0]
        if(strcmp(command,"exit")==0) exit(0);
        rc = fork();
        if(rc<0){
            fprintf(stderr, "Could not fork\n\n");
            exit(0);
        }
        //Child proccess where command is executed.
        else if(rc==0){


            // parse to find if the greater than or less than is found
            // if not continue normally, if it is than separate the left and right sides of it
            // parse the left normally and the right is just the file name to read from or right
            // to depending on whether < or > was found.
            // Don't forget to fflush(0) to make sure the stream is clear
            
            /*if(inFound) // if < is found
            {
                int fd1 = open(input, O_RDONLY, 0); //open the file
                dup2(fd1, STDIN_FILENO); // copy from the file to the input stream
                close(fd1); //close the file.
            }
            if(outFound) // if > is found
            {
                int fd2 = creat(output, 0644); //create a file with filename output
                dup2(fd2, STDOUT_FILENO);  //send the output to the file
                close(fd2); //close the file
            }*/

            //call exec() to run command
            execvp(args[0],args);
            //Sould not print if exec() is successful
            fprintf(stderr, "Could not find command: %s\n\n", command);
            exit(0);
        }
        //Parent process
        else if(rc>0){
            wait(NULL);
        }
    }
    return 0;
}
