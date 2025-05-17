#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<signal.h>
#include"PART2.h"

void handle_sigchld(int s)
{
    if(monitor_stopped)
    {
        monitor_pid=-1;
        printf("Monitor stopped\n");

        monitor_stopped=0;
    }
}

int main(void){
    char cmd[101];

    struct sigaction sigchld;
    sigchld.sa_handler=handle_sigchld;
    sigchld.sa_flags=0;
    sigemptyset(&sigchld.sa_mask);

    if((sigaction(SIGCHLD, &sigchld, NULL))==-1)
    {
        perror("Error sigaction");
        exit(-1);   
    }


    while(1){
        printf(">> ");
        fflush(stdout);
        
        if(fgets(cmd, sizeof(cmd), stdin) == NULL ){
            break;
        }

        cmd[strcspn(cmd, "\n")] = 0;

        if(strcmp(cmd, "start_monitor") == 0){
            
            start_monitor();

        }
        else if(strcmp(cmd, "list_hunts") == 0){
            
            if(monitor_running)
            {
                list_hunts_wrap();

               // usleep(100000);
                read_from_pipe(pfd_list_hunts);
            }
            else
            {
                printf("Error! Monitor not running!\nTry \"start_running\" command\n");
            }
        
        }
        else if(strcmp(cmd,"list_treasures") == 0){
        
            if(monitor_running)
            {
                list_treasures();

               // usleep(100000);
                read_from_pipe(pfd_list_treasures);
            }
            else
            {
                printf("Error! Monitor not running!\nTry \"start_running\" command\n");
            }
        
        }
        else if(strcmp(cmd, "view_treasure") == 0){
        
            if(monitor_running)
            {
                view_treasure();

                //usleep(100000);
                read_from_pipe(pfd_view);

            }
            else
            {
                printf("Error! Monitor not running!\nTry \"start_running\" command\n");
            }
        
        }
        else if(strcmp(cmd, "stop_monitor") == 0){
        
            stop_monitor();
            monitor_running=0;
        
        }
        else if(strcmp(cmd, "exit") == 0){
        
            if(monitor_running)
            {
                printf("Cannot exit without stopping monitor!\nTry command \"stop_monitor\"\n");
            }
            else
            {
                exit(0);
            }
        }
        else if(strcmp(cmd, "calculate_score")==0)
        {
            char username[50];
            printf("Username:");
            scanf("%s", username);
            getchar();
            calculate_score_wrap(username);
        }
        else{
           
            printf("Unknown command\n");
        
        }
    }
    return 0;
}