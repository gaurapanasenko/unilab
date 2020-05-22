# Лабораторна робота №6, Панасенко Єгор, ПА-17-2

## Постановка задачі

1. Написати програму на мові С, яка виводить на термінал:
    * Ім'я даного процесу;
    * аргументи даного процесу, якщо вони є;
    * змінні оточення, якщо такі є. Якщо у процесу 4 або більше змінних оточення, то повернути 0, в іншому випадку повернути 1.
2. Написати програму на мові С, яка використовує наступні функції:
    * fork();
    * execlp();
    * execvp();
    * execle();
    * execve().
3. За допомогою функцій сімейства exec визвати програму 1.

## Вихідний текст розв'язку

### Файл: `1.c`

```c
#include <stdio.h>

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
```

### Файл: `2.c`

```c
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
```

### Файл: `3.c`

```c
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
```

## Опис розв'язку

### Файл `1.c`

Розберемо нашу першу програму.
Вона оброблює та виводить на екран аргументи `argc, **argv, **envp` функції `main`,
перший аргумент зберігає кількість переданих аргументів у командному рядку,
другий - строковий масив аргументів, причому перший аргумент завжди це назва процесу,
третій - строковий масив змінних оточення, який закінчується `NULL`, як термінант.

### Файл `2.c`

Ця програма виконується у двох режимах в залежності від кількості переданих аргументів.
У одному, якщо передано два аргументи, програма працює у режимі підпрограми,
яка виконує деяку умовну роботу.
У іншому режимі, якщо не передано аргументи програма запускає саму себе у режимах підпрограми.
У цьому режимі програма виконує `fork` для того щоб можна було запустити сімейство `exec`,
так як ці функції продовжують роботу програми тільки у випадку помилки.

### Файл `3.c`

Це програма запускає першу програму за допомогою різних функцій сімейства `exec`.
Причому підпроцес запускається у дочірній гілці та у головній гілці
за допомогою `waitpid` очікується код завершення процесу.

## Опис тестових прикладів

Запустимо таку послідовність команд:

```shell
gcc -Wall -o 1 1.c
./1 one two three four
gcc -Wall -o 2 2.c
./2
gcc -Wall -o 3 3.c
./3
```

Отримаємо подібний результат:

```shell
$ gcc -Wall -o 1 1.c
$ ./1 one two three four
./1 one two three four
SHELL=/bin/bash
COLORTERM=truecolor
XDG_CONFIG_DIRS=/etc:/etc/xdg:/usr/share
XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session1
XDG_MENU_PREFIX=lxqt-
XDG_DATA_HOME=/home/gaura/.local/share
XDG_CONFIG_HOME=/home/gaura/.config
DESKTOP_SESSION=/usr/share/xsessions/lxqt
LC_MONETARY=en_US.UTF-8
LXQT_SESSION_CONFIG=session
EDITOR=mcedit
GTK_MODULES=canberra-gtk-module
XDG_SEAT=seat0
PWD=/home/gaura/gen/university/06/Unix_Lab6
LOGNAME=gaura
XDG_SESSION_DESKTOP=
QT_QPA_PLATFORMTHEME=lxqt
XDG_SESSION_TYPE=x11
XAUTHORITY=/home/gaura/.Xauthority
HOME=/home/gaura
LC_PAPER=uk_UA.UTF-8
LANG=en_US.UTF-8
XDG_CURRENT_DESKTOP=LXQt
VTE_VERSION=6002
XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0
GTK_CSD=0
XDG_CACHE_HOME=/home/gaura/.cache
XDG_SESSION_CLASS=user
TERM=xterm
GTK_OVERLAY_SCROLLING=0
USER=gaura
DISPLAY=:0
SHLVL=1
LC_MEASUREMENT=en_US.UTF-8
XDG_VTNR=1
XDG_SESSION_ID=2
XDG_RUNTIME_DIR=/run/user/1000
QT_PLATFORM_PLUGIN=lxqt
LC_TIME=en_US.UTF-8
LC_COLLATE=en_US.UTF-8
XDG_DATA_DIRS=/home/gaura/.local/share/flatpak/exports/share:/var/lib/flatpak/exports/share:/usr/local/share:/usr/share
BROWSER=firefox
PATH=/home/gaura/gen/bin:/home/gaura/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/bin:/usr/lib/jvm/default/bin:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl
DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus
MAIL=/var/spool/mail/gaura
LC_NUMERIC=en_US.UTF-8
OLDPWD=/home/gaura/gen/university/06/Unix_Lab2
_=./1
$ gcc -Wall -o 2 2.c
$ ./2
./2 first execlp
./2 second execvp
./2 third execle
./2 fourth execve
./2
$ gcc -Wall -o 3 3.c
$ ./3
./1 first execlp
SHELL=/bin/bash
COLORTERM=truecolor
XDG_CONFIG_DIRS=/etc:/etc/xdg:/usr/share
XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session1
XDG_MENU_PREFIX=lxqt-
XDG_DATA_HOME=/home/gaura/.local/share
XDG_CONFIG_HOME=/home/gaura/.config
DESKTOP_SESSION=/usr/share/xsessions/lxqt
LC_MONETARY=en_US.UTF-8
LXQT_SESSION_CONFIG=session
EDITOR=mcedit
GTK_MODULES=canberra-gtk-module
XDG_SEAT=seat0
PWD=/home/gaura/gen/university/06/Unix_Lab6
LOGNAME=gaura
XDG_SESSION_DESKTOP=
QT_QPA_PLATFORMTHEME=lxqt
XDG_SESSION_TYPE=x11
XAUTHORITY=/home/gaura/.Xauthority
HOME=/home/gaura
LC_PAPER=uk_UA.UTF-8
LANG=en_US.UTF-8
XDG_CURRENT_DESKTOP=LXQt
VTE_VERSION=6002
XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0
GTK_CSD=0
XDG_CACHE_HOME=/home/gaura/.cache
XDG_SESSION_CLASS=user
TERM=xterm
GTK_OVERLAY_SCROLLING=0
USER=gaura
DISPLAY=:0
SHLVL=1
LC_MEASUREMENT=en_US.UTF-8
XDG_VTNR=1
XDG_SESSION_ID=2
XDG_RUNTIME_DIR=/run/user/1000
QT_PLATFORM_PLUGIN=lxqt
LC_TIME=en_US.UTF-8
LC_COLLATE=en_US.UTF-8
XDG_DATA_DIRS=/home/gaura/.local/share/flatpak/exports/share:/var/lib/flatpak/exports/share:/usr/local/share:/usr/share
BROWSER=firefox
PATH=/home/gaura/gen/bin:/home/gaura/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/bin:/usr/lib/jvm/default/bin:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl
DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus
MAIL=/var/spool/mail/gaura
LC_NUMERIC=en_US.UTF-8
OLDPWD=/home/gaura/gen/university/06/Unix_Lab2
_=./3

------------------
(program exited with code: 0)

./1 second execvp
SHELL=/bin/bash
COLORTERM=truecolor
XDG_CONFIG_DIRS=/etc:/etc/xdg:/usr/share
XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session1
XDG_MENU_PREFIX=lxqt-
XDG_DATA_HOME=/home/gaura/.local/share
XDG_CONFIG_HOME=/home/gaura/.config
DESKTOP_SESSION=/usr/share/xsessions/lxqt
LC_MONETARY=en_US.UTF-8
LXQT_SESSION_CONFIG=session
EDITOR=mcedit
GTK_MODULES=canberra-gtk-module
XDG_SEAT=seat0
PWD=/home/gaura/gen/university/06/Unix_Lab6
LOGNAME=gaura
XDG_SESSION_DESKTOP=
QT_QPA_PLATFORMTHEME=lxqt
XDG_SESSION_TYPE=x11
XAUTHORITY=/home/gaura/.Xauthority
HOME=/home/gaura
LC_PAPER=uk_UA.UTF-8
LANG=en_US.UTF-8
XDG_CURRENT_DESKTOP=LXQt
VTE_VERSION=6002
XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0
GTK_CSD=0
XDG_CACHE_HOME=/home/gaura/.cache
XDG_SESSION_CLASS=user
TERM=xterm
GTK_OVERLAY_SCROLLING=0
USER=gaura
DISPLAY=:0
SHLVL=1
LC_MEASUREMENT=en_US.UTF-8
XDG_VTNR=1
XDG_SESSION_ID=2
XDG_RUNTIME_DIR=/run/user/1000
QT_PLATFORM_PLUGIN=lxqt
LC_TIME=en_US.UTF-8
LC_COLLATE=en_US.UTF-8
XDG_DATA_DIRS=/home/gaura/.local/share/flatpak/exports/share:/var/lib/flatpak/exports/share:/usr/local/share:/usr/share
BROWSER=firefox
PATH=/home/gaura/gen/bin:/home/gaura/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/bin:/usr/lib/jvm/default/bin:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl
DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus
MAIL=/var/spool/mail/gaura
LC_NUMERIC=en_US.UTF-8
OLDPWD=/home/gaura/gen/university/06/Unix_Lab2
_=./3

------------------
(program exited with code: 0)

./1 third execle
PATH="/usr/bin"
NUMBER=third
NAME=execle

------------------
(program exited with code: 1)

./1 fourth execve
PATH="/usr/bin"
NUMBER=fourth
NAME=execve

------------------
(program exited with code: 1)

```
