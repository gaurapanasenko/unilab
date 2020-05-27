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
