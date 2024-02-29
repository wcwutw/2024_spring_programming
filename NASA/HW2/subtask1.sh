#!/usr/bin/env bash

print_help() {
	echo "usage: ./compare.sh [OPTION] <PATH A> <PATH B>"
	echo "options:"
	echo "-a: compare hidden files instead of ignoring them"
	echo "-h: output information about compare.sh"
	echo "-l: treat symlinks as files instead of ignoring them"
	echo "-n <EXP>: compare only files whose paths follow the REGEX <EXP>"
	echo "-r: compare directories recursively"
}

size="$#"

if [[ "$size" -lt 2 ]]; then
	echo "less than two input(err 8)"
	print_help
	exit 1
fi

for arg in "${@: -2}"; do
	if [ ! -d "$arg" ] && [ ! -f "$arg" ] && [ ! -L "$arg" ]; then
		echo "last two isn't eligible path(err 7 or err 8)"
		print_help
		exit 1
	fi
done

ropt=false
aopt=false
nopt=false
lopt=false
exp=""
path_a=""
path_b=""

while getopts ":ahlrn:?" opt; do
	case $opt in
		a)
			aopt=true
			;;
		h) #done
			echo "input -h(err 1)"
			print_help
			exit 1
			;;
		l)
			lopt=true
			;;
		n)
			nopt=true
			if [[ "$OPTARG" =~ $exp ]]; then
				exp="$OPTARG"
			else
				echo "regex didn't come after -n(err 3)"
				print_help
				exit 1
			fi
			;;
		r)
			ropt=true
			;;
		\?) #done
			echo "parameter not found(err 2)"
			print_help
			exit 1
			;;
	esac
done
shift $((OPTIND -1))

size="$#"

#if [[ -n "$regex" ]]; then
#	echo "regex didn't come after -n(err 3)"
#	print_help
#	exit 1
#fi


if [[ "$size" -ne 2 ]]; then
	echo "read something useless(err 9)"
	print_help
	exit 1
fi

path_a="$1"
path_b="$2"

if [[ "$ropt" == "true" && (! -d "$path_a" || ! -d "$path_b") ]]; then
	echo "-r but not both directory(err 5)"
	print_help
	exit 1
fi

if [[ "$ropt" == "false" && ( -d "$path_a" || -d "$path_b") ]]; then
	echo "not -r but contain directory(err 6)"
	print_help
	exit 1
fi

if [[ ("$aopt" == "true" || "$nopt" == "true") && "$ropt" == "false" ]]; then
	echo "-a or -n but didn't come with -r(err 4)"
	print_help
	exit 1
fi

echo "accepted"
