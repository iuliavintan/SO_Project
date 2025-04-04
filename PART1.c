#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include"PART1.h"

void add(char hunt[10])
{
    DIR *director;

    director=opendir(hunt);
    
    if(director==NULL)
    {
        if(mkdir(hunt, 0777)==0)
        {
            printf("Directory %s created succesfully!\n", hunt);
        }
        else
        {
            perror("Failed to create new hunt directory!:(");
            exit(-1);
        }
    }
    
}