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
    //for how many valid commands we want to track up to 100 14 chars long
    char historyOfCommands[100][14];
    int commandScroller = 0;
    int userCommandScroller = 0;

    while(strcmp(command,"exit")!=0){

        //get current working directory
        getcwd(directoriesPath,99);
        userCommandScroller = 0;
        printf("%s***prompt->", directoriesPath);
        scanf("%s", command);


        // Some commands
        if(strcmp(command,"vim")==0)
        {
            //enter text editor
            system("vim");
        }

        else if(strcmp(command,"help")==0)
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

    //This is just for testing history array
    int i = 0;
    for(i; i < commandScroller; i++)
    {
        printf("%s\n", historyOfCommands[i]);
    }
    return 0;
}
