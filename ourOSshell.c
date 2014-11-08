#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int processHandler()
{
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
        //Call exec() to run command
        //Should not print if exec is successful

        //printf("Could not find command: %s\n\n", command);
        exit(0);

    } 

    else
    {
        //Parent process code
        int wc = wait(NULL); //is child finished?
    }
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
    char historyOfCommands[100][14];
    int commandScroller = 0;
    int userCommandScroller = 0;
    int argumentScroller = 0;

    while(strcmp(argumentsAfterParsing[0],"exit")!=0){

        //used for moving through the argument array after parsing
        argumentScroller = 0;
        //get current working directory
        getcwd(directoriesPath,99);
        userCommandScroller = 0;
        printf("%s***prompt->", directoriesPath);

        int i; 
        fgets(command, 99, stdin);//instead of scanf, scanf doesnt work for parsing

        // remove newline, if present
        i = strlen(command)-1;
        if( command[ i ] == '\n') 
            command[i] = '\0';

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

        /*
        //Working on changing directory need to parse arguments first though
        else if (strcmp(command,"cd")==0){  
        if (command==1){ //no args were given
        printf("cd: please specify a path.\n");
        }
        else{
        returnCode = chdir(command); 
        if (returnCode == -1){
        perror(command);
        }
        }
        */

        else if(strcmp(argumentsAfterParsing[0],"help")==0)
        {
            //These are the things you can do in ourOSshell
            printf("So far you can't do anything. GoodLuck!\n");
        }

        //doesn't match any commands
        else if(strcmp(command,"")==0)
        {
            printf("This is not a command. Check 'help' for commands.\n");
        }

        //valid command so use processHandler
        else
        {
            //processHandler(command);
        }

        //A start to the saving commands history
        strcpy(historyOfCommands[commandScroller], command);
        commandScroller = commandScroller + 1;
        //If there are 99 saved entries in history array, then start over
        if(commandScroller == 99)
        {
            commandScroller = 0;
        }

    }//end while 

    //Test history array, will erase after history is working
    int i = 0;
    for(i; i < commandScroller; i++)
    {
        printf("%s\n", historyOfCommands[i]);
    }
    return 0;
}
