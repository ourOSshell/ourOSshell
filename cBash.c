#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <fcntl.h>

//colors for printing to screen
#define KRED "\x1B[31m"
#define KRESET "\x1B[0m"

//Adds a character to the end of a string
void append(char* str, char ch, int pos){
    int len = strlen(str);
    char *buf = (char*)malloc(len+1);
    strncpy(buf, str, pos);
    len = strlen(buf);
    buf[len] = ch;
    buf[len+1] = '\0';
    len++;
    strcpy(buf+len, str+pos);
    strcpy(str, buf);
    free(buf);
}

int parser(char* str, char* array[], char* ch){
    int i = 0;
    char* token = strtok(str, ch);
    while(token){
        //printf("token: %s\n", token);
        array[i] = strdup(token);
        token = strtok(NULL, ch);
        i++;
    }
    return i;
}

int main(){
    char* input;
    char* output;
    bool outFound = false;
    bool inFound = false;
    //whole command which must latter be parsed.
    //Should be an array of strings
    char command[100] = "\0";
    //last character read in
    char ch;
    //will hold parsed command string
    char *args[20];
    char *argsout[5];
    char *argsin[5];
    int i,j;
    for(i=0; i<20; i++) args[i]=NULL;
    int argsLength = 0;
    //return code for fork()
    int rc;
    //current working directory
    char cwd[100];
    //index into command string
    int commandIndex = 0;
    //do{int c = getchar(); printf("c=%d\n", c);}while(1);
    while(1){
        //clear command string
        command[0] = '\0';
        commandIndex = 0;
        //get current directory
        if(getcwd(cwd, sizeof(cwd))){
            printf("%s%s%s --> ", KRED, cwd, KRESET);
        }
        else{
            printf(KRED "prompt" KRESET "-> ");
        }
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
                if(strlen(command)>0){
                    //remove last char from screen
                    j = commandIndex;
                    printf("\b");
                    while(1){
                        if(j==strlen(command)){
                            printf(" ");
                            break;
                        }
                        printf("%c", command[j]);
                        j++;
                    }
                    while(j>=commandIndex){
                        printf("\b");
                        j--;
                    }
                    //remove last char from behind the scenes
                    j = commandIndex;
                    while(j<=strlen(command)){
                        command[j-1] = command[j];
                        j++;
                    }
                    commandIndex--;
                }
            }
            //arrows
            else if(ch == '\033'){
                //get rid of '['
                getchar();
                switch(getchar()){
                    //up
                    case 'A':
                        printf("up");
                        break;
                    //down
                    case 'B':
                        printf("down");
                        break;
                    //right
                    case 'C':
                        //printf("right");
                        if(commandIndex<strlen(command)){
                            commandIndex++;
                            printf("\033[C");
                        }
                        break;
                    //left
                    case 'D':
                        //printf("left");
                        if(commandIndex>0){
                            commandIndex--;
                            printf("\033[D");
                        }
                        break;
                }
            }
            //put the char on the string and on the screen
            else{
                putchar(ch);
                j = commandIndex;
                while(j<strlen(command)){
                    printf("%c", command[j]);
                    j++;
                }
                while(j>commandIndex){
                    printf("\b");
                    j--;
                }
                append(command, ch, commandIndex);
                commandIndex++;
            }
        }
        //Return to normal capturing of keystrokes
        system("/bin/stty cooked echo");
        //to make sure we have the right string in the end
        printf("\n");
        if(strlen(command)==0) continue;

        // if(strstr(command, ">"))
        // {
        //     outFound = true;
        //     printf("found > and set outFound to true");
        //     printf("\n");
        //     printf("args[0]: ", args[0]);
        //     printf("\n");
        //     printf("args[1]: ", args[1]);
        //     printf("\n");
        //     printf("args[2]: ", args[2]);
        // }
        // else if(strstr(command, "<"))
        // {
        //     inFound = true;
        //     printf("found < and set inFound to true");
        // }
        
        //parse(command);
        //index into array of arguments
        int argsIndex;
        for(argsIndex = 0; argsIndex<argsLength; argsIndex++){
            args[argsIndex] = NULL;
        }
        /*argsIndex = 0;
        char* token = strtok(command, " ");
        while (token){
            //printf("token: %s\n", token);
            args[argsIndex] = strdup(token);
            //strcpy(args[argsIndex], token); // move into our arguments array
            token = strtok(NULL, " ");
            argsIndex = argsIndex + 1;
        }
        argsLength = argsIndex;*/

        // file redirection code (there are bugs so uncomment to test if you wish)
        /*
        if(strstr(command, ">"))
        {
            outFound = true;
            parser(command, argsout, ">");
            output = argsout[1];
            strcpy(command, argsout[0]);
        }
        else if(strstr(command, "<"))
        {
            inFound = true;
            parser(command, argsin, "<");
            input = argsin[1];
            strcpy(command, argsin[0]);
        }
        else
        {
            //parse normally
            //argsLength = parser(command, args, " ");
        }
        */

        argsLength = parser(command, args, " ");

        
        //Exit loop if command is exit
        //Should eventually be args[0]?
        if(strcmp(command,"exit")==0) exit(0);
        if(strcmp(args[0], "cd") == 0) //check to see if the command is "cd"
        {
            if(argsLength == 1) //if there is no argument go one level up.
            {
                chdir("/");
            }    
            else 
            {
                chdir(args[1]); //if there are arguments change the directory to whatever the argument is
            }
        }
        else{
            rc = fork();
            if(rc<0){
                fprintf(stderr, "Could not fork\n\n");
                exit(0);
            }
            //Child proccess where command is executed.
            else if(rc==0){
    
                //more file redirection code (there are bugs so uncomment to test if you wish)
                // dont forget to fflush(0);
                fflush(0);
                if(inFound) // if < is found
                {
                    int fd1 = open(input, O_RDONLY, 0); // open the file
                    // get contents of file and put into the file stream
                    dup2(fd1, STDIN_FILENO);
                    close(fd1); // close the file
                    inFound = false;
                }
    
                if(outFound) // if > is found
                {
                    int fd2 = creat(output, 0644); // create the file
                    dup2(fd2, STDOUT_FILENO); // get contents from std out and out into file
                    close(fd2); // close file
                    outFound = false;
                }
    
                //call exec() to run command
                execvp(args[0],args);
                //Sould not print if exec() is successful
                fprintf(stderr, "Could not find command: %s\n\n", command);
                exit(0);
            }
            //Parent process
            else if(rc>0){
                wait(NULL);
                //fflush(0);
            }
        }
    }
    return 0;
}
