#!/usr/bin/env bash
# $1=gen.c $2=a.c $3=b.c $4=time


gcc -o gen "$1"
gcc -o a "$2"
gcc -o b "$3"
time="$4"

for ((i=1; i<=$time; i++)) do
	#in=$(./gen $i)
	#output_a=$(echo "$in" | ./a)
	#output_b=$(echo "$in" | ./b)
	./gen $i > in.txt
	./a < in.txt > outputa.txt
	./b < in.txt > outputb.txt
	output_a=$(./a < in.txt)
	output_b=$(./b < in.txt)
	if diff outputa.txt outputb.txt >useless.txt; then
		:
	else
		echo "Test $i:"
		echo "Input:"
		echo "--------------------"
		#echo "$in"
		cat in.txt
		echo "--------------------"
		echo "Output of $2:"
		echo "--------------------"
		cat outputa.txt
		#echo "$output_a"
		echo "--------------------"
		echo "Output of $3:"
		echo "--------------------"
		cat outputb.txt
		#echo "$output_b"
		echo "--------------------"
		exit 1
	fi
done
