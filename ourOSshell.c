#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

// added for mkdir
#include <sys/stat.h>
#include <sys/types.h>

int processHandler(char *argument)
{
    //check if getting string
    //printf("%s\n", argument);
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

        //this will execute the program
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
    printf("  ,---. ,--.          ,--,--. \n");
    printf(" '   .-'|  ,---. ,---.|  |  | \n");
    printf("  .  `-.|  .-.  | .-. |  |  | \n");
    printf(" .-'    |  | |  |   --|  |  | \n");
    printf(" `-----'`--' `--'`----`--`--' \n");
    printf("\n");
    printf("\n");

    char directoriesPath[100];
    char command[100];
    //The parser will fill this array with our different arguments
    char argumentsAfterParsing[10][100];
    //for how many valid commands we want to track up to 100, 14 chars long
    char historyOfCommands[100][20];
    int commandScroller = 0;
    int userCommandScroller = 0;
    int argumentScroller = 0;
    int returnCode; // used for change directory.

    while(strcmp(argumentsAfterParsing[0],"exit")!=0){

        //used for moving through the argument array after parsing
        argumentScroller = 0;
        userCommandScroller = 0;
        printf("--PROMPT-->");

        int i; 
        fgets(command, 99, stdin);//instead of scanf, scanf doesnt work for parsing

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
            printf("arguments %d: %s\n", r, argumentsAfterParsing[r]);
        }

        // Some commands
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

        //get history index
        else if(strcmp(argumentsAfterParsing[0],"H")==0)
        {
            //Print out history index
            int it = 0;
            for(it; it < commandScroller; it++)
            {
                printf("Command %d: %s\n", it, historyOfCommands[it]);
            }
        }//end history index

        //working on using the index of history commands
        else if(strcmp(argumentsAfterParsing[0],"use")==0)
        {

        }

        //makes directory in current working directory
        else if(strcmp(argumentsAfterParsing[0],"mkdir")==0)
        {
            int result = mkdir(argumentsAfterParsing[1], 0777);
        }


        else if(strcmp(argumentsAfterParsing[0],"help")==0)
        {
            //These are the things you can do in ourOSshell
            printf("So far you can:\nChange directories 'cd'\nExit 'exit'\nGet cwd 'cwd'\n" 
                "Use vim 'vim'\nCheck command history 'H'\nMake directory 'mkdir'");
        }

        //get current working directory
        else if(strcmp(argumentsAfterParsing[0],"cwd")==0)
        {

            getcwd(directoriesPath,99);
            printf("%s\n", directoriesPath);

        }

        //doesn't match any commands
        else if(strcmp(argumentsAfterParsing[0],"")==0)
        {
            printf("This is not a command. Check 'help' for commands.\n");
        }

        //valid command so use processHandler
        else if(strcmp(argumentsAfterParsing[0],"execute")==0)
        {
            char *arg = argumentsAfterParsing[1];
            processHandler(arg);
        }

    }//end while 

    return 0;
}
