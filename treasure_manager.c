#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"PART1.h"

int main(int argc, char *argv[])
{
    
    char log_path[1024];
    snprintf(log_path, sizeof(log_path), "%s/logged_hunt.txt", argv[2] ); // se creaza path ul catre log 

    if(argc == 3)
    {
        if(strcmp(argv[1], "--add")==0)
        {
            add(argv[2], log_path);
        }
        else if(strcmp(argv[1], "--list")==0)
        {
            list(argv[2], log_path);
        }
        else if(strcmp(argv[1], "--remove_hunt")==0)
        {
            remove_hunt(argv[2], log_path);
        }
        else
        {
            printf("Another option with 3 arguments!\n");
        }
    }
    else if(argc == 4)
    {
        if(strcmp(argv[1], "--view")==0)
        {
            int id = atoi(argv[3]);
            view(argv[2], id, log_path);
        }
        else if(strcmp(argv[1], "--remove_treasure")==0)
        {
            int id=atoi(argv[3]);
            remove_treasure(argv[2], id, log_path);
        }
        else
        {
            printf("Wrong arguments!\n Try again\n");
        }
    }
    else
    {
        perror("Wrong number of arguments! Try again\n");
        exit(-1);
    }
    

    return 0;

}