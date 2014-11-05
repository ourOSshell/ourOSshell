#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(){
    char command[100];
    int rc;
    while(1){
        printf("prompt-> ");
        scanf("%s", command);
        if(strcmp(command,"exit")==0) exit(0);
        rc = fork();
        if(rc<0){
            fprintf(stdout, "Could not create another process.\n\n");
            exit(0);
        }
        else if(rc==0){
            //Call exec() to run command
            //Should not print if exec is successful
            printf("Could not find command: %s\n\n", command);
            exit(0);
        }
        else if(rc>0){
            int wc = wait(NULL);
	}
    }
    return 0;
}
