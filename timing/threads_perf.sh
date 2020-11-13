#!/bin/bash

if [ $# != 2 ]; then
        echo "usage: ./threadstime.sh [PROGRAM_NAME] [NUMBER_OF_THREADS]" > /dev/stderr
        exit 1
fi

N=$2
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

		/usr/bin/time ./$1 $j | grep real | awk '{printf "%s", $1}'

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
