#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char command[100];
    while(strcmp(command,"exit")!=0){
        printf("prompt-> ");
        scanf("%s", command);
    }
    return 0;
}
