#!/bin/bash

if [ $# != 1 ]; then
        echo "usage: ./threadstime.sh [NUMBER_OF_THREADS]" > /dev/stderr
        exit 1
fi

N=$1
RUNS=5

for (( c=1; c<=N; c++ ))
do
  printf "%d" $c

	if [ $c == $N ]
	then
		printf "\n"
	else
		printf ","
	fi
done

i=1
while  [ $i -le $RUNS ]; do
	j=1
	while [ $j -le $N ]; do

		make clean -s 2> /dev/null
		/usr/bin/time make -j $j -is 2>&1 | grep real | awk '{printf "%s", $1}'

		if [ $j == $N ]
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
