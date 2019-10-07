#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <wctype.h>
#include <limits.h>
#define CHUNK 128

int ispartofword(wchar_t c) {
	return iswalpha(c) || iswalnum(c) || c == '\'';
}

int find_word_number(wchar_t * string, int word_num) {
	int x = 0,i = 0;
	if (ispartofword(string[0])) x = 1;
	for (i = 1; string[i] && x < word_num; i++)
		if (!ispartofword(string[i-1]) && ispartofword(string[i]))
			x++;
	if (x == word_num)
		return i;
	else
		return -1;
}

void ign_other(FILE * input) {
	wchar_t c = 0;
	while ((c = fgetwc(input)) != EOF && c != '\n') {}
}

wchar_t * get_string (FILE * input, int * status) {
	wchar_t * a = (wchar_t *) calloc(CHUNK,sizeof(wchar_t)), ch = EOF;
	int length = 0;
	while (ch) {
		ch = fgetwc(input);
		if (length >= INT_MAX - 2 * CHUNK || ch == EOF || ch == '\n') {
			if (ch == EOF)
				*status = 2;
			else if (ch == '\n')
				*status = 3;
			else if (length >= INT_MAX - 2 * CHUNK)
				*status = 1;
			ch = 0;
		}
		if (length % CHUNK == CHUNK - 1)
			a = (wchar_t *) realloc (a, (length + CHUNK + 1) * sizeof(wchar_t));
		a[length++] = ch;
	}
	return a;
}

int main() {
	setlocale(LC_ALL, "");
	int word_num = 0, found = -1, status = 0,i,mw = 0;
	wchar_t * a;
	while (status < 2) {
		wprintf(L"Введите строку: ");
		a = get_string(stdin,&status);
		if (status == 1) {
			system("clear");
			wprintf(L"Слишком длинная строчка, максимальная длина строчки %i. Попробуйте ещё.\n", INT_MAX - 2 * CHUNK);
			ign_other(stdin);
		} else if (status > 1) {
			if (ispartofword(a[0])) mw++;
			if (a[0])
				for (i=1; a[i]; i++)
					if (!ispartofword(a[i-1]) && ispartofword(a[i]))
						mw++;
			if (mw == 0) {
				status = 0;
				system("clear");
				wprintf(L"Эта строчка пустая\n",INT_MAX);
			}
		}
		if (status < 2) free(a);
	}
	while (found == -1) {
		wprintf(L"Введите номер слова: ");
		wscanf(L"%i",&word_num);
			ign_other(stdin);
		if (word_num <= mw)
			found = find_word_number(a,word_num);
		if (found == -1) {
			system("clear");
			wprintf(L"Нет столько слов сколько вы просите. Максимальное количество слов: %i.\n",mw);
		}
	}
	wprintf(L"Слово находиться на %i позиции\n",found);
	return 0;
}
