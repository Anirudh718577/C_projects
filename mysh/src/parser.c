#include <stdio.h>
#include <string.h>
#include "../include/mysh.h"

int parse_input(char *input, char **args)
{
    int i = 0;
    char *token;

    token = strtok(input, " \t");

    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " \t");
    }

    args[i] = NULL;

    return i;
}