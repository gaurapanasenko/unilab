#include "tests.h"

using namespace std;

int test(int id) {
	int x = 1;
	if (id == x++) {
		string str = "1234";
		cout << "Тест " << id << ": Инициализация.\n"
			 << "    Вводим \"" << str << "\" в SuperInt и сравниваем вывод\n"
			 << "    объекта со стокой \"" << str << "\"\n";
		SuperInt num(str);
		cout << "  Получили: " << num << endl;
		return test_status(num.get_string() == str);
	} else if (id == x++) {
		string str = "1231231312312313-2312312313123";
		string str2 = "0";
		cout << "Тест " << id << ": Инициализация с неверной строкой.\n"
			 << "    Вводим \"" << str << "\" в SuperInt и сравниваем вывод\n"
			 << "    объекта со стокой \"" << str2 << "\"\n";
		SuperInt num(str);
		cout << "  Получили: " << num << endl;
		return test_status(num.get_string() == str2);
	} else if (id == x++) {
		string str = "1234";
		cout << "Тест " << id << ": Присваивание.\n"
			 << "  Вводим \"" << str << "\" в SuperInt через присваивание и\n"
			 << "  сравниваем вывод объекта со стокой \"" << str << "\"\n";
		SuperInt num;
		num = str;
		cout << "  Получили: " << num << endl;
		return test_status(num.get_string() == str);
	} else if (id == x++) {
		string str = "1234567891357924680";
		cout << "Тест " << id << ": Ввод больших чисел.\n"
			 << "  Вводим \"" << str << "\" в SuperInt и\n"
			 << "  сравниваем вывод объекта со стокой \"" << str << "\"\n";
		SuperInt num(str);
		cout << "  Получили: " << num << endl;
		return test_status(num.get_string() == str);
	} else if (id == x++) {
		string str = "-1234567891357924680";
		cout << "Тест " << id << ": Ввод отриательных больших чисел.\n"
			 << "  Вводим \"" << str << "\" в SuperInt и\n"
			 << "  сравниваем вывод объекта со стокой \"" << str << "\"\n";
		SuperInt num(str);
		cout << "  Получили: " << num << endl;
		return test_status(num.get_string() == str);
	} else if (id == x++) {
		string str = "-1234567891357924680";
		cout << "Тест " << id << ": Конструктор копирования.\n"
			 << "  Вводим \"" << str << "\" в SuperInt и копируем в другой\n"
			 << "  SuperInt сравниваем вывод объекта со стокой \""<<str<<"\"\n";
		SuperInt num(str);
		SuperInt num2(num);
		cout << "  Получили: " << num2 << endl;
		return test_status(num.get_string() == str);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "547509411375863014";
		cout << "Тест " << id << ": Сравнение одинаковых чисел.\n"
			 << "  " << str1 << " == " << str2 << endl;
		SuperInt num1(str1), num2(str2);
		return test_status(num1 == num2);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "-547509411375863014";
		cout << "Тест " << id << ": Сравнение одинаковых по модулю чисел.\n"
			 << "  " << str1 << " > " << str2 << endl;
		SuperInt num1(str1), num2(str2);
		return test_status(num1 > num2);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "47509411375863014";
		cout << "Тест " << id << ": Сравнение разных чисел.\n"
			 << "  " << str1 << " > " << str2 << endl;
		SuperInt num1(str1), num2(str2);
		return test_status(num1 > num2);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "54750941137586301";
		cout << "Тест " << id << ": Сравнение разных чисел.\n"
			 << "  " << str1 << " > " << str2 << endl;
		SuperInt num1(str1), num2(str2);
		return test_status(num1 > num2);
	} else if (id == x++) {
		string str1 = "-547509411375863014";
		string str2 = "54750941137586301";
		cout << "Тест " << id << ": Сравнение разных чисел.\n"
			 << "  " << str1 << " < " << str2 << endl;
		SuperInt num1(str1), num2(str2);
		return test_status(num1 < num2);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "24440630276";
		cout << "Тест " << id << ": Сравнение разных чисел.\n"
			 << "  " << str1 << " > " << str2 << endl;
		SuperInt num1(str1), num2(str2);
		return test_status(num1 > num2);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "1";
		cout << "Тест " << id << ": Сравнение разных чисел.\n"
			 << "  " << str1 << " > " << str2 << endl;
		SuperInt num1(str1), num2(str2);
		return test_status(num1 > num2);
	} else if (id == x++) {
		string str1 = "12345";
		string str2 = "23456";
		string str3 = "35801";
		cout << "Тест " << id << ": Сложение.\n"
			 << "  " << str1 << " + " << str2 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 + num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "24440630276";
		string str3 = "547509435816493290";
		cout << "Тест " << id << ": Сложение огромных чисел.\n"
			 << "  " << str1 << " + " << str2 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 + num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "-547509411375863014";
		string str2 = "24440630276";
		string str3 = "-547509386935232738";
		cout << "Тест " << id << ": Сложение отрицательного числа и "
			 << "положительного.\n" << "  " << str1 << " + " << str2
			 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 + num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "-24440630276";
		string str3 = "547509386935232738";
		cout << "Тест " << id << ": Сложение отрицательного числа и "
			 << "положительного.\n" << "  " << str1 << " + " << str2
			 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 + num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "-12345";
		string str2 = "-23456";
		string str3 = "-35801";
		cout << "Тест " << id << ": Сложение отрицательных чисел.\n"
			 << "  " << str1 << " + " << str2 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 + num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "24440630276";
		string str3 = "547509386935232738";
		cout << "Тест " << id << ": вычитание огромных чисел.\n"
			 << "  " << str1 << " - " << str2 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 - num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "-547509411375863014";
		string str2 = "24440630276";
		string str3 = "-547509435816493290";
		cout << "Тест " << id << ": вычитание отрицательного числа и "
			 << "положительного.\n" << "  " << str1 << " - " << str2
			 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 - num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "-24440630276";
		string str3 = "547509435816493290";
		cout << "Тест " << id << ": вычитание отрицательного числа и "
			 << "положительного.\n" << "  " << str1 << " - " << str2
			 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 - num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "-12345";
		string str2 = "-23456";
		string str3 = "-11111";
		cout << "Тест " << id << ": вычитание отрицательных чисел.\n"
			 << "  " << str1 << " - " << str2 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 - num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "24440630276";
		string str3 = "13381475096067856395597011864";
		cout << "Тест " << id << ": Умножение огромных чисел.\n"
			 << "  " << str1 << " * " << str2 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 * num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "-547509411375863014";
		string str2 = "24440630276";
		string str3 = "-13381475096067856395597011864";
		cout << "Тест " << id << ": Умножение отрицательного числа и "
			 << "положительного.\n" << "  " << str1 << " * " << str2
			 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 * num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "547509411375863014";
		string str2 = "-24440630276";
		string str3 = "-13381475096067856395597011864";
		cout << "Тест " << id << ": Умножение отрицательного числа и "
			 << "положительного.\n" << "  " << str1 << " * " << str2
			 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 * num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	} else if (id == x++) {
		string str1 = "-12345";
		string str2 = "-23456";
		string str3 = "289564320";
		cout << "Тест " << id << ": Умножение отрицательных чисел.\n"
			 << "  " << str1 << " * " << str2 << " = " << str3 << endl;
		SuperInt num1(str1), num2(str2), num3;
		num3 = num1 * num2;
		cout << "  Получили: " << num3 << endl;
		return test_status(num3.get_string() == str3);
	}
	return 0;
}

int run_tests() {
	int x = 0, y = 0, z = 0;
	for (int i = 1; (x = test(i)); i++)
		if (x == 1) y++; else z++;
	cout << "Всего пройденных тестов:      " << y + z << endl;
	cout << "Количество тестов с ошибками: \033[0;31m" << y << "\033[0m\n";
	cout << "Количество успешных тестов:   \033[0;32m" << z << "\033[0m\n";
	return y;
}

int test_status(bool status) {
	cout << "  Статус: ";
	if (status) cout << "\033[0;32mOK\033[0m\n";
	else cout << "\033[0;31mERROR\033[0m\n";
	return ((int)status) + 1;
}
