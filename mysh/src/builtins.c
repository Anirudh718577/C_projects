#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/mysh.h"

/*
 * builtin_cd: Change directory
 */
static int builtin_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "mysh: cd: missing argument\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("mysh: cd");
        }
    }
    return 1;
}

/*
 * builtin_pwd: Print working directory
 */
static int builtin_pwd(char **args)
{
    char cwd[MAX_INPUT];

    (void)args; // Unused parameter

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("mysh: pwd");
    }
    return 1;
}

/*
 * builtin_echo: Print arguments
 */
static int builtin_echo(char **args)
{
    for (int i = 1; args[i] != NULL; i++)
    {
        printf("%s", args[i]);
        if (args[i + 1] != NULL)
        {
            printf(" ");
        }
    }
    printf("\n");
    return 1;
}

/*
 * handle_builtin: Check if command is a built-in and execute it
 * Returns: 1 if built-in was executed, 0 otherwise
 */
int handle_builtin(char **args)
{
    if (args[0] == NULL)
    {
        return 0;
    }

    if (strcmp(args[0], "cd") == 0)
    {
        return builtin_cd(args);
    }

    if (strcmp(args[0], "pwd") == 0)
    {
        return builtin_pwd(args);
    }

    if (strcmp(args[0], "echo") == 0)
    {
        return builtin_echo(args);
    }

    if (strcmp(args[0], "help") == 0)
    {
        printf("mysh - A simple shell\n");
        printf("Built-in commands:\n");
        printf("  cd <dir>    Change directory\n");
        printf("  pwd         Print working directory\n");
        printf("  echo <text> Print text\n");
        printf("  help        Show this help\n");
        printf("  exit        Exit the shell\n");
        return 1;
    }

    return 0; // Not a built-in
}