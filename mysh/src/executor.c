#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../include/mysh.h"

void execute_command(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0)
    {
        // Fork failed
        perror("mysh: fork failed");
        return;
    }

    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) < 0)
        {
            perror("mysh");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Parent process - wait for child to complete
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}