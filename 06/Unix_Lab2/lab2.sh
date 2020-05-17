#!/bin/sh

DIRECTORY="$1"

[ ! -d "$DIRECTORY" ] && echo "No such directory: \"$DIRECTORY\"" && exit 1
INDEX="$DIRECTORY/index"

if [ -f "$INDEX" ]; then "$INDEX"
elif [ -f "$INDEX.html" ]; then gtkmoz "$INDEX.html"
elif [ -f "$INDEX.htm" ]; then bareview "$INDEX.htm"
else
    FILE=`Xdialog --stdout --title "Виберіть файл" --fselect "$DIRECTORY" 50 150`
    [ -f $FILE ] && $FILE
fi
exit $?
