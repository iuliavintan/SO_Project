#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include"PART1.h"

treasure *new_treasure()
{
    treasure *comoara=malloc(sizeof(treasure));
    if(comoara==NULL)
    {
        perror("Error! Insuficient memory creating new treasure:(");
        exit(-1);
    }

    printf("New treasure information:\n");

    printf("ID:");
    scanf("%d", &comoara->id);

    getchar();
    printf("User name:");
    fgets(comoara->name, 39, stdin);
    comoara->name[strlen(comoara->name)-1]='\0';

    printf("GPS longitude:");
    scanf("%f", &comoara->longitude);

    printf("GPS latitude:");
    scanf("%f", &comoara->latitude);

    getchar();
    printf("Clue:");
    fgets(comoara->clue, 199, stdin);
    comoara->clue[strlen(comoara->clue)-1]='\0';

    printf("Value:");
    scanf("%d", &comoara->value);

    return comoara;
}

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
    
    char path[50];
    strcpy(path, hunt);
    strcat(path, "/game.txt");

    int fd=open(path, O_CREAT | O_WRONLY | O_APPEND, 0777);

    if(fd==-1)
    {
        perror("Failed to open treasure file!:(");
        exit(-1);
    }

    treasure *comoara=new_treasure();

    int buf_size=snprintf(NULL, 0, "%d, %s, %f, %f, %s, %d\n", comoara->id, comoara->name, comoara->longitude, comoara->latitude, comoara->clue, comoara->value);

    char *buf=malloc(buf_size+1);
    if(buf==NULL)
    {
        perror("Eroare! Memorie insuficienta!:(");
        exit(-1);
    }

    sprintf(buf, "%d, %s, %f, %f, %s, %d\n", comoara->id, comoara->name, comoara->longitude, comoara->latitude, comoara->clue, comoara->value);
    write(fd, buf, strlen(buf));
    free(buf);
    close(fd);
    closedir(director);
}