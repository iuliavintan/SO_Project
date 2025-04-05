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
#include<errno.h>
#include"PART1.h"


//add info to variable
treasure *new_treasure()
{
    treasure *comoara=malloc(sizeof(treasure));
    if(comoara==NULL)
    {
        perror("Error! Insuficient memory creating new treasure:(");
        free(comoara);
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

//function to mark an action and the time it has been done
void log_action(const char action[], char path[])
{
    int fd_log = open(path, O_CREAT | O_APPEND | O_WRONLY, 0644); //owner can read and write group and others can only read the log file
    if(fd_log==-1)
    {
        perror("Error openening log file!\n");
        exit(-1);
    }
    
    time_t raw_time;
    struct tm *info_time;

    time(&raw_time);                    //the nr of seconds since the Epoch
    info_time=localtime(&raw_time);      //returns a pointer to a struct tm

    char timestamp[100];

    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", info_time);     //writing the timestamp in a string

    char actual_log[256];
    snprintf(actual_log, sizeof(actual_log), "Performed action: %s at %s\n", action, timestamp);

    if(write(fd_log, actual_log, strlen(actual_log))==-1)
    {
        perror("Unable to write in log file\n");
        exit(-1);
    }


    close(fd_log);
}


//Add the new treasure to a specified hunt - each hunt is stored in a separate directory.

void add(char hunt[10], char log_path[1024])
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
    sprintf(path, "%s/game.txt", hunt);

    int fd=open(path, O_CREAT | O_WRONLY | O_APPEND, 0777);

    if(fd==-1)
    {
        perror("Failed to open treasure file!:(");
        exit(-1);
    }

    treasure *comoara=new_treasure();

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
    
    char symlink_path[100];

    sprintf(symlink_path, "logged_hunt-%s", hunt);

    struct stat st;

    if(lstat(symlink_path, &st)==-1)
    {
        if(errno==ENOENT)       //checking if the symlink exists
        {
            if(symlink(log_path, symlink_path)==-1) //if not, we create it
            {
                perror("Failed to create symbolic link\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("Symbolic link made!\n");
            }
        }
    }
    

    log_action("Added Treasure", log_path);


    free(comoara);
    close(fd);
    closedir(director);
}

void print_treasure(treasure *comoara)
{
    
    printf("ID: %d\n", comoara->id);
    printf("Name: %s\n", comoara->name);
    printf("GPS longitude: %f\n", comoara->longitude);
    printf("GPS latitude: %f\n", comoara->latitude);
    printf("Clue: %s\n", comoara->clue);
    printf("Value: %d\n", comoara->value);
}

//parsing the file line by line to extract each treasure info
void read_and_print_file(int f)
{
    treasure *comoara=malloc(sizeof(treasure));
    if(comoara==NULL)
    {
        perror("Error! Insuficient memory creating new treasure:(");
        free(comoara);
        exit(-1);
    }

    int nr=1;
    while(read(f, comoara, sizeof(treasure)))
    {
        printf("Treasure %d\n", nr++);
        print_treasure(comoara);
        printf("..........................................\n");
    }
    free(comoara);
}


//listing all treasures from a specific hunt
//providing info about directory: hunt name, file size in bytes, the time of the last modification
void list(char hunt[10], char log_path[1024])
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

    printf("File content:\n\n");
    read_and_print_file(fd);

    char message[200];
    sprintf(message, "Listed %s", hunt);

    log_action(message, log_path);


    close(fd);
    closedir(director);
}


//printing the details about a specific treasure from a given hunt 
//searched by ID
void view(char hunt[10], int id)
{
    DIR *director;
    
    director=opendir(hunt);

    if(director==NULL)
    {
        printf("The hunt you provided doesn't exist!\n");
        exit(-1);
    }
    
    char path[50];
    strcpy(path, hunt);
    strcat(path, "/game.txt");

    int fd=open(path, O_RDONLY, 0777);
    if(fd==-1)
    {
        perror("Failed to open treasure file!:(");
        exit(-1);
    }

    treasure *buff=malloc(sizeof(treasure));
    if(buff==NULL)
    {
        perror("Error! Insuficient memory creating new treasure:(");
        free(buff);
        close(fd);
        closedir(director);
        exit(-1);
    }
    
    int found=0;

    while(read(fd, buff, sizeof(treasure)))
    {
        if(buff->id==id)
        {
            printf("Treasure found!\n");
            print_treasure(buff);
            found=1;
            break;
        }
    }
    
    //char message[1000];
    
    if(found==0)
    {
        printf("There aren't any treasures matching ID%d\n", id);
    }

    
    

    free(buff);
    close(fd);
    closedir(director);
}

void remove_treasure(char hunt[10], int id)
{
    DIR *director;

    director=opendir(hunt);
    if(director==NULL)
    {
        perror("The hunt you provided doesn't exist!\n");
        closedir(director);
        exit(-1);
    }

    char path[50];
    strcpy(path, hunt);
    strcat(path, "/game.txt");

    int fd=open(path, O_RDWR, 0777);
    if(fd==-1)
    {
        perror("Failed to open treasure file!:(");
        exit(-1);
    }



    close(fd);
    closedir(director);
}