#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define PROGRAM "./1"

void log_stat(int stat_loc) {
    printf("\n------------------\n(program exited with code: %i)\n\n", WEXITSTATUS(stat_loc));
}

int main(int argc, char **argv) {
    pid_t pid;
    int stat_loc;
    char *exec_argv[4] = {PROGRAM, "", "", NULL};
    char *evec_env[4] = {"PATH=\"/usr/bin\"", "NUMBER=", "NAME=", NULL};

    if ((pid = fork()) == -1) {
        perror("Failed to fork");
        return 1;
    }

    if (pid == 0) {
        execlp(PROGRAM, PROGRAM, "first", "execlp", NULL);
        perror("Failed to exec");
        return 1;
    }

    waitpid(pid, &stat_loc, 0);
    log_stat(stat_loc);

    if ((pid = fork()) == -1) {
        perror("Failed to fork");
        return 1;
    }

    if (pid == 0) {
        exec_argv[1] = "second";
        exec_argv[2] = "execvp";
        execvp(PROGRAM, exec_argv);
        perror("Failed to exec");
        return 1;
    }

    waitpid(pid, &stat_loc, 0);
    log_stat(stat_loc);

    if ((pid = fork()) == -1) {
        perror("Failed to fork");
        return 1;
    }

    if (pid == 0) {
        evec_env[1] = "NUMBER=third";
        evec_env[2] = "NAME=execle";
        execle(PROGRAM, PROGRAM, "third", "execle", NULL, evec_env);
        perror("Failed to exec");
        return 1;
    }

    waitpid(pid, &stat_loc, 0);
    log_stat(stat_loc);

    if ((pid = fork()) == -1) {
        perror("Failed to fork");
        return 1;
    }

    if (pid == 0) {
        exec_argv[1] = "fourth";
        exec_argv[2] = "execve";
        evec_env[1] = "NUMBER=fourth";
        evec_env[2] = "NAME=execve";
        execve(PROGRAM, exec_argv, evec_env);
        perror("Failed to exec");
        return 1;
    }

    waitpid(pid, &stat_loc, 0);
    log_stat(stat_loc);

    return 0;
}
