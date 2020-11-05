#!/bin/bash

N=8
RUNS=5

i=1

echo "threadsnbr,run1,run2,run3,run4,run5"

while  [ $i -le $N ]; do
	printf "%d," $i
	j=1
	while [ $j -le $RUNS ]; do

		make clean -s 2> /dev/null
		/usr/bin/time make -j $i -is 2>&1 | grep real | awk '{printf "%s", $1}'

		if [ $j == $RUNS ]
		then
			printf "\n"
		else
			printf ","
		fi

		((j++))

	done
	((i++))
done

make clean -s 2> /dev/null
