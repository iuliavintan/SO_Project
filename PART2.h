#ifndef PART2_H
#define PART2_H

#include<signal.h>
#include<stdio.h>

#define VIEW_TREASURE SIGUSR1
#define LIST_TREASURES SIGUSR1
#define LIST_HUNTS SIGUSR1


extern int monitor_pid;
extern int monitor_running;
extern int monitor_stopped;
extern int pfd_list_hunts[2];
extern int pfd_list_treasures[2];
extern int pfd_view[2];

void read_from_pipe(int pfd[2]);

void monitor();

void start_monitor();

void view_treasure();

void list_treasures();

void list_hunts_wrap();

void stop_monitor();

void calculate_score_wrap(char username[50]);

#endif