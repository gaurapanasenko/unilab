#!/bin/sh

DIRECTORY="$1"

[ ! -d "$DIRECTORY" ] && echo "No such directory: \"$DIRECTORY\"" && exit 1

if [ -f index ]; then ./index
elif [ -f index.html ]; then gtkmoz index.html
elif [ -f index.htm ]; then bareview index.html
else
    FILE=`Xdialog --stdout --title "Виберіть файл" --fselect "$DIRECTORY" 50 150`
    [ -f $FILE ] && $FILE
fi
