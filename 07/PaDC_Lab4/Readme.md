# Звіт з лабораторної роботи №4<br>за дисципліною "Паралельні та розподілені обчислення"<br>студента групи ПА-17-2<br>Панасенка Єгора Сергійовича<br>Кафедра комп'ютерних технологій<br>ФПМ, ДНУ, 2017-2018 навч.р.

Повний проект можна знайти тут: https://github.com/gaurapanasenko/unilab/tree/master/07/PaDC_Lab4

## Постановка задачі. Написання простих програм MPI

* Напишіть проrраму на мові С, яка буде виконуватись двома процесами.
* Організуйте передачу одновимірного масиву з нульового процесу до першого.
* Обчисліть суму всіх елементів масиву і розішліть її всім процесам.
* Організуйте видачу результату на нульовому процесі.
* Використати фунціх MPI_Send та MPI_Recv.

## Опис розв'язку

Створено програму `main`, яка за допомогою MPI виконує у зазначену постановку задачі. Як початкові данні взято массив розміром 32, який заповнюється від 0 до 31.

## Отримані результати

Зкомпілюємо програму `main`.

Запустимо программу:
```shell
$ mpiexec ./main
Sum: 496, Rank: 0
```

## Вихідний текст

### Файл `main.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int worker(int argc, char* argv[], int rank, int size, char *hostname, int host_len) {
    int ierr, data_size, sum = 0, data[32], i;

    switch (rank) {
    case 1:
        MPI_Recv(data, 32, MPI_INT, 0, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i = 0; i < 32; i++) {
            sum += data[i];
        }
        for (i = 0; i < size; i++) {
            if (i != rank)
                MPI_Send(&sum, 1, MPI_INT, i, 11, MPI_COMM_WORLD);
        }
    break;
    case 0:
        for (i = 0; i < 32; i++) {
            data[i] = i;
        }
        MPI_Send(data, 32, MPI_INT, 1, 10, MPI_COMM_WORLD);
    default:
        MPI_Recv(&sum, 32, MPI_INT, 1, 11, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Sum: %i, Rank: %i\n", sum, rank);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int status;
    MPI_Init(&argc, &argv);

    int rank, size, host_len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(hostname, &host_len);
    status = worker(argc, argv, rank, size, hostname, host_len);

    MPI_Finalize();
    return status;
}
```

### Файл `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.5)

project(PaDC_Lab3 LANGUAGES C)

find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(main main.c)

target_link_libraries(main ${MPI_C_LIBRARIES})
```
