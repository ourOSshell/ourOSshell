#include<stdio.h>
 
int main()
{
    int i,n; //declare two integers
    n = 0;   //set n to 0
    printf("Enter an integer:\n");
    scanf("%d",&n);//reading in an integer

    for(i = n;i >= 1;i--)
    {       
             printf("%d\n",(i*i*i));               
    }
    printf("Well that was easy!");
    return 0;
}

