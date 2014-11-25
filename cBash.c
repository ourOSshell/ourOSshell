#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/utsname.h>

//colors for printing to screen
#define KRED     "\x1B[31m"
#define KGREEN   "\x1B[32"
#define KYELLOW  "\x1b[33m"
#define KBLUE    "\x1b[34m"
#define KMAGENTA "\x1b[35m"
#define KCYAN    "\x1b[36m"
#define KRESET   "\x1B[0m"

//Adds a character to the end of a string
/*void append(char* str, char ch, int pos){
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
}*/

void append(char* str, char ch){
    int length = strlen(str);
    str[length] = ch;
    str[length+1] = '\0';
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

char *replace_substr(const char *str, const char *old, const char *new){
    char *ret, *r;
    const char *p, *q;
    size_t oldlen = strlen(old);
    size_t i, retlen, newlen = strlen(new);

    if(oldlen != newlen){
        for(i=0, p=str; (q=strstr(p, old)) != NULL; p=q+oldlen)
            i++;
        retlen = p - str + strlen(p) + i * (newlen - oldlen);
    }
    else retlen = strlen(str);

    if((ret = malloc(retlen+1)) == NULL)
        return NULL;

    for(r=ret, p=str; (q=strstr(p, old)) != NULL; p=q+oldlen){
        ptrdiff_t l = q - p;
        memcpy(r, p, l);
        r += l;
        memcpy(r, new, newlen);
        r += newlen;
    }
    strcpy(r, p);
    return ret;
}

int main(int argc, char *argv[]){

    struct utsname ubuffer;
    //if(argc < 1){
    //  fprintf(stderr, "%s: Not enough arguments");
    //return(-1);
    uname(&ubuffer);

    //opening ascii art
    system("reset"); // clears screen upon program execution
    printf(KCYAN " _____    _____         _ \n");
    printf("|   __|  | __  |___ ___| |_\n");
    printf("|  [__   | __ -| .'|_ -|   |\n");
    printf("|_____|  |_____|__,|___|_|_|\n");
    printf("\nSystem name:  %s\n", ubuffer.sysname);
    printf("Host name: %s\n", ubuffer.nodename);
    printf("OS Release: %s\n", ubuffer.release);
    printf("OS Version: %s\n", ubuffer.version);
    printf("CPU Type: %s\n", ubuffer.machine);
    printf("\n" KRESET);


    //***********************************************
    //           VARIABLES USED FOR HISTORY MODE
    //***********************************************
    // to transition between using past history and new commands
    bool addToHistory = false;                                                  
    //char ch;
    int t = 0;
    int commandScroller = 0;
    char historyOfCommands[10][20];
    //char command[100];
    //***********************************************
    //          END VARIABLES USED FOR HISTORY MODE
    //***********************************************

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
    char temp[100];
    char *cwd = malloc(100);
    char *homedir = getenv("HOME");
    //index into command string
    //int commandIndex = 0;
    //do{int c = getchar(); printf("c=%d\n", c);}while(1);
    while(1){
        //clear command string
        command[0] = '\0';
        outFound = false;
        inFound = false;
        //commandIndex = 0;
        //get current directory
        if(getcwd(temp, sizeof(temp))){
            cwd = temp;
            cwd = replace_substr(cwd, homedir, "~");
            printf("%s%s%s --> ", KRED, cwd, KRESET);
        }
        else{
            printf(KRED "prompt" KRESET "-> ");
        }
        //Get key strokes directly and stop echo of every key stroke
        system ("/bin/stty raw -echo");
        //while return is not hit

        int tempScroller = 0;

        //********************************
        //   START INPUT
        //********************************
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
                /*if(commandIndex>0){
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
                }*/
            }
            //arrows
            else if(ch == '\033'){
                //get rid of '['
                getchar();
                switch(getchar()){
                    //up
                case 'A':
                    // printf("up");

                    //********************************
                    //   HISTORY MODE
                    //********************************

                    //This is key for returning to the correct place on line
                    printf("\33[2K\r");

                    //printf("--History_Mode-->%s", historyOfCommands[tempScroller]);
                    printf("%s%s%s --> %s", KRED, cwd, KRESET, historyOfCommands[tempScroller]);

                }//end switch

                if(tempScroller == commandScroller)
                {
                    tempScroller = -1;
                }

                tempScroller ++;

                //The one you selected will be -1 cause temp scroller adds one on exit
                char *thisHist = historyOfCommands[tempScroller -1];
                strcpy(command, thisHist);
                addToHistory = false;

            }//end else if

            //********************************
            //  END HISTORY MODE
            //********************************

            //put the char on the string and on the screen
            else{
                putchar(ch);
                /*j = commandIndex;
                while(j<strlen(command)){
                printf("%c", command[j]);
                j++;
                }
                while(j>commandIndex){
                printf("\b");
                j--;
                }*/
                append(command, ch);
                //commandIndex++;

                //only add to history if it's a new command
                addToHistory = true;
            }
        }



        //********************************
        //   ADD TO HISTORY
        //********************************

        //only add to history if it is a new command
        if(addToHistory == true)
        {
            //This will put the whole unparsed  string into history, needs reparsing to work as commands again
            strcpy(historyOfCommands[commandScroller], command);
            commandScroller = commandScroller + 1;
            //If there are 10 saved entries in history array, then rewrite first entry
            if(commandScroller == 10)
            {
                commandScroller = 0;
            }

        }

        //********************************
        //   END ADD TO HISTORY
        //********************************

        //Return to normal capturing of keystrokes
        system("/bin/stty cooked echo");
        //to make sure we have the right string in the end
        printf("\n");
        if(strlen(command)==0) continue;

        //parse(command);
        //index into array of arguments
        int argsIndex;
        for(argsIndex = 0; argsIndex<argsLength; argsIndex++){
            args[argsIndex] = NULL;
        }

        // file redirection code (there are bugs so uncomment to test if you wish)

        if(strstr(command, ">"))
        {
            outFound = true;
            parser(command, argsout, ">");
            output = replace_substr(argsout[1], " ", "");
            strcpy(command, argsout[0]);
        }
        else if(strstr(command, "<"))
        {
            inFound = true;
            parser(command, argsin, "<");
            input = replace_substr(argsin[1], " ", "");
            strcpy(command, argsin[0]);
        }
        //else
        //{
        //parse normally
        //argsLength = parser(command, args, " ");
        //}


        argsLength = parser(command, args, " ");


        //Exit loop if command is exit
        //Should eventually be args[0]?
        if(strcmp(command,"exit")==0) exit(0);
        if(strcmp(args[0], "cd") == 0) //check to see if the command is "cd"
        {
            if(argsLength == 1) //if there is no argument go one level up.
            {
                chdir(homedir);
            }    
            else 
            {
                args[1] = replace_substr(args[1], "~", homedir);
                printf("%s\n", args[1]);
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
