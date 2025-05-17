#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/wait.h>

#include"PART1.h"
#include"PART2.h"

#define COMM_SIZE 101

int monitor_pid=-1;
int monitor_running =0;
int monitor_stopped=0;

void write_command(char *message)
{
    char path[50];
    sprintf(path, "cmd_file");

    int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if(fd == -1)
    {
        perror("Error opening cmd_file");
        exit(-1);
    }

    write(fd, message, strlen(message));

    close(fd);
}

void handle_sigusr1(int sig_type)
{
    char path[50];

    sprintf(path, "cmd_file");

    int fd=open(path, O_RDONLY, 0777);
    if(fd==-1)
    {
        perror("Error opening cmd_file");
        exit(-1);
    }

    char command[101]={0};
    read(fd, command, COMM_SIZE);

    command[strcspn(command, "\n")] = 0;

    if(strstr(command, "--view")!=0)
    {
        char hunt[30];
        char *com=strtok(command, " ");
        com = strtok(NULL, " ");
        strcpy(hunt, com);
        com=strtok(NULL, " ");
        char id[30];
        strcpy(id, com);
        char log_path[1024];
        snprintf(log_path, sizeof(log_path), "%s/logged_hunt.txt", hunt);

        pid_t pid_view=fork();

        if(pid_view<0)
        {
            perror("Error opening view process!");
            exit(-1);
        }


        if(pid_view==0)
        {
            if((execlp("./treasure_manager", "treasure_manager", "--view", hunt, id, NULL))==-1)
            perror("Error execlp");
            exit(1);
        }
    }
    else if((strstr(command, "--list_hunts"))!=0)
    {
        list_hunts();
    }
    else if((strstr(command, "--list"))!=0)
    {
        char hunt[30];
        char *com=strtok(command, " ");
        com=strtok(NULL, " ");
        strcpy(hunt, com);
        char log_path[1024];
        snprintf(log_path, sizeof(log_path), "%s/logged_hunt.txt", hunt);

        pid_t pid_list=fork();

        if(pid_list<0)
        {
            perror("Error opening list process!");
            exit(-1);
        }
        if(pid_list==0)
        {
            execlp("./treasure_manager", "treasure_manager", "--list", hunt, NULL);

            exit(1);
        }
    }
    
    close(fd);
}

void monitor()
{
    struct sigaction sa;
    sa.sa_handler=handle_sigusr1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=0;

    if((sigaction(SIGUSR1, &sa, NULL)) == -1)
    {
        perror("Error sigaction");
        exit(-1);
    }

    while(1)
    {
        pause();
    }

    exit(0);
}

void start_monitor()
{

    if((monitor_pid=fork())<0)
    {
        perror("Error opening monitor process!");
        exit(-1);
    }

    if(monitor_pid==0)
    {
        monitor_running=1;
        monitor();
        exit(0);
    }
    else
    {
        monitor_running=1;
        printf("Monitor started with PID %d\n", monitor_pid);
    }

}

void view_treasure()
{
    char hunt[10];
    int id;
    printf("Enter hunt: ");
    scanf("%s", hunt);
    getchar();
    hunt[strcspn(hunt, "\n")]=0;
    printf("Enter treasure ID:");
    scanf("%d", &id);
    getchar();
    char message[101];

    sprintf(message, "%s %s %d", "--view", hunt, id);

    write_command(message);

    kill(monitor_pid,  VIEW_TREASURE);
}

void list_treasures()
{
    char hunt[10];
    printf("Enter hunt:");
    scanf("%s", hunt);
    getchar();
    hunt[strcspn(hunt, "\n")]=0;
    char message[101];
    sprintf(message, "%s %s", "--list", hunt);
    write_command(message);
    kill(monitor_pid, LIST_TREASURES);
}

void list_hunts_wrap()
{
    char message[101];
    sprintf(message, "%s", "--list_hunts");
    write_command(message);
    kill(monitor_pid, LIST_HUNTS);
}

void calculate_score_wrap(char username[50])
{
    int pfd[2];

    if(pipe(pfd)==-1)
    {
        perror("Error creating pipe for calculation score");
        exit(-1);
    }

    pid_t pid=fork();
    if(pid==0)
    {
        //child process
        close(pfd[0]);
        //dup2(pfd[1], 1); //redirecting to stdout
        execl("./calculate_score", "calculate_score", username, NULL);
        perror("Execl failed");
        exit(-1);
    }
    else{
        //parent process
        close(pfd[1]);
        char buff[256];
        memset(buff, 0, sizeof(buff));
        read(pfd[0], buff, sizeof(buff));
        printf("%s", buff);

        close(pfd[0]);
        wait(NULL);
    }
    
}

void stop_monitor()
{
    printf("Monitor is stopping...\n");
    usleep(10000000);

    kill(monitor_pid, SIGTERM);

    monitor_stopped=1;


    while(monitor_pid != -1)
    {
      char command[30];
      if(scanf("%s", command) == 1)
	{
	  printf("Unable to perform <%s> action while monitor is stoppping!\n", command);
	}      
    }

    int status;
    
    if((waitpid(monitor_pid, &status, 0))==-1)
    {
        perror("Error waitpid");
        exit(-1);
    }

    if (WIFEXITED(status))
    {
      printf("Monitor terminated with status %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status))
    {
      printf("Monitor stopped by signal %d\n", WTERMSIG(status));
    }
    else
    {
      printf("Monitor stopped abnormally.\n");
    }


}