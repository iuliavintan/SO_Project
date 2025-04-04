#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>
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

    // int buf_size=snprintf(NULL, 0, "%d, %s, %f, %f, %s, %d\n", comoara->id, comoara->name, comoara->longitude, comoara->latitude, comoara->clue, comoara->value);

    // char *buf=malloc(buf_size+1);
    // if(buf==NULL)
    // {
    //     perror("Eroare! Memorie insuficienta!:(");
    //     exit(-1);
    // }

    // sprintf(buf, "%d, %s, %f, %f, %s, %d\n", comoara->id, comoara->name, comoara->longitude, comoara->latitude, comoara->clue, comoara->value);
    

    // if(write(fd, buf, strlen(buf))==-1)
    // {
    //     perror("Unable to add new treasure:(");
    //     free(buf);
    //     close(fd);
    //     exit(-1);
    // }

    if(write(fd, comoara, sizeof(treasure))==-1)
    {
        perror("Unable to add new treasure:(");
        free(comoara);
        close(fd);
        exit(-1);
    }
    else
    {
        printf("=====================================\n");
        printf("New treasure added succesfully!\n");
    }
    
    free(comoara);
    close(fd);
    closedir(director);
}

void read_file(int f)
{
    treasure *comoara=malloc(sizeof(treasure));
    int nr=0;
    while(read(f, comoara, sizeof(treasure)))
    {
        printf("Treasure %d\n", nr++);
        printf("ID: %d\n", comoara->id);
        printf("Name: %s\n", comoara->name);
        printf("GPS longitude: %f\n", comoara->longitude);
        printf("GPS latitude: %f\n", comoara->latitude);
        printf("Clue: %s\n", comoara->clue);
        printf("Value: %d\n", comoara->value);
       // printf("%d %s %f %f %s %d\n", , comoara->name, comoara->longitude, comoara->latitude, comoara->clue, comoara->value);
    }
}

void list(char hunt[10])
{
    DIR *director;
    director=opendir(hunt);

    if(director==NULL)
    {
        printf("The hunt you provided doesn't exist!\n");
        exit(-1);
    }
    
    struct stat fisi_stat;

    char path[50];
    strcpy(path, hunt);
    strcat(path, "/game.txt");

    int fd=open(path, O_RDONLY, 0777);

    if(fd==-1)
    {
        perror("Failed to open treasure file!:(");
        exit(-1);
    }

    if(stat(path, &fisi_stat)==-1)
    {
        printf("Error! Failed to obtain information from file!:(");
        close(fd);
        closedir(director);
        exit(-1);
    }

    printf("Hunt name: %s\n", hunt);
    printf("File size: %ld bytes\n", fisi_stat.st_size);
    printf("Last modification made: %s", ctime(&fisi_stat.st_atim.tv_sec));
    printf("_____________________________________________________________\n");

    printf("File content:\n");
    read_file(fd);

    close(fd);
    closedir(director);
}

void view(char hunt[10], int id)
{
    DIR *director;
    
    director=opendir(hunt);

    if(director==NULL)
    {
        printf("The hunt you provided doesn't exist!\n");
        exit(-1);
    }
    

    closedir(director);
}