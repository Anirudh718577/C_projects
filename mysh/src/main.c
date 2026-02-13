#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/mysh.h"
int main()
{

    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    printf("Welcome to MyShell! Type 'exit' to quit.\n");

    while (1)
    {
        printf("mysh> ");
        fflush(stdout);

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0)
        {
            continue;
        }

        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        parse_input(input, args);
        if (!handle_builtin(args))
        {
            execute_command(args);
        }
    }
    printf("Goodbye!\n");
    return 0;
}