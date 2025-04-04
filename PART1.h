#ifndef PART1_H
#define PART1_H
#include<stdio.h>

typedef struct treasure{
    int id;
    char name[40];
    float longitude, latitude;
    char clue[200];
    int value;
}treasure;

treasure  *new_treasure();

void add(char hunt[10]);

void list(char hunt[10]);

void view(char hunt[10], int id);

#endif