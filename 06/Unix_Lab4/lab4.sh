#!/bin/sh

[ -z "$1" ] || [ -z "$2" ] && { echo "Немає потрібних аргументів"; exit 1; }

OUTPUT_FILE="${@:$#}"
FILES="${@:1:$(($#-1))}"

find $FILES >/dev/null || { echo "Файли перелічені вище не існують"; exit 1; }

sed -E "s/<[^>]*?>//g" $FILES >$OUTPUT_FILE
exit $?
