#include <stdio.h>
#include <unistd.h>

#define CHILDREN "./child"
#define BUFF_SIZE 21


int main(int argc, char **argv) {
    pid_t pid;
    int stdin_fd[2];
    int stderr_fd[2];
    char buff[BUFF_SIZE] = {0};

    if (argc < 2) {
        printf("Wrong arguments\n");
        return 1;
    }

    if (pipe(stdin_fd) == -1 || pipe(stderr_fd) == -1) {
        perror("Failed to create pipe");
        return 1;
    }

    if ((pid = fork()) == -1) {
        perror("Failed to fork");
        return 1;
    }

    if (pid == 0) {
        close(stdin_fd[1]);
        close(stderr_fd[0]);

        dup2(stdin_fd[0], 0);
        dup2(stderr_fd[1], 2);

        close(stdin_fd[0]);
        close(stderr_fd[1]);

        execlp(CHILDREN, CHILDREN, NULL);
        perror("Failed to exec");
        return 1;
    }

    close(stdin_fd[0]);
    close(stderr_fd[1]);

    write(stdin_fd[1], argv[1], sizeof(argv[1]));
    read(stderr_fd[0], buff, BUFF_SIZE);
    printf("%s", buff);

    close(stdin_fd[1]);
    close(stderr_fd[0]);

    return 0;
}
