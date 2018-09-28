#ifndef TESTS_H
#define TESTS_H
#include <string>
#include <iostream>
#include <sstream>
#include "superint.h"
using namespace std;

// Запустить тест с номером id
int test(int id);

// Вывести цветной статус
int test_status(bool status);

// Запустить все тесты
void run_tests();

#endif
