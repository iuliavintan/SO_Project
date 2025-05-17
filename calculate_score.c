#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include"PART1.h"

int calculate_score_one_hunt(char hunt[10], char username[40])
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
        printf("%s", file_path);
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

int calculate_score(char username[50])
{
    struct dirent *file;
    
    DIR *director=opendir("./"); //root game directory

    if(director==NULL)
    {
        perror("Error opening root file directory");
        exit(-1);
    }

    int pfd[2]; //created pipe to send scores from child to parent
    
    if( pipe(pfd) == -1)
    {
        perror("Error creating pipe");
        exit(-1);
    }
    
    while( (file = readdir(director) )!= NULL )
    {
        if( strcmp(file->d_name, "." )!=0 && strcmp(file->d_name, "..")!=0)
        if(file->d_type==DT_DIR && strcmp(file->d_name, ".git")!=0)
        {
            char hunt[256];
            sprintf(hunt, "%s", file->d_name);

            int pid=fork();
            if(pid<0)
            {
                perror("Error fork");
                exit(-1);
            }

            if(pid==0)
            {
                int score=calculate_score_one_hunt(hunt, username);
                close(pfd[0]);
                write(pfd[1], &score, sizeof(score));
                close(pfd[1]);
                exit(0);
            }
        }
    }

    // while(wait(NULL)>0)
    // {
    //     //we wait for all child processes
    // }

    int score=0;
    int score_total=0;

    close(pfd[1]);
    while(read(pfd[0], &score, sizeof(score)))
    {
        score_total+=score;
    }
    close(pfd[0]);

    closedir(director);
    return score_total;
}

int main(int argc, char **argv)
{
    int score=calculate_score(argv[1]);

    printf("The score for user: %s is %d\n", argv[1], score);

    return 0;
}