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

void add();

#endif