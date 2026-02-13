# mysh - A Simple Unix Shell

A custom shell implementation in C for learning systems programming concepts.

## Features

- Execute external commands
- Built-in commands: `cd`, `pwd`, `echo`, `help`
- Graceful error handling
- Coming soon: I/O redirection, pipes, background processes

## Building

```bash
make
```

## Running

```bash
./mysh
```

Or build and run in one step:

```bash
make run
```

## Usage

```bash
mysh> pwd
/home/user/mysh
mysh> cd ..
mysh> ls
mysh> echo Hello World
Hello World
mysh> help
mysh> exit
```

## Testing

Run with valgrind to check for memory leaks:

```bash
make valgrind
```

## Project Structure

```bash
mysh/
├── Makefile          # Build configuration
├── README.md         # This file
├── include/          # Header files
│   └── mysh.h
├── src/              # Source files
│   ├── main.c        # Main loop
│   ├── parser.c      # Input parsing
│   ├── executor.c    # Command execution
│   └── builtins.c    # Built-in commands
└── tests/            # Test files
```

## Learning Resources

- `man fork` - Process creation
- `man execvp` - Execute programs
- `man waitpid` - Wait for process
- `man chdir` - Change directory
