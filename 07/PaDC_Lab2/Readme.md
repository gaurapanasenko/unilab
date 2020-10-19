# Звіт з лабораторної роботи №2<br>за дисципліною "Паралельні та розподілені обчислення"<br>студента групи ПА-17-2<br>Панасенка Єгора Сергійовича<br>Кафедра комп'ютерних технологій<br>ФПМ, ДНУ, 2017-2018 навч.р.

Повний проект можна знайти тут: https://github.com/gaurapanasenko/unilab/tree/master/07/PaDC_Lab2

## Постановка задачі. Використання OpenMP

* Встановіть MPICH останньої версії, який можна загрузити з сайту http://www.mcs.anl.gov/research/projects/mpich2/
* Вивчіть правила виклику програми mpiexec
* Відкомпілюйте і виконайте тестову програму pi.c з різною кількість процесів на одному компютері.
* Опишіть процес перевірки встановленого програмного забезпечення.

## Опис розв'язку

Створено програму `hello_world`, яка використовує MPI.

## Отримані результати

Зкомпілюємо програму `hello_world`.

Запустимо послідовно программу напряму, з одним потоком, з 2 та 3 потоками, отримаємо такий вивід:
```shell
$ ./hello_world
Hello, World!
$ mpiexec -n 1 ./hello_world
Hello, World!
$ mpiexec -n 2 ./hello_world
Hello, World!
Hello, World!
$ mpiexec -n 3 ./hello_world
--------------------------------------------------------------------------
There are not enough slots available in the system to satisfy the 3
slots that were requested by the application:

  ./hello_world

Either request fewer slots for your application, or make more slots
available for use.

A "slot" is the Open MPI term for an allocatable unit where we can
launch a process.  The number of slots available are defined by the
environment in which Open MPI processes are run:

  1. Hostfile, via "slots=N" clauses (N defaults to number of
     processor cores if not provided)
  2. The --host command line parameter, via a ":N" suffix on the
     hostname (N defaults to 1 if not provided)
  3. Resource manager (e.g., SLURM, PBS/Torque, LSF, etc.)
  4. If none of a hostfile, the --host command line parameter, or an
     RM is present, Open MPI defaults to the number of processor cores

In all the above cases, if you want Open MPI to default to the number
of hardware threads instead of the number of processor cores, use the
--use-hwthread-cpus option.

Alternatively, you can use the --oversubscribe option to ignore the
number of available slots when deciding the number of processes to
launch.
--------------------------------------------------------------------------
```

## Вихідний текст

### Файл `hello_world.c`

```c
#include "mpi.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    printf("Hello, World!\n");
    MPI_Finalize();
    return 0;
}
```

### Файл `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.5)

project(PaDC_Lab2 LANGUAGES C)

find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_executable(hello_world hello_world.c)

target_link_libraries(hello_world ${MPI_C_LIBRARIES})
```
