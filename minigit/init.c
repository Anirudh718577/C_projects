#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// Cross-platform directory creation logic
#ifdef _WIN32
#include <direct.h> // Windows specific
#define MAKE_DIR(path) _mkdir(path)
#else
#include <sys/stat.h> // POSIX specific
#include <sys/types.h>
#define MAKE_DIR(path) mkdir(path, 0755)
#endif

int main(int argc, char *argv[])
{

    if (argc < 2 || strcmp(argv[1], "init") != 0)
    {
        printf("Usage: ./minigit init\n");
        return 1;
    }

    if (MAKE_DIR(".minigit") == -1)
    {
        if (errno == EEXIST)
        {
            printf("minigit repository already exists in this directory.\n");
            return 0; // Clean exit
        }
        else
        {
            perror("Failed to create .minigit directory");
            return 1;
        }
    }

    if (MAKE_DIR(".minigit/objects") == -1)
    {
        perror("Failed to create .minigit/objects directory");
        return 1;
    }

    FILE *index_file = fopen(".minigit/index", "w");
    if (index_file == NULL)
    {
        perror("Failed to create .minigit/index file");
        return 1;
    }
    fclose(index_file);

    printf("Initialized empty minigit repository in .minigit/\n");

    return 0;
}