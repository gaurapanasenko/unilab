#!/bin/sh

TEMPLATE="<vbox><text><label>MESSAGE</label></text><button ok></button></vbox>"

[ -z "$1" ] || [ -z "$2" ] && {
    MAIN_DIALOG=${TEMPLATE/MESSAGE/Немає потрібних аргументів} gtkdialog
    exit 1
}

GROUP="$1"
FILE="$GROUP.txt"
QUERY="$2"

if [ -f "$FILE" ]
then
    STUDENT=`grep "$QUERY" "$FILE" | head -n 1`
    if [ -z $STUDENT ]; then MAIN_DIALOG=${TEMPLATE/MESSAGE/Студент(ка) по запиту \"$QUERY\" не знайдений у группі $GROUP} gtkdialog
    else MAIN_DIALOG=${TEMPLATE/MESSAGE/Студент(ка) $STUDENT належить группі $GROUP} gtkdialog
    fi
else MAIN_DIALOG=${TEMPLATE/MESSAGE/Немає такої группи $GROUP} gtkdialog
fi
