# Лабораторна робота №5, Панасенко Єгор, ПА-17-2

## Постановка задачі

Створіть окремий файл на мові C. Запишіть у ньому функцію, що приймає два параметри. Скомпілюйте динамічну бібліотеку з розширенням `.so`. Створіть основну програму, що буде викликати дану функцію. Використати статичне та динамічне завантаження бібліотеки.

## Вихідний текст розв'язку

### Файл: `main.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include "sumy.h"

int main(int argc, char **argv) {
    int arr[argc];
    for (int i = 0; i < argc; i++) {
        arr[i] = atoi(argv[i]);
    }
    printf("Summary: %i\n", sumy_array(arr, argc));
    return 0;
}
```

### Файл: `Makefile`

```makefile
CC=clang
IDIR=.
LDIR=.

DEPS=sumy.h
LIBS=-L$(LDIR) -lsumy

CFLAGS=-I$(IDIR) -O2

all: main-static main-shared

%-shared.o: %.c $(DEPS)
    $(CC) -fPIC -c -o $@ $< $(CFLAGS)

%-static.o: %.c $(DEPS)
    $(CC) -c -o $@ $< $(CFLAGS)

lib%.so: %-shared.o $(DEPS)
    $(CC) -shared -o $@ $< $(CFLAGS)

lib%.a: %-static.o $(DEPS)
    ar cr $@ $<

main-shared: main.c libsumy.so
    $(CC) -o $@ $< $(CFLAGS) $(LIBS)

main-static: main.c libsumy.a
    $(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
    rm -f *.o *.a *.so main-static main-shared
```

### Файл: `sumy.h`

```c
#ifndef SUMY_H
#define SUMY_H

int sumy_array(int arr[], int size);

#endif
```

### Файл: `sumy.c`

```c
#include "sumy.h"

int sumy_array(int* arr, int size) {
    int value = 0;
    int *i = arr, *last = arr + size;
    for (; i != last; ++i) {
        value += *i;
    }
    return value;
}
```

## Опис розв'язку

Програма `main.c` передає масив аргументів, перероблених у числа, у функцію
`sumy_array` з файлу `sumy.c` та виводить на екран результат.
Файл `Makefile` зберігає набір правил для компіляції.
У даному випадку метою є компілювання файлів `main-static` та `main-shared`,
причому `main-shared` залежить від бібліотеки `libsumy.so`,
це динамічне використання бібліотеки. У `main-static` статична бібліотека вже включена у бінарний файл,
тому не залежить від інших файлів.

Особливістю компілювання статичних бібліотек у тому що вони запаковуються у файл
`lib*.a` за допомогою `ar`. Цей файл є архівом, який зберігає скомпільовані
об'єкти.

Особливістю компілювання динамічних бібліотек у тому що вони створюються
компілятором через аргумент `-shared`,
причому усі об'єкти повинні бути скомпільовані за допомогою аргументу `-fPIC`,
що розшифровується як "position-independent code", тобто код незалежний від позиції.

## Опис тестових прикладів

Запустимо таку послідовність команд:

```shell
make
./main-static 5 7
LD_LIBRARY_PATH=. ./main-shared 5 7
LD_LIBRARY_PATH=. ./main-shared 6 8
```

Отримаємо такий результат:
```shell
$ make
clang -c -o sumy-static.o sumy.c -I. -O2
ar cr libsumy.a sumy-static.o
clang -o main-static main.c libsumy.a -I. -O2 -L. -lsumy
clang -fPIC -c -o sumy-shared.o sumy.c -I. -O2
clang -shared -o libsumy.so sumy-shared.o -I. -O2
clang -o main-shared main.c -I. -O2 -L. -lsumy
rm sumy-shared.o sumy-static.o
$ ./main-static 5 7
Summary: 12
$ LD_LIBRARY_PATH=. ./main-shared 5 7
Summary: 12
$ LD_LIBRARY_PATH=. ./main-shared 6 8
Summary: 14
```
