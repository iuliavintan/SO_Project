#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include"PART1.h"

int calculate_score(char hunt[10], char username[40])
{
    DIR *director;

    director=opendir(hunt);
    if(director==NULL)
    {
        printf("The hunt you provided doesn't exist\n");
        exit(-1);
    }

    char file_path[50];
    sprintf(file_path, "%s/game.txt", hunt);
    
    int fd=open(file_path, O_RDONLY, 0777);
    if(fd==-1)
    {
        perror("Error opening treasure file!:(");
        exit(-1);
    }

    treasure *comoara=malloc(sizeof(treasure));
    if(comoara==NULL)
    {
        perror("Error! Insufficient memory creating new treasure!:(");
        exit(-1);
    }

    int score =0;
    while(read(fd, comoara, sizeof(treasure)))
    {
        if( strcmp(comoara->name, username)==0 )
        {
            score+=comoara->value;
        }
    }

    close(fd);
    closedir(director);
    free(comoara);

    return score;
}