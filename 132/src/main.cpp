#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "superint.h"
#include "tests.h"

using namespace std;

//~ std::ostream &operator<<(std::ostream &output, vector<SuperInt>& right) {
	//~ struct winsize w;
	//~ ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	//~ printf ("lines %d\n", w.ws_row);
	//~ printf ("columns %d\n", w.ws_col);
	//~ int line = 0;
	//~ for (size_t i = 0; i < right.size(); i++) {
		//~ stringstream ss;
		//~ ss << right[i];
		//~ if (line + ss.str().size() + 1 > w.ws_col) {
			//~ line = 0;
			//~ output << endl;
		//~ }
		//~ output << ss.str() << " ";
		//~ line += ss.str().size() + 1;
	//~ }
	//~ output << endl;
	//~ return output;
//~ }

std::ostream &operator<<(std::ostream &output, const vector<SuperInt>& right) {
	output << "array(" << right.size() << ") {\n";
	for (size_t i = 0; i < right.size(); i++) {
		output << "\t[" << i << "] => " << right[i] << endl;
	}
	output << "}\n";
	return output;
}

bool is_in(const string& str, const string arr[], const int& size) {
	for (int i = 0; i < size; i++)
		if (str == arr[i]) return 1;
	return 0;
}

int main() {
	srand(time(NULL));
	string answer = "", buf;
	vector<SuperInt> arr;
	static const string show_arr[] = {"3", "5", "7", "8", "9", "10", "11","12"};
	static const int show_arr_size = 8;
	size_t tmp0 = 0, tmp1 = 0, tmp2 = 0, tmp3 = 0;
	while (answer != "0") {
		cout << " 1. Запустить тесты\n"
			 << " 2. Добавить число в конец массива\n"
			 << " 3. Удалить число из массива по индексу\n"
			 << " 4. Сгенегировать 10 случайных чисел\n"
			 << " 5. Вывести массив на экран\n"
			 << " 6. Очистить массив\n"
			 << " 7. Сложить два числа из массива\n"
			 << " 8. Отнять два числа из массива\n"
			 << " 9. Умножить два числа из массива\n"
			 << "10. Поделить два числа из массива\n"
			 << "11. Поделить число из массива на маленькое число\n"
			 << "12. Переместить часть числа в другое число на той же позиции\n"
			 << " 0. Выйти\n>>> ";
		getline(cin, answer);
		if (is_in(answer, show_arr, show_arr_size)) cout << arr;
		if (answer == "0") exit(0);
		else if (answer == "1") run_tests();
		else if (answer == "2") {
			getline(cin, buf);
			arr.push_back(buf);
		}
		else if (answer == "3") {
			getline(cin, buf);
			tmp0 = atoll(buf.c_str());
			if (tmp0 < arr.size())
				arr.erase(arr.begin() + tmp0);
			else cout << "Неправильный индекс\n";
		}
		else if (answer == "4") {
			for (int i = 0; i < 10; i++) {
				stringstream ss;
				if (rand() % 2) ss << "-";
				tmp0 = rand() % 128;
				for (int j = 0; j < tmp0; j++)
					ss << abs(rand() % 10);
				arr.push_back(ss.str());
			}
			cout << arr;
		}
		else if (answer == "5") {}
		else if (answer == "6") arr.clear();
		else if (answer == "7" || answer == "8"
				|| answer == "9" || answer == "10") {
			cout << "Введите индексы:\n>>> ";
			cin >> tmp0 >> tmp1;
			getline(cin, buf);
			if (tmp0 >= arr.size() || tmp1 >= arr.size())
				cout << "Неправильные индексы\n";
			else if (answer == "7")
				cout << arr[tmp0] + arr[tmp1] << endl;
			else if (answer == "8")
				cout << arr[tmp0] - arr[tmp1] << endl;
			else if (answer == "9")
				cout << arr[tmp0] * arr[tmp1] << endl;
			else if (answer == "10")
				cout << arr[tmp0] / arr[tmp1] << endl;
		} else if (answer == "11") {
			cout << "Введите индекс:\n>>> ";
			cin >> tmp0;
			getline(cin, buf);
			cout << "Введите число:\n>>> ";
			cin >> tmp1;
			getline(cin, buf);
			cout << arr[tmp0] / tmp1 << endl;
		} else if (answer == "12") {
			cout << "Введите индексы:\n>>> ";
			cin >> tmp0 >> tmp1;
			getline(cin, buf);
			if (tmp0 >= arr.size() || tmp1 >= arr.size())
				cout << "Неправильные индексы\n";
			else {
				cout << "Введите диапазон:\n>>> ";
				cin >> tmp2 >> tmp3;
				getline(cin, buf);
				cout << (arr[tmp0](tmp2, tmp3) = arr[tmp1]) << endl;
			}
		}
		else cout << "Нет такой опции\n";
		cout << "Нажмите ENTER для продолжения... ";
		getline(cin, buf);
	}
	cout << "\n\n\n";
}
