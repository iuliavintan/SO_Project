#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"PART1.h"

int main(int argc, char *argv[])
{
    
    if(argc == 3)
    {
        if(strcmp(argv[1], "--add")==0)
        {
            add(argv[2]);
        }
        else if(strcmp(argv[1], "--list")==0)
        {
            list(argv[2]);
        }
        else
        {
            printf("Another option with 3 arguments!\n");
        }



    }
    else
    {
        perror("Wrong number of arguments for --add option!\n");
        exit(-1);
    }
    
    return 0;

}