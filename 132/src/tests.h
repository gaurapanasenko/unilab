#ifndef TESTS_H
#define TESTS_H
#include <iostream>
#include "superint.h"
using namespace std;

// Запустить тест с номером id
int test(int id);

// Вывести цветной статус
int test_status(bool status);

// Запустить все тесты
int run_tests();

#endif
