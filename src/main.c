#include "main.h"
#include "functions.h"
#include "regex.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    // argv[1] = fonction, argv[2] = intervalle, argv[3] = precision, argv[4] = vects dir
    if (argc == 1){
        printf("You must enter parameters !\n");
        return 0;
    }
    if (argc > 5){
        printf("Too much parameters !\n");
        return 1;
    }

    RBTree *regex = str2regex(argv[1]);
    export(regex, "mon_regex");
    freeRBTree(regex);
    return 0;
}
