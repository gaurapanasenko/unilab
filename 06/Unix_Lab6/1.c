#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp) {
    char **last_arg = argv + argc;
    for (; argv != last_arg; ++argv) {
        printf("%s ", *argv);
    }
    printf("\n");
    int count = 0;
    for (char **env = envp; *env != NULL; ++env) {
        printf("%s\n", *env);
        count++;
    }
    if (count >= 4) return 0;
    else return 1;
}
