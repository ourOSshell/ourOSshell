#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    char command[100];
    while(strcmp(command,"exit")!=0){
        printf("prompt-> ");
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

    }//end while 
    return 0;
}
