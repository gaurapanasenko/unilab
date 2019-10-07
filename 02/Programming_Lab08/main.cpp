#include <iostream>
#include <fstream>
#include <string>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

template <class T>
struct element
{
	T value;
	struct element * prev, * next;
};

template <class T>
class List {
	element<T> * head, * last;
	int size;
private:
	void DeleteNode (element<T> * node) {
		if (node == NULL) return;
		if (node->prev == NULL)
			head = node->next;
		else node->prev->next = node->next;
		if (node->next == NULL)
			last = node->prev;
		else node->next->prev = node->prev;
		size--;
		delete node;
	}
	element<T> * FindByKey (int key) {
		if (head == NULL || key < 0 || key >= size)
			return NULL;
		element<T> * node = head;
		for (int i = 0; i < key; i++)
			node = node->next;
		return node;
	}
	element<T> * FindByValue (T value) {
		if (head == NULL)
			return NULL;
		element<T> * node = head;
		while (node != NULL) {
			if (node->value == value) return node;
			node = node->next;
		}
		return NULL;
	}
	void SwapNodes (element<T> * node1, element<T> * node2) {
		if (node1 == NULL || node2 == NULL) return;

		element<T> * node1prev, * node1next, * node2prev, * node2next;

		node1prev = node1->prev;
		node1next = node1->next;
		node2prev = node2->prev;
		node2next = node2->next;

		if (node1prev != NULL)
			node1prev->next = node2;
		else head = node2;

		if (node1next != NULL)
			node1next->prev = node2;
		else last = node2;

		if (node2prev != NULL)
			node2prev->next = node1;
		else head = node1;

		if (node2next != NULL)
			node2next->prev = node1;
		else last = node1;

		node1->prev = node2prev;
		node1->next = node2next;
		node2->prev = node1prev;
		node2->next = node1next;

		if (node1->next == node1)
			node1->next = node2;
		if (node1->prev == node1)
			node1->prev = node2;
		if (node2->next == node2)
			node2->next = node1;
		if (node2->prev == node2)
			node2->prev = node1;
	}
	void SwapNodesByKey (element<T> * node1, element<T> * node2) {
		if (node1 == NULL || node2 == NULL) return;
		T tmp = node1->value;
		node1->value = node2->value;
		node2->value = tmp;
	}
public:
	List () {
		head = NULL;
		last = NULL;
		size = 0;
	}
	~List() {
		Clear();
	}
	void Clear() {
		if (head == NULL)
			return;
		element<T> * iter = head, * next;
		while (iter != NULL) {
			next=iter->next;
			delete iter;
			iter=next;
		}
		head = NULL;
		last = NULL;
		size = 0;
	}
	void Add (T value, int position = 0) {
		element<T> * node =  new element<T>;
		node->value = value;
		node->next = NULL;
		node->prev = NULL;
		int pos = position % (size + 1);
		if (size == 0) {
			head = node;
			last = node;
			size++;
			return;
		}
		element<T> * prev = NULL, * next = NULL;
		if (pos >= 0) {
			next = head;
			for (int i = 0; i < pos; i++) {
				prev = next;
				next = next->next;
			}
		} else {
			prev = last;
			for (int i = pos; i < -1; i++) {
				next = prev;
				prev = prev->prev;
			}
		}
		if (prev != NULL) {
			prev->next = node;
			node->prev = prev;
		}
		else head = node;
		if (next != NULL) {
			next->prev = node;
			node->next = next;
		}
		else last = node;
		size++;
	}
	int Size() {
		return size;
	}
	int Get(int key, T * value) {
		element<T> * node = FindByKey(key);
		if (node == NULL)
			return 0;
		*value = node->value;
		return 1;
	}
	void DeleteByKey (int key) {
		DeleteNode(FindByKey(key));
	}
	void DeleteByValue (T value) {
		DeleteNode(FindByValue(value));
	}
	void print(ostream& out) {
		if (head == NULL) {
			out << "Список пустий\n";
			return;
		} else if (size > 300) {
			out << "Список занадто великий\n";
			return;
		}
		element<T> * iter = head;
		while (iter->next != NULL) {
			out << iter->value << " ";
			iter=iter->next;
		}
		out << iter->value << endl;
	}

	void Swap (int key1, int key2) {
		element<T> * node1, * node2;
		node1 = FindByKey(key1);
		node2 = FindByKey(key2);
		SwapNodes(node1,node2);
	}

	void Sort () {
		if (head == last) return;
		for (element<T> * i = head; i != NULL; i=i->next) {
			for (element<T> * j = i->next; j != NULL; j=j->next) {
				if (i->value>j->value) {
					SwapNodes(i,j);
					element<T> * tmp = i;
					i = j;
					j = tmp;
				}
			}
		}
	}

	void SortByKey () {
		if (head == last) return;
		for (element<T> * i = head; i != NULL; i=i->next)
			for (element<T> * j = i->next; j != NULL; j=j->next)
				if (i->value>j->value)
					SwapNodesByKey(i,j);
	}

	T * getArray () {
		T * array = (T*) calloc(size,sizeof(T));
		element<T> * iter = head;
		for (int i = 0; i < size; i++) {
			array[i] = iter->value;
			iter = iter->next;
		}
		return array;
	}

	void setArray (T * array, int array_size) {
		Clear();
		for (int i = 0; i < array_size; i++) {
			Add(array[i],-1);
		}
	}
};

template <typename T> ostream& operator<<(ostream& out, List<T>& obj)
{
	obj.print(out);
	return out;
}
template <typename T> istream& operator>>(istream& in, List<T>& obj)
{
	std::string::size_type x;
	string line;
	getline(in,line);
	while ((x = line.find(' ')) != std::string::npos) {
		x = line.find(' ');
		if (x != 0)
			obj.Add(atoi(line.substr(0,x).c_str()),-1);
		line.erase(0,x+1);
	}
	obj.Add(atoi(line.c_str()),-1);
	return in;
}

template <class T>
void sort_array (T * array, int size) {
	T tmp;
	for (int i = 0; i < size; i++)
		for (int j = i + 1; j < size; j++)
			if (array[i] > array[j]) {
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;
			}
}

int get_number (istream& in, ostream& out, string message, int min = INT_MIN, int max = INT_MAX) {
	int number = min-1, flag = 0;
	if (min == INT_MIN) flag = 1;
	string line;
	while (number < min || number > max || flag) {
		out << message << ":\n";
		getline(in,line);
		number = atoi(line.c_str());
		if (number < min || number > max) {
			system("clear");
			out << "Вы ввели неправильне число, дозволено тільки ціле число від " << min << " до " << max << "\n";
		}
		if (flag == 1) flag = 0;
	}
	return number;
}

template <int>
int random (int min, int max) {
	return rand() % (max - min + 1) + min;
}

template <class T>
double random (int min, int max) {
	return (double(rand() % (max*10 - min*10 + 1) + min*10))/10;
}

template <class T>
void menu (istream& in, ostream& out, int * state) {
	List<T> list, list1;
	string line;
	int answer = 0, min, max, size;
	T x = 0, y = 0, * array;
	while (answer != 14 && answer != 15) {
		out << "Список має таку кількість елементів: " << list.Size() << endl;
		out << "Поточний список такий:\n";
		out << list;
		out << "Вибeріть дію:\n";
		out << " 1. Додати рядок в кінець списку з клавіатури\n";
		out << " 2. Додати випадкові n елементів в кінець списку\n";
		out << " 3. Додати 50 випадкових елементів від -100 до 100\n";
		out << " 4. Додати елемент в потрібну позицію\n";
		out << " 5. Видалити один елемент зі списку за номером\n";
		out << " 6. Видалити один елемент зі списку за значенням\n";
		out << " 7. Вивести елемент за номером\n";
		out << " 8. Поміняти елементи місцями\n";
		out << " 9. Очищення списку\n";
		out << "10. Відсортувати список заміною вузлів\n";
		out << "11. Відсортувати список заміною значень\n";
		out << "12. Відсортувати список через масив\n";
		out << "13. Порівняти сорування массиву, списку (заміною вузлів та заміною значень)\n";
		out << "14. Перейти у режим " << ((*state == 1)?"double":"int") << endl;
		out << "15. Вихід\n>>> ";
		getline(in,line);
		answer = atoi(line.c_str());
		system("clear");
		switch (answer) {
			case 1:
				out << "Список має таку кількість елементів: " << list.Size() << endl;
				out << "Введіть будь-які числа підряд\n";
				in >> list;
				break;
			case 2:
				x = get_number(in,out,"Введіть кількість",1);
				min = 1; max = 0;
				while (min > max) {
					min = get_number(in,out,"Введіть мінімально можливе число");
					max = get_number(in,out,"Введіть максимально можливе число");
					if (min > max) {
						out << "Неможливий діапазон\n";
					}
				}
				for (int i = 0; i < x; i++) {
					list.Add(random<T>(min,max),-1);
				}
				break;
			case 3:
				x = 50;
				min = -100; max = 100;
				for (int i = 0; i < x; i++) {
					list.Add(random<T>(min,max),-1);
				}
				break;
			case 4:
				out << list;
				x = get_number(in,out,"Введіть число");
				y = get_number(in,out,"Введіть позицію (нуль - рандомна позиція, відємне число - відрахвати з останього, позитивнне число - відрахувати з першого)");
				if (y > 0) y--;
				else if (y == 0) y = rand();
				list.Add(x, y);
				break;
			case 5:
				out << list;
				list.DeleteByKey(get_number(in,out,"Введіть число",1,list.Size()) - 1);
				break;
			case 6:
				out << list;
				list.DeleteByValue(get_number(in,out,"Введіть число"));
				break;
			case 7:
				if (list.Get(get_number(in,out,"Введіть число",1) - 1,&x))
					out << x << endl;
				else out << "Елемент не існує" << endl;
				break;
			case 8:
				if (list.Size() > 0) {
					out << list;
					x = get_number(in,out,"Введіть номер",1,list.Size()) - 1;
					y = get_number(in,out,"Введіть номер",1,list.Size()) - 1;
					list.Swap(x,y);
				}
				break;
			case 9:
				list.Clear();
				break;
			case 10:
				list.Sort();
				break;
			case 11:
				list.SortByKey();
				break;
			case 12:
				array = list.getArray();
				size = list.Size();
				sort_array(array,size);
				list.setArray(array,size);
				free(array);
				break;
			case 13:
				T * array_orig, * array;
				out << "N,Array,List1,List2\n";
				clock_t t1,t2;
				double dur[3];
				for (int n = 500; n <= 20000; n += 500) {
					array_orig = (T *) calloc(n,sizeof(T));
					array = (T *) calloc(n,sizeof(T));
					for (int i = 0; i < n; i++) {
						array_orig[i] = random<T>(-1000,1000);
						array[i] = array_orig[i];
					}
					t1 = clock();
					sort_array(array,n);
					t2 = clock();
					dur[1] = 1.0*(t2-t1)/CLOCKS_PER_SEC;
					list1.setArray(array_orig,n);
					t1 = clock();
					list1.Sort();
					t2 = clock();
					dur[2] = 1.0*(t2-t1)/CLOCKS_PER_SEC;
					list1.setArray(array_orig,n);
					t1 = clock();
					list1.SortByKey();
					t2 = clock();
					dur[3] = 1.0*(t2-t1)/CLOCKS_PER_SEC;
					out << n << "," << fixed << dur[1] << ","
						<< fixed << dur[2] << ","
						<< fixed  << dur[3] << endl;
					list1.Clear();
					free(array_orig);
					free(array);
				}
				break;
			case 14:
				*state = (*state == 1) ? 2 : 1;
				break;
			case 15:
				*state = 0;
				break;
			default:
				out << "Неправильне значення, треба ввести число від 1 до 11\n";
		}
	}
	list.Clear();
	list1.Clear();
}

int main() {
	srand(time(NULL));
	int state = 1;
	while (state != 0) {
		if (state == 1)
			menu<int>(cin, cout, &state);
		else menu<double>(cin, cout, &state);
	}
	return 0;
}
