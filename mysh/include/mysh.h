#ifndef MYSH_H
#define MYSH_H

#define MAX_INPUT 1024
#define MAX_ARGS 64

/* Parser Functions */
int parse_input(char *input, char **args);

/* Executor Functions */
void execute_command(char **args);

/* Built-in command functions */
int handle_builtin(char **args);

#endif /* MYSH_H */