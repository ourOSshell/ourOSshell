#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>

// added for mkdir
#include <sys/stat.h>
#include <sys/types.h>

int processHandler(char *argument)
{
    //check if getting string
    printf("%s\n", argument);

    int ret;
    int pid;

    printf("Dealing with Processes (pid:%d)\n", (int) getpid());
    int rc = fork() ; 

    if (rc < 0) 
    {
        //error forking
        exit(1);
    } 

    else if (rc == 0) 
    {
        //child code


        ret = execvp(argument);
        if (ret==-1)
        {
            perror("failed to execute command"); 
            //exit(0);
        } //end if failed
    }

    else
    {
        //Parent process code
        int wc = wait(NULL); //is child finished?
    }

    printf("\n");
}


int main(){

    //opening ascii art
    printf(" _____    _____         _ \n");
    printf("|   __|  | __  |___ ___| |_\n");
    printf("|  [__   | __ -| .'|_ -|   |\n");
    printf("|_____|  |_____|__,|___|_|_|\n");
    printf("\n");
    printf("\n");

    // to transition between using past history and new commands
    bool useHistory = false;                                                  
    char ch;
    int t = 0;
    char directoriesPath[100];
    char command[100];
    //The parser will fill this array with our different arguments
    char argumentsAfterParsing[10][100];
    //for how many valid commands we want to track up to 100, 14 chars long
    char historyOfCommands[10][20];
    int commandScroller = 0;
    int userCommandScroller = 0;
    int argumentScroller = 0;
    int returnCode; // used for change directory.

    while(strcmp(argumentsAfterParsing[0],"exit")!=0){

        //Return to normal capturing of keystrokes
        system("/bin/stty cooked echo");

        //used for moving through the argument array after parsing
        argumentScroller = 0;
        userCommandScroller = 0;

        //if just got out of history mode use history command
        if(useHistory == false)
        {
            printf("--PROMPT-->");

            fgets(command, 99, stdin);//instead of scanf, scanf doesnt work for parsing
        }
        else
        {
            //use  history command

            useHistory = false;//automatically resets after 
        }


        int i; 
        // remove newline, if present, absolutely needed
        i = strlen(command)-1;
        if( command[ i ] == '\n') 
            command[i] = '\0';

        //This will put the whole unparsed  string into history, needs reparsing to work as commands again
        //Needs to be here since C parsing is destructive.
        strcpy(historyOfCommands[commandScroller], command);
        commandScroller = commandScroller + 1;
        //If there are 10 saved entries in history array, then rewite first entry
        if(commandScroller == 10)
        {
            commandScroller = 0;
        }

        //printf("This is your string: %s", command);


        //***********************************************
        //-------------THE PARSER-----------------------
        //***********************************************
        char* token = strtok(command, " ");
        while (token) {
            //printf("token: %s\n", token);
            strcpy(argumentsAfterParsing[argumentScroller], token); // move into our arguments array
            token = strtok(NULL, " ");
            argumentScroller= argumentScroller +1;
        }
        //end parser

        //Using for testing whether arguments are in array
        int r = 0;
        for(r = 0; r < argumentScroller; r++) 
        {
            // printf("arguments %d: %s\n", r, argumentsAfterParsing[r]);
        }

        // vim command
        if(strcmp(argumentsAfterParsing[0],"vim")==0)
        {
            //enter text editor
            system("vim");
        }


        //Working on changing directory
        else if (strcmp(argumentsAfterParsing[0],"cd")==0)
        { 
            returnCode = chdir(argumentsAfterParsing[1]); 
            if (returnCode == -1)
            {
                perror(argumentsAfterParsing[0]);
            }

        }// end changing directory

        //makes directory in current working directory
        else if(strcmp(argumentsAfterParsing[0],"mkdir")==0)
        {
            int result = mkdir(argumentsAfterParsing[1], 0777);
        }


        else if(strcmp(argumentsAfterParsing[0],"help")==0)
        {
            //These are the things you can do in ourOSshell
            printf("So far you can:\nChange directories 'cd'\nExit 'exit'\nGet cwd 'cwd'\n" 
                "Use vim 'vim'\ncycle command history 'h'\nMake directory 'mkdir'\n");
        }

        //get current working directory
        else if(strcmp(argumentsAfterParsing[0],"cwd")==0)
        {

            getcwd(directoriesPath,99);
            printf("%s\n", directoriesPath);

        }


        //valid command so use processHandler
        else if(strcmp(argumentsAfterParsing[0],"execute")==0)
        {
            char *arg = argumentsAfterParsing[1];
            processHandler(arg);
        }

        //***********************************************
        //            START HISTORY MODE CODE
        //***********************************************

        //Typing h gets you into history mode where you can use up arrows
        else if(strcmp(argumentsAfterParsing[0],"h")==0)
        {
            printf("--History_Mode--> ");

            system ("/bin/stty raw -echo");

            int tempScroller = 0;

            while((ch = getchar())!='\r'){

                if(ch == '\033'){
                    //get rid of '['
                    getchar();
                    switch(getchar()){
                        //up
                    case 'A':

                        //This is key for returning to the correct place on line
                        printf("\33[2K\r");

                        printf("--History_Mode-->%s", historyOfCommands[tempScroller]);
                        //end 
                    }//end switch
                }//end else if

                if(tempScroller == commandScroller)
                {
                    tempScroller = 0;
                }

                tempScroller ++;

            }//end while not return

            printf("\n\r");

            //**IMPORTANT**
            //command = historyOfCommands[tempScroller];// if press enter
            //Upon exit it will reparse this command and try to use if valid.

            //The one you selected will be -1 cause temp scroller adds one on exit
            char *this = historyOfCommands[tempScroller -1];
            strcpy(command, this);
            useHistory = true; 

        }//end outer elseif flip into h mode

        //***********************************************
        //            END HISTORY MODE CODE
        //***********************************************

        //*********************************************
        //               START PIPING CODE
        //*********************************************
        /*
        else if(strcmp(argumentsAfterParsing[0],"pipe")==0)
        {
        int do_command(char **args, int pipes)
        {
        // The number of commands to run
        const int commands = pipes + 1;
        int i = 0;

        int pipefds[2*pipes];

        for(i = 0; i < pipes; i++)
        {
        if(pipe(pipefds + i*2) < 0)
        {
        perror("Couldn't Pipe");
        exit(EXIT_FAILURE);
        }
        }

        int j = 0;
        int k = 0;
        int s = 1;
        int place;
        int commandStarts[10];
        commandStarts[0] = 0;

        // This loop sets all of the pipes to NULL
        // And creates an array of where the next
        // Command starts

        while (args[k] != NULL){
        if(!strcmp(args[k], "|")){
        args[k] = NULL;
        // printf("args[%d] is now NULL", k);
        commandStarts[s] = k+1;
        s++;
        }
        k++;
        }



        for (i = 0; i < commands; ++i) {
        // place is where in args the program should
        // start running when it gets to the execution
        // command
        place = commandStarts[i];

        pid = fork();
        if(pid == 0) {
        //if not last command
        if(i < pipes){
        if(dup2(pipefds[j + 1], 1) < 0){
        perror("dup2");
        exit(EXIT_FAILURE);
        }
        }

        //if not first command&& j!= 2*pipes
        if(j != 0 ){
        if(dup2(pipefds[j-2], 0) < 0){
        perror("dup2");
        exit(EXIT_FAILURE);
        }
        }

        int q;
        for(q = 0; q < 2*pipes; q++){
        close(pipefds[q]);
        }

        // The commands are executed here, 
        // but it must be doing it a bit wrong          
        if( execvp(args[place], args+place) < 0 ){
        perror(*args);
        exit(EXIT_FAILURE);
        }
        }
        else if(pid < 0){
        perror("error");
        exit(EXIT_FAILURE);
        }

        j+=2;
        }

        for(i = 0; i < 2 * pipes; i++){
        close(pipefds[i]);
        }

        for(i = 0; i < pipes + 1; i++){
        wait(&status);
        }
        }
        }
        */
        //*********************************************
        //               END PIPING CODE
        //*********************************************


    }//end while 

    return 0;
}// end main
