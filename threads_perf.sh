#!/bin/bash

if [ $# != 3 ]; then
        echo "usage: ./threads_perf.sh [PROGRAM_NAME] [NUMBER_OF_THREADS] [OUTPUT_FILE]" > /dev/stderr
        exit 1
fi

N=$2
OUT=$3
RUNS=5

 > $OUT

for (( c=1; c<=N; c++ ))
do
  printf "%d" $c >> $OUT

	if [ $c == $N ]
	then
		printf "\n" >> $OUT
	else
		printf "," >> $OUT
	fi
done

i=1
while  [ $i -le $RUNS ]; do
	j=1
	while [ $j -le $N ]; do
    x=$((j/2))
    y=$((j/2))

    if [ $((j%2)) -eq 0 ]; then
      ((y++))
    fi

		/usr/bin/time -f %e -o $OUT -a ./$1 $x $y
    truncate -s-1 $OUT

		if [ $j == $N ]
		then
			printf "\n" >> $OUT
		else
			printf "," >> $OUT
		fi
		((j++))
	done
	((i++))
done
