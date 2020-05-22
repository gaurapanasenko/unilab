# Лабораторна робота №7, Панасенко Єгор, ПА-17-2

## Постановка задачі

Створити дві програми `parent.c` та `child.c`. Перша програма передає через командну строку число другій, яка зчитує його зі стандартного потоку вводу, множить його на 2 і передає число на стандартний потік помилок. parent.c зчитує це число і виводить перше і друге число на екран.

## Вихідний текст розв'язку

### Файл: `child.c`

```c
#include <stdio.h>

int main() {
    int number = 0;
    scanf("%i", &number);
    fprintf(stderr, "%i", 2 * number);
    return 0;
}
```

### Файл: `parent.c`

```c
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
    printf("%s\n", buff);

    close(stdin_fd[1]);
    close(stderr_fd[0]);

    return 0;
}
```

## Опис розв'язку

### Файл: `child.c`

Програма за допомогою `scanf` забирає число із стандартного потоку вводу та
за допомогою `fprintf` виводить результат у стандартний потік помилок.

### Файл: `parent.c`

Програма створює два канали даних: для запису та для читання,
далі у дочірньому процесі прив'язує канали до відповідних потоків
та закриває ці канали, так як далі вони не непотрібні, так як прив'язані до потоків,
далі запускає програму.
У головному процесі програма передає число у канал запису,
далі очікує число та виводить його.

## Опис тестових прикладів

Запустимо таку послідовність команд:

```shell
gcc -Wall -o child child.c
gcc -Wall -o parent parent.c
./parent 5
```

Отримаємо такий результат:
```shell
$ gcc -Wall -o child child.c
$ gcc -Wall -o parent parent.c
$ ./parent 5
10
```
