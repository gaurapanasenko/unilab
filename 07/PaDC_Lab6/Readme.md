# Звіт з лабораторної роботи №6<br>за дисципліною "Паралельні та розподілені обчислення"<br>студента групи ПА-17-2<br>Панасенка Єгора Сергійовича<br>Кафедра комп'ютерних технологій<br>ФПМ, ДНУ, 2017-2018 навч.р.

Повний проект можна знайти тут: https://github.com/gaurapanasenko/unilab/tree/master/07/PaDC_Lab6

## Постановка задачі. Написання простих програм MPI

* Організуйте розсилку інформації від одного процесса всім остальным членам деякої області зв'язку (MPI_Bcast).
* Зберіть (MPI_Gather) розподілені по процессам массиви в один массив зі збереженням його в адресном просторі виділеного (root) процесса (MPI_Gather).
* Проведіть глобальні обчислювальні операції (sum, min, max и min) над даними, які розташовані в адресних просторах різних процесів: зі збереженням результата в адресном просторі одного процесса (MPI_Reduce).
* Організуйте видачу відповідних результатів на нульовому процесі.

## Опис розв'язку

Створено програму `main`, яка за допомогою MPI виконує у зазначену постановку задачі. Як початкові данні взято массив розміром 32, який заповнюється від 0 до 31.

## Отримані результати

Зкомпілюємо програму `main`.

Запустимо программу:
```shell
$ mpiexec -n 2 ./main
Sum: 496, Max: 31, Min: 0, Rank: 0
Gathered array:
    0     1     2     3     4     5     6     7
    8     9    10    11    12    13    14    15
   16    17    18    19    20    21    22    23
   24    25    26    27    28    29    30    31
    0     1     2     3     4     5     6     7
    8     9    10    11    12    13    14    15
   16    17    18    19    20    21    22    23
   24    25    26    27    28    29    30    31
```

## Вихідний текст

### Файл `main.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

static const int ROOT = 0;

int worker(int argc, char* argv[], int rank, int size, char *hostname, int host_len) {
    int ierr, data_size, data[32], *new_data, i, size_block, start, end;
    int sum_local = 0, sum = 0;
    int min_local, min;
    int max_local, max;

    new_data = (int *)malloc((unsigned long)size * 32 * sizeof(int));

    if (rank == ROOT) {
        for (i = 0; i < 32; i++) {
            data[i] = i;
        }
    }
    MPI_Bcast(data, 32, MPI_INT, ROOT, MPI_COMM_WORLD);
    MPI_Gather(data, 32, MPI_INT, new_data, 32, MPI_INT, ROOT, MPI_COMM_WORLD);

    size_block = 32 / size;
    start = rank * size_block;
    end = start + size_block;

    for(i = start; i < end; i++){
        sum_local += data[i];
    }
    MPI_Reduce(&sum_local, &sum, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);

    max_local = data[start];
    for(i = start + 1; i < end; i++){
        if (data[i] > max_local)
            max_local = data[i];
    }
    MPI_Reduce(&max_local, &max, 1, MPI_INT, MPI_MAX, ROOT, MPI_COMM_WORLD);

    min_local = data[start];
    for(i = start + 1; i < end; i++){
        if (data[i] < min_local)
            min_local = data[i];
    }
    MPI_Reduce(&min_local, &min, 1, MPI_INT, MPI_MIN, ROOT, MPI_COMM_WORLD);

    if (rank == ROOT) {
        printf("Sum: %i, Max: %i, Min: %i, Rank: %i\n",
               sum, max, min, rank);
        printf("Gathered array:");
        for (int i = 0; i < 32 * size; i++) {
            if (i % 8 == 0)
                printf("\n");
            printf("%5i ", new_data[i]);
        }
        printf("\n");
    }
    free(new_data);
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

project(PaDC_Lab6 LANGUAGES C)

find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(main main.c)

target_link_libraries(main ${MPI_C_LIBRARIES})
```
