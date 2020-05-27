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
