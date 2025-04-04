#include<stdio.h>
#include<stdlib.h>
#include"PART1.h"

int main(void)
{
    //add("hunt1");
    treasure *comoara;
    comoara=new_treasure();

    printf("%d, %s, %f, %f, %s, %d\n", comoara->id, comoara->name, comoara->longitude, comoara->latitude, comoara->clue, comoara->value);

    free(comoara);
    return 0;

}