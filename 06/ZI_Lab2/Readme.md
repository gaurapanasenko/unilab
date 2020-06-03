# Звіт з лабораторної роботи №1<br/>за дисципліною "Захист інформації"<br/>студента групи ПА-17-2<br/>Панасенка Єгора Сергійовича<br/>Кафедра комп'ютерних технологій, фпм, дну <br/>2018-2019 навч.р.

## Постановка задачі

Реалізувати 2 простих методи шифрування.

## Код роботи

### Файл `caesar.py`

```python
#!/usr/bin/python

import sys

ALPHABET = ([chr(ord('а') + i) for i in range(0, 32)]
            + [chr(ord('a') + i) for i in range(0, 26)] + [" "])

ENCRYPT = 2
DECRYPT = 4


def main():
    do = 0
    for arg in sys.argv[1:]:
        if arg == "--encrypt":
            do = do | ENCRYPT
        if arg == "--decrypt":
            do = do | DECRYPT
    print("Введите ключевое строку: ")
    keyword = sys.stdin.readline()[:-1]
    print("Введите исходную строку: ")
    string = sys.stdin.readline()[:-1]
    for i in range(len(keyword)):
        if keyword[i] not in ALPHABET:
            print("Ошибка: такого символа нет в алфавите")
            exit(1)
        for j in range(i+1, len(keyword)):
            if keyword[i] == keyword[j]:
                print("Ошибка: ключевое слово имеет повторяющиеся символы")
                exit(1)
    alphabet = list(keyword) + [i for i in ALPHABET if i not in keyword]
    if do & ENCRYPT:
        outstring = ""
        for i in string:
            ind = alphabet.index(i)
            outstring += alphabet[(alphabet.index(i) + 5) % len(alphabet)]
        print("Зашифрованный вариант %s" % outstring)
    if do & DECRYPT:
        outstring = ""
        for i in string:
            length = len(alphabet)
            outstring += alphabet[(length + alphabet.index(i) - 5) % length]
        print("Расшифрованный вариант %s" % outstring)


if __name__ == "__main__":
    main()
```

### Файл `vigener.py`

```python
#!/usr/bin/python

import sys

ALPHABET = ([chr(ord('а') + i) for i in range(0, 32)]
            + [chr(ord('a') + i) for i in range(0, 26)] + [" "])

ENCRYPT = 2
DECRYPT = 4


def main():
    """Main function."""
    dothing = 0
    for arg in sys.argv[1:]:
        if arg == "--encrypt":
            dothing = dothing | ENCRYPT
        if arg == "--decrypt":
            dothing = dothing | DECRYPT
    print("Введите ключевое строку: ")
    raw_keyword = sys.stdin.readline()[:-1]
    print("Введите исходную строку: ")
    string = sys.stdin.readline()[:-1]
    keyword = []
    for i, char in enumerate(string):
        kind = i % len(raw_keyword)
        if string[i] not in ALPHABET or raw_keyword[kind] not in ALPHABET:
            print("Ошибка: такого символа нет в алфавите")
            sys.exit(1)
        for j in range(i+1, len(keyword)):
            if keyword[i] == keyword[j]:
                print("Ошибка: ключевое слово имеет повторяющиеся символы")
                sys.exit(1)
        keyword.append(raw_keyword[i % len(raw_keyword)])
    if dothing & ENCRYPT:
        outstring = ""
        for i, char in enumerate(string):
            key_index = ALPHABET.index(keyword[i])
            str_index = ALPHABET.index(char)
            outstring += ALPHABET[(key_index + str_index) % len(ALPHABET)]
        print("Зашифрованный вариант %s" % outstring)
    if dothing & DECRYPT:
        outstring = ""
        for i, char in enumerate(string):
            length = len(ALPHABET)
            key_index = ALPHABET.index(keyword[i])
            str_index = ALPHABET.index(char)
            outstring += ALPHABET[(length + length + str_index - key_index) % length]
        print("Расшифрованный вариант %s" % outstring)


if __name__ == "__main__":
    main()
```
