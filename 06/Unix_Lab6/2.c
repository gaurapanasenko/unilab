#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    pid_t pid;
    char *exec_argv[4] = {argv[0], "", "", NULL};
    char **last_arg = argv + argc;

    switch(argc) {
        case 1:
            if ((pid = fork()) == -1) {
                perror("Failed to fork");
                return 1;
            }

            if (pid == 0) {
                execlp(argv[0], argv[0], "first", "execlp", NULL);
                perror("Failed to exec");
                return 1;
            }

            if ((pid = fork()) == -1) {
                perror("Failed to fork");
                return 1;
            }

            if (pid == 0) {
                exec_argv[1] = "second";
                exec_argv[2] = "execvp";
                execvp(argv[0], exec_argv);
                perror("Failed to exec");
                return 1;
            }

            if ((pid = fork()) == -1) {
                perror("Failed to fork");
                return 1;
            }

            if (pid == 0) {
                execle(argv[0], argv[0], "third", "execle", NULL, NULL);
                perror("Failed to exec");
                return 1;
            }

            if ((pid = fork()) == -1) {
                perror("Failed to fork");
                return 1;
            }

            if (pid == 0) {
                exec_argv[1] = "fourth";
                exec_argv[2] = "execve";
                execve(argv[0], exec_argv, NULL);
                perror("Failed to exec");
                return 1;
            }
            sleep(1);
        case 3:
            for (; argv != last_arg; ++argv) {
                printf("%s ", *argv);
            }
            printf("\n");
            break;
        default:
            printf("Bad arguments\n");
    }

    return 0;
}
