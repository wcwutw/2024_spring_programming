#!/usr/bin/env bash
# $1=gen.c $2=a.c $3=b.c $4=time

gcc -o gen "$1"
gcc -o a "$2"
gcc -o b "$3"
time="$4"

for ((i=1; i<=$time; i++)) do
	./gen $i > in.txt
	output_a=$(./a < in.txt)
	output_b=$(./b < in.txt)
	if [ "$output_a" != "$output_b" ]; then
		echo "Test $i:"
		echo "Input:"
		echo "--------------------"
		cat in.txt
		echo "--------------------"
		echo "Output of sol.c:"
		echo "--------------------"
		echo "$output_a"
		echo "--------------------"
		echo "Output of wa.c:"
		echo "--------------------"
		echo "$output_b"
		echo "--------------------"
		exit 1
	fi
done
