# Звіт з лабораторної роботи №5<br>за дисципліною "Паралельні та розподілені обчислення"<br>студента групи ПА-17-2<br>Панасенка Єгора Сергійовича<br>Кафедра комп'ютерних технологій<br>ФПМ, ДНУ, 2017-2018 навч.р.

Повний проект можна знайти тут: https://github.com/gaurapanasenko/unilab/tree/master/07/PaDC_Lab5

## Постановка задачі. Написання простих програм MPI

* Напишіть програму на мові С, яка буде зберігати квадратну матрицю на M процесах. Розмір матриці N обчисліть таким чином, щоб кожний процес зберігав K рядків матриці і N = k*M, де M - число запущених процесів.
* Напишіть підпрограми для перемноження матриці на вектор і навпаки. Результуючий вектор розішліть на всі процеси.
* Обчисліть час виконання даних процедур. Упевніться, що він буде приблизно однаковий для обох випадків.

## Опис розв'язку

Розроблено дві програми `matvec` та `vecmat`, перша множить матрицю на вектор, друга навпаки. Ці дві підпрограми були розроблені так, щоб значення з функції `rand`, у випадку коли `rand` видає одні і ті самі значення при перезапуску програми, утворювали транспоновану матрицю та той самий вектор, для того щоб їх множення утворювало один і той самий вектор. Ці програми приймають `k` як перший аргумент.

Код написаний так щоб він відрізнявся якомога менше. Для цього впевнимося в цьому виконавши `diff matvec.c vecmat.c`:

```diff
15c15
<             matrix[i][j] = rand() % 16;
---
>             matrix[j / (size / k) + i / k * k][i % k + j % (size / k) * k] = rand() % 16;
17c17
<     printf("Matrix <=16x16:\n");
---
>     printf("Matrix in memory <=16x16:\n");
24a25,34
>
>     printf("Matrix <=16x16:\n");
>     for (i = 0; i < min_size; i++) {
>         for (j = 0; j < min_size; j++) {
>             // I do not know how I did it, but it works as I want
>             printf("%5i ", matrix[i / (size / k) + j / k * k][j % k + i % (size / k) * k]);
>         }
>         printf("\n");
>     }
>     printf("\n");
27c37
< int gen_and_broadcast_array(int comm_rank, int comm_size, int k, int (*matrix_part)[comm_size * k], int * vector) {
---
> int gen_and_broadcast_array(int comm_rank, int comm_size, int k, int (*matrix_part)[k], int * vector) {
73c83
<     int (*matrix_part)[size], *vector, *res_vector;
---
>     int (*matrix_part)[k], *vector, *res_vector;
77c87
<     matrix_part = (int (*)[size])malloc(k * size * sizeof(int));
---
>     matrix_part = (int (*)[size])malloc(size * k * sizeof(int));
88c98
<             res_vector[i + comm_rank * k] += matrix_part[i][j] * vector[j];
---
>             res_vector[i + comm_rank * k] += vector[j] * matrix_part[j][i];
```

## Отримані результати

Запустимо вище зазначені програми на двох потоках та при `k` рівним 4, тобто розмір матриці буде 8x8.

```shell
[gaura@gauramid build-PaDC_Lab5-Desktop-Debug]$ mpiexec -n 2 ./matvec 4
Matrix <=16x16:
    7     6     9     3     1    15    10    12
    9    13    10    11     2    11     3     6
   12     2     4     8    11     8     7    13
    6    10    14     3     3    15     9    10
    6     2    13     7     1     8     3    10
    5    13     5     7     8     9    14     4
   11     2    13     6    11     4     4     1
   14     2     4     1     1    13    12     7

Vector:
    0     9    14     1     1     1    12     7

Res vector in rank    1:  403   359   276   485   322   407   276   282 . Run in 0.000002 s.
Res vector in rank    0:  403   359   276   485   322   407   276   282 . Run in 0.000001 s.
[gaura@gauramid build-PaDC_Lab5-Desktop-Debug]$ mpiexec -n 2 ./vecmat 4
Matrix in memory <=16x16:
    7     9    12     6     6    13     2    10
    9    10     4    14     3    11     8     3
    1     2    11     3    15    11     8    15
   10     3     7     9    12     6    13    10
    6     5    11    14     2    13     2     2
   13     5    13     4     7     7     6     1
    1     8    11     1     8     9     4    13
    3    14     4    12    10     4     1     7

Matrix <=16x16:
    7     9    12     6     6     5    11    14
    6    13     2    10     2    13     2     2
    9    10     4    14    13     5    13     4
    3    11     8     3     7     7     6     1
    1     2    11     3     1     8    11     1
   15    11     8    15     8     9     4    13
   10     3     7     9     3    14     4    12
   12     6    13    10    10     4     1     7

Vector:
    0     9    14     1     1     1    12     7

Res vector in rank    1:  403   359   276   485   322   407   276   282 . Run in 0.000001 s.
Res vector in rank    0:  403   359   276   485   322   407   276   282 . Run in 0.000001 s.
```

Як бачимо при матриця та вектор згенерувались однаковими, з єдиною різницею, що матриця транспонована. Таким чином ми отримали однаковий вектор при множенні. 

Тепер спробуємо теж саме повторити з `k` рівним 1024.

```shell
[gaura@gauramid build-PaDC_Lab5-Desktop-Debug]$ mpiexec -n 2 ./matvec 1024
Matrix <=16x16:
    7     6     9     3     1    15    10    12     9    13    10    11     2    11     3     6
    7     4     8     9    12     0    13     4    14     1     2    13     4     0     4     6
    1     4     8     9    10     8     3    10     8    14    15     0     9     4     7     1
    2    12     9     2     5    11     4     2     7     5     3     7    11     8     3     0
    3     5     5    14    12     8     5     8     6    15    14     1     0    14     1    12
   15     5     3     6    15    11    15     0     4     5     7     2     4     0    10     9
    7     8    14     4     0     7    14     7     4    10     4     6    14     0     8     6
    0    15    14    10     8    13     8     4     5    14    15    11     3    11    12    13
    7     8     0    14     5     6    10     0    12     5     9    12    14     0    11     0
   13     2     5     8     0     8    14     8     4     5     1    12     2     0     3     1
   11    10     4     5     3     0    12     5     3     3    13     6     3    15     9     2
    3    10     3     9     9     5     8    10     4     5     0    15     7     7     8     0
    4    14     4    10    14    15     6    15     1     2     1     1     8    12    15     1
    9     8     4     5    12    12    14    12     8    14    15     3     0     7     1     4
    4     6     9     8    12     4     8    14     2     8     9     4    11     1     3     3
    4     2     6     9     1     6     9     4     7     8    14     9     4     1    15     3

Vector:
    5     6    12     7     3     0     7    15     0    12    10    10    12     9     5    12

Res vector in rank    1:116424 114282 115935 113586 115795 117237 119042 113869 115312 115010 115364 115768 117207 116495 112312 113657 . 
Run in 0.047171 s.
Res vector in rank    0:116424 114282 115935 113586 115795 117237 119042 113869 115312 115010 115364 115768 117207 116495 112312 113657 . 
Run in 0.046409 s.
[gaura@gauramid build-PaDC_Lab5-Desktop-Debug]$ mpiexec -n 2 ./vecmat 1024
Matrix in memory <=16x16:
    7     7     1     2     3    15     7     0     7    13    11     3     4     9     4     4
    9     8     8     9     5     3    14    14     0     5     4     3     4     4     9     6
    1    12    10     5    12    15     0     8     5     0     3     9    14    12    12     1
   10    13     3     4     5    15    14     8    10    14    12     8     6    14     8     9
    9    14     8     7     6     4     4     5    12     4     3     4     1     8     2     7
   10     2    15     3    14     7     4    15     9     1    13     0     1    15     9    14
    2     4     9    11     0     4    14     3    14     2     3     7     8     0    11     4
    3     4     7     3     1    10     8    12    11     3     9     8    15     1     3    15
   12     9     4    12     8     9     7     8     2     7     1     6     7    13     6    11
    4     9    14    10    13     3    11     9     9     4    15     7    14     4     3    15
   11    13    14     5    14    11    11     6    15     2    15    10    14     1    14     9
    7     5     4     4     1     8     6     2    12     7    15    11     1     4    14     5
    6     7     9    13     9     2     5     3     3     0     6     8     0     7     2     4
   14     0     5     0     8     4     0     1    15     9    12     2     7    12     0     5
    3     7     3     1     9     3    12     6     2     4     1     4     0     1    10     0
    9     5    13    14     9     3     6     6     7     3     4     8    12     6     3     4

Matrix <=16x16:
    7     7     1     2     3    15     7     0     7    13    11     3     4     9     4     4
    6     4     4    12     5     5     8    15     8     2    10    10    14     8     6     2
    9     8     8     9     5     3    14    14     0     5     4     3     4     4     9     6
    3     9     9     2    14     6     4    10    14     8     5     9    10     5     8     9
    1    12    10     5    12    15     0     8     5     0     3     9    14    12    12     1
   15     0     8    11     8    11     7    13     6     8     0     5    15    12     4     6
   10    13     3     4     5    15    14     8    10    14    12     8     6    14     8     9
   12     4    10     2     8     0     7     4     0     8     5    10    15    12    14     4
    9    14     8     7     6     4     4     5    12     4     3     4     1     8     2     7
   13     1    14     5    15     5    10    14     5     5     3     5     2    14     8     8
   10     2    15     3    14     7     4    15     9     1    13     0     1    15     9    14
   11    13     0     7     1     2     6    11    12    12     6    15     1     3     4     9
    2     4     9    11     0     4    14     3    14     2     3     7     8     0    11     4
   11     0     4     8    14     0     0    11     0     0    15     7    12     7     1     1
    3     4     7     3     1    10     8    12    11     3     9     8    15     1     3    15
    6     6     1     0    12     9     6    13     0     1     2     0     1     4     3     3

Vector:
    5     6    12     7     3     0     7    15     0    12    10    10    12     9     5    12

Res vector in rank    1:116424 114282 115935 113586 115795 117237 119042 113869 115312 115010 115364 115768 117207 116495 112312 113657 . 
Run in 0.326013 s.
Res vector in rank    0:116424 114282 115935 113586 115795 117237 119042 113869 115312 115010 115364 115768 117207 116495 112312 113657 . 
Run in 0.286498 s.
```

Отримали знову однаковий результат, але помічаємо іншу особливість, що множення матриці на вектор працює значно швидше ніж множення вектору на матрицю, хоча якщо ми подивимось різницю у коді, то побачимо зміну тільки у способі множення. Це пов'язано з тим, що процесор здатний кешувати наперед блоки пам'яті, які використовуються, а так як при множенні матриці ми переходимо від строки до строки, які знаходяться віддалено один від одного, це спричиняє ріст часу виконання.

Тепер нас цікавить чи є приріст швидкості у порівнянні з одним потоком і тому запустимо програми з одним потоком та `k` рівним 2048, щоб отримати той самий розмір матриці.

```shell
[gaura@gauramid build-PaDC_Lab5-Desktop-Debug]$ mpiexec -n 1 ./matvec 2048
Matrix <=16x16:
    7     6     9     3     1    15    10    12     9    13    10    11     2    11     3     6
    7     4     8     9    12     0    13     4    14     1     2    13     4     0     4     6
    1     4     8     9    10     8     3    10     8    14    15     0     9     4     7     1
    2    12     9     2     5    11     4     2     7     5     3     7    11     8     3     0
    3     5     5    14    12     8     5     8     6    15    14     1     0    14     1    12
   15     5     3     6    15    11    15     0     4     5     7     2     4     0    10     9
    7     8    14     4     0     7    14     7     4    10     4     6    14     0     8     6
    0    15    14    10     8    13     8     4     5    14    15    11     3    11    12    13
    7     8     0    14     5     6    10     0    12     5     9    12    14     0    11     0
   13     2     5     8     0     8    14     8     4     5     1    12     2     0     3     1
   11    10     4     5     3     0    12     5     3     3    13     6     3    15     9     2
    3    10     3     9     9     5     8    10     4     5     0    15     7     7     8     0
    4    14     4    10    14    15     6    15     1     2     1     1     8    12    15     1
    9     8     4     5    12    12    14    12     8    14    15     3     0     7     1     4
    4     6     9     8    12     4     8    14     2     8     9     4    11     1     3     3
    4     2     6     9     1     6     9     4     7     8    14     9     4     1    15     3

Vector:
    5     6    12     7     3     0     7    15     0    12    10    10    12     9     5    12

Res vector in rank    0:116424 114282 115935 113586 115795 117237 119042 113869 115312 115010 115364 115768 117207 116495 112312 113657 . 
Run in 0.092395 s.
[gaura@gauramid build-PaDC_Lab5-Desktop-Debug]$ mpiexec -n 1 ./vecmat 2048
Matrix in memory <=16x16:
    7     7     1     2     3    15     7     0     7    13    11     3     4     9     4     4
    6     4     4    12     5     5     8    15     8     2    10    10    14     8     6     2
    9     8     8     9     5     3    14    14     0     5     4     3     4     4     9     6
    3     9     9     2    14     6     4    10    14     8     5     9    10     5     8     9
    1    12    10     5    12    15     0     8     5     0     3     9    14    12    12     1
   15     0     8    11     8    11     7    13     6     8     0     5    15    12     4     6
   10    13     3     4     5    15    14     8    10    14    12     8     6    14     8     9
   12     4    10     2     8     0     7     4     0     8     5    10    15    12    14     4
    9    14     8     7     6     4     4     5    12     4     3     4     1     8     2     7
   13     1    14     5    15     5    10    14     5     5     3     5     2    14     8     8
   10     2    15     3    14     7     4    15     9     1    13     0     1    15     9    14
   11    13     0     7     1     2     6    11    12    12     6    15     1     3     4     9
    2     4     9    11     0     4    14     3    14     2     3     7     8     0    11     4
   11     0     4     8    14     0     0    11     0     0    15     7    12     7     1     1
    3     4     7     3     1    10     8    12    11     3     9     8    15     1     3    15
    6     6     1     0    12     9     6    13     0     1     2     0     1     4     3     3

Matrix <=16x16:
    7     7     1     2     3    15     7     0     7    13    11     3     4     9     4     4
    6     4     4    12     5     5     8    15     8     2    10    10    14     8     6     2
    9     8     8     9     5     3    14    14     0     5     4     3     4     4     9     6
    3     9     9     2    14     6     4    10    14     8     5     9    10     5     8     9
    1    12    10     5    12    15     0     8     5     0     3     9    14    12    12     1
   15     0     8    11     8    11     7    13     6     8     0     5    15    12     4     6
   10    13     3     4     5    15    14     8    10    14    12     8     6    14     8     9
   12     4    10     2     8     0     7     4     0     8     5    10    15    12    14     4
    9    14     8     7     6     4     4     5    12     4     3     4     1     8     2     7
   13     1    14     5    15     5    10    14     5     5     3     5     2    14     8     8
   10     2    15     3    14     7     4    15     9     1    13     0     1    15     9    14
   11    13     0     7     1     2     6    11    12    12     6    15     1     3     4     9
    2     4     9    11     0     4    14     3    14     2     3     7     8     0    11     4
   11     0     4     8    14     0     0    11     0     0    15     7    12     7     1     1
    3     4     7     3     1    10     8    12    11     3     9     8    15     1     3    15
    6     6     1     0    12     9     6    13     0     1     2     0     1     4     3     3

Vector:
    5     6    12     7     3     0     7    15     0    12    10    10    12     9     5    12

Res vector in rank    0:116424 114282 115935 113586 115795 117237 119042 113869 115312 115010 115364 115768 117207 116495 112312 113657 . 
Run in 0.551603 s.
```

Як ми і сподівалися з двома потоками програми працюють швидше приблизно у 2 рази.

## Вихідний текст

### Файл `matvec.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

#define MPI_WTIME_IS_GLOBAL 1

const int BUFF_SIZE = 2048;

int gen_array(int size, int k, int (*matrix)[size]) {
    int min_size = size < 16 ? size : 16, i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            matrix[i][j] = rand() % 16;

    printf("Matrix <=16x16:\n");
    for (i = 0; i < min_size; i++) {
        for (j = 0; j < min_size; j++) {
            printf("%5i ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int gen_and_broadcast_array(int comm_rank, int comm_size, int k, int (*matrix_part)[comm_size * k], int * vector) {
    // Calculate sizes
    int size = comm_size * k;
    int part_size = k * size;
    int min_size = size < 16 ? size : 16;

    if (comm_rank == 0) {
        // Initalize matrix and vector and allocate memory for them
        int (*matrix)[size], *new_vector;
        matrix = (int (*)[size])malloc(size * size * sizeof(int));
        new_vector = (int *)malloc(size * sizeof(int));

        // Generate data
        gen_array(size, k, matrix);
        printf("Vector:\n");
        for (int i = 0; i < size; i++) {
            new_vector[i] = rand() % 16;
            if (i < 16)
                printf("%5i ", new_vector[i]);
        }
        printf("\n\n");

        // Broadcast data
        memcpy(matrix_part, matrix, part_size * sizeof(int));
        memcpy(vector, new_vector, size * sizeof(int));
        for (int i = 1; i < comm_size; i++) {
            MPI_Send(matrix + k * i, part_size, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
            MPI_Send(new_vector, size, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
        }

        // Free memory
        free(matrix);
        free(new_vector);
    } else {
        MPI_Recv(matrix_part, part_size, MPI_INT, 0, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(vector, size, MPI_INT, 0, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

int worker(int argc, char* argv[], int comm_rank, int comm_size) {
    // Calculate sizes
    int k = atoi(argv[1]);
    int size = comm_size * k;
    int part_size = comm_size * size;

    // Initialize needed variables
    int (*matrix_part)[size], *vector, *res_vector;
    double start, end;

    // Allocate memory for matrix and vectors
    matrix_part = (int (*)[size])malloc(k * size * sizeof(int));
    vector = (int *)malloc(size * sizeof(int));
    res_vector = (int *)calloc(size, sizeof(int));

    // Generate and broadcast matrix
    gen_and_broadcast_array(comm_rank, comm_size, k, matrix_part, vector);

    // Calculate
    start = MPI_Wtime();
    for (int i = 0; i < k; i++)
        for (int j = 0; j < size; j++)
            res_vector[i + comm_rank * k] += matrix_part[i][j] * vector[j];
    end = MPI_Wtime();

    // Sync rest of result vector with other processes
    for (int i = 0; i < comm_size; i++) {
        if (i > comm_rank) {
            MPI_Send(res_vector + comm_rank * k, k, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
            MPI_Recv(res_vector + i * k, k, MPI_INT, i, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (i < comm_rank) {
            MPI_Recv(res_vector + i * k, k, MPI_INT, i, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(res_vector + comm_rank * k, k, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
        }
    }

    // Print result data
    printf("Res vector in rank %4i:", comm_rank);
    size = size < 16 ? size : 16;
    for (int i = 0; i < size; i++)
        printf("%5i ", res_vector[i]);
    printf(". Run in %f s.\n", end - start);
    fflush(stdout);

    // Free allocated memory
    free(matrix_part);
    free(vector);
    free(res_vector);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s must have numeric argument, "
                        "that will represent size matrix (k)\n",
                argv[0]);
        return 1;
    }
    int status;
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    status = worker(argc, argv, rank, size);

    MPI_Finalize();
    return status;
}
```

### Файл `vecmat.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mpi.h"

#define MPI_WTIME_IS_GLOBAL 1

const int BUFF_SIZE = 2048;

int gen_array(int size, int k, int (*matrix)[size]) {
    int min_size = size < 16 ? size : 16, i, j;
    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            matrix[j / (size / k) + i / k * k][i % k + j % (size / k) * k] = rand() % 16;

    printf("Matrix in memory <=16x16:\n");
    for (i = 0; i < min_size; i++) {
        for (j = 0; j < min_size; j++) {
            printf("%5i ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Matrix <=16x16:\n");
    for (i = 0; i < min_size; i++) {
        for (j = 0; j < min_size; j++) {
            // I do not know how I did it, but it works as I want
            printf("%5i ", matrix[i / (size / k) + j / k * k][j % k + i % (size / k) * k]);
        }
        printf("\n");
    }
    printf("\n");
}

int gen_and_broadcast_array(int comm_rank, int comm_size, int k, int (*matrix_part)[k], int * vector) {
    // Calculate sizes
    int size = comm_size * k;
    int part_size = k * size;
    int min_size = size < 16 ? size : 16;

    if (comm_rank == 0) {
        // Initalize matrix and vector and allocate memory for them
        int (*matrix)[size], *new_vector;
        matrix = (int (*)[size])malloc(size * size * sizeof(int));
        new_vector = (int *)malloc(size * sizeof(int));

        // Generate data
        gen_array(size, k, matrix);
        printf("Vector:\n");
        for (int i = 0; i < size; i++) {
            new_vector[i] = rand() % 16;
            if (i < 16)
                printf("%5i ", new_vector[i]);
        }
        printf("\n\n");

        // Broadcast data
        memcpy(matrix_part, matrix, part_size * sizeof(int));
        memcpy(vector, new_vector, size * sizeof(int));
        for (int i = 1; i < comm_size; i++) {
            MPI_Send(matrix + k * i, part_size, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
            MPI_Send(new_vector, size, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
        }

        // Free memory
        free(matrix);
        free(new_vector);
    } else {
        MPI_Recv(matrix_part, part_size, MPI_INT, 0, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(vector, size, MPI_INT, 0, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

int worker(int argc, char* argv[], int comm_rank, int comm_size) {
    // Calculate sizes
    int k = atoi(argv[1]);
    int size = comm_size * k;
    int part_size = comm_size * size;

    // Initialize needed variables
    int (*matrix_part)[k], *vector, *res_vector;
    double start, end;

    // Allocate memory for matrix and vectors
    matrix_part = (int (*)[size])malloc(size * k * sizeof(int));
    vector = (int *)malloc(size * sizeof(int));
    res_vector = (int *)calloc(size, sizeof(int));

    // Generate and broadcast matrix
    gen_and_broadcast_array(comm_rank, comm_size, k, matrix_part, vector);

    // Calculate
    start = MPI_Wtime();
    for (int i = 0; i < k; i++)
        for (int j = 0; j < size; j++)
            res_vector[i + comm_rank * k] += vector[j] * matrix_part[j][i];
    end = MPI_Wtime();

    // Sync rest of result vector with other processes
    for (int i = 0; i < comm_size; i++) {
        if (i > comm_rank) {
            MPI_Send(res_vector + comm_rank * k, k, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
            MPI_Recv(res_vector + i * k, k, MPI_INT, i, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (i < comm_rank) {
            MPI_Recv(res_vector + i * k, k, MPI_INT, i, 10 + comm_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(res_vector + comm_rank * k, k, MPI_INT, i, 10 + i, MPI_COMM_WORLD);
        }
    }

    // Print result data
    printf("Res vector in rank %4i:", comm_rank);
    size = size < 16 ? size : 16;
    for (int i = 0; i < size; i++)
        printf("%5i ", res_vector[i]);
    printf(". Run in %f s.\n", end - start);
    fflush(stdout);

    // Free allocated memory
    free(matrix_part);
    free(vector);
    free(res_vector);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s must have numeric argument, "
                        "that will represent size matrix (k)\n",
                argv[0]);
        return 1;
    }
    int status;
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    status = worker(argc, argv, rank, size);

    MPI_Finalize();
    return status;
}
```

### Файл `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.5)

project(PaDC_Lab5 LANGUAGES C)

find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(main main.c)

target_link_libraries(main ${MPI_C_LIBRARIES})
```
