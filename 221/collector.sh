#!/bin/bash

SOURCE_CODE="main.c"
if [ ! -z "$1" ]
then
	SOURCE_CODE=$1
fi
OUTPUT_FILE="statistics.txt"
if [ ! -z "$2" ]
then
	OUTPUT_FILE=$2
fi
BEGIN="1000"
if [ ! -z "$3" ]
then
	BEGIN=$3
fi
STEP="1000"
if [ ! -z "$4" ]
then
	STEP=$4
fi

rm $OUTPUT_FILE
echo "Size,Time,Memory" > $OUTPUT_FILE
gcc -g $SOURCE_CODE
read -r -a MEM <<< "$(free -m | tr -s " " | grep Mem)"
FREE_MEM=`echo "(${MEM[1]}*85/100-${MEM[2]}-${MEM[4]})" | bc`
USED_MEMORY=0
i=$BEGIN
while [ $USED_MEMORY -le $FREE_MEM ]
do
    USED_MEMORY=`echo "break 25
run $i >> $OUTPUT_FILE
call malloc_stats()
continue
quit" | gdb a.out 2>&1 | grep "system bytes" | head -2  | tail -1 | tr -s " " | cut -d " " -f 4`
    echo $USED_MEMORY
    USED_MEMORY=`echo $USED_MEMORY/1024/1024 | bc`
    sed -i '${s/$/,'$USED_MEMORY'/}' $OUTPUT_FILE
    read -r -a MEM <<< "$(free -m | tr -s " " | grep Mem)"
    FREE_MEM=`echo "(${MEM[1]}*85/100-${MEM[2]}-${MEM[4]})" | bc`
    i=$((i+STEP))
done
rm a.out

exit 0
