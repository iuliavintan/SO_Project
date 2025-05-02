#ifndef PART2_H
#define PART2_H

#include<signal.h>
#include<stdio.h>

#define VIEW_TREASURE SIGUSR1
#define LIST_TREASURES SIGUSR1


extern int monitor_pid;
extern int monitor_running;
extern int monitor_stopped;

void monitor();

void start_monitor();

void view_treasure();

void list_treasures();

void stop_monitor();

#endif