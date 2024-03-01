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

ropt=false
aopt=false
nopt=false
lopt=false
exp=""
path_a=""
path_b=""

check() {
	local size="$#"
	if [[ "$size" -lt 2 ]]; then
		echo "less than two input(err 8)"
		print_help
		exit 1
	fi

	for arg in "${@: -2}"; do
		if [ ! -e "$arg" ]; then
			echo "<file A> or <file B> not found in the directory"
			print_help
			exit 1
		fi

		if [ ! -d "$arg" ] && [ ! -f "$arg" ] && [ ! -L "$arg" ]; then
			echo "last two isn't eligible path(err 7 or err 8)"
			print_help
			exit 1
		fi
	done

	while getopts ":ahlrn:?" opt; do
		case $opt in
			a)
				aopt=true
				;;
			h)
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
			\?)
				echo "parameter not found(err 2)"
				print_help
				exit 1
				;;
		esac
	done

	shift $((OPTIND -1))
	size="$#"

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
	
	if [[ "$lopt" == "false" && ( -L "$path_a" || -L "$path_b") ]]; then
		echo "contain symb link but don't have -l"
		print_help
		exit 1
	fi

	echo "accepted"
}

cmp_file() {
	local diff_output=$(diff -d "$1" "$2" 2>&1)
	local result=$(echo "$diff_output")
	local x
	if [[ $result == *"Binary files"* ]]; then 
		#to be changed
		x=100
	else
		local a=$(echo "$diff_output" | grep -cE "^<[[:space:]]")
		local b=$(echo "$diff_output" | grep -cE "^>[[:space:]]")
		local tota=$(wc -l < "$1");
		local c=$((tota-a))
		local max=$((b > a ? b : a))
		local x=$((100 * max / (max + c)))
	fi
	#return x
	echo "changed ${x}%"
}

cmp_sym() {
	tar1=$(readlink "$1")
	tar2=$(readlink "$2")
	if [ -L "$symlink1" ] && [ ! -L "$symlink2" ]; then
		echo "$changed 100%"
	elif [ ! -L "$symlink1" ] && [ -L "$symlink2" ]; then
		echo "changed 100%"
	else
		if [ "$tar1" != "$tar2" ]; then
			echo "changed 100%"
		else
			echo "changed 0%"
		fi
	fi
}

cmp_dir() {
	declare -a file_status=()
	
	if [[ "$lopt" == "true" ]]; then
		local symlinks=$(find "$1"/* -type l)
		for symlink1 in $symlinks; do
			if [ -L "$symlink1" ]; then
				symlink2="$2/$(basename "$symlink1")"
				if [ -e "$symlink2" ]; then
					local result=$(cmp_sym "$symlink1" "$symlink2")
					if [[ $result != "changed 0%" ]]; then
						local now=$(basename "$symlink1");
						file_status+=("$now: $result");
					fi
				else 
					file_status+=("${symlink1#$1/} delete");
				fi
			fi
		done
		symlinks=$(find "$2"/* -type l)
		for symlink2 in $symlinks; do
			if [ -L "$symlink2" ]; then
				symlink1="$1/$(basename "$symlink2")"
				if [ ! -e "$symlink1" ]; then
					file_status+=("${symlink2#$2/} create");
				fi
			fi
		done
	fi

	if [[ "$aopt" == "false" ]]; then
		while IFS= read -r -d '' file1; do
			rel_path="${file1#$1/}"
			file2="$2/$rel_path"
			if [ ! -e "$file2" ]; then
				file_status+=("$rel_path delete");
			else
				local result=$(cmp_file "$file1" "$file2")
				if [[ $result != "changed 0%" ]]; then
					file_status+=("$rel_path: $result");
				fi
			fi
		done < <(find "$1" -type f -not -path '*/\.*' -print0)
	else
		while IFS= read -r -d '' file1; do
			rel_path="${file1#$1/}"
			file2="$2/$rel_path"
			if [ ! -e "$file2" ]; then
				file_status+=("$rel_path delete");
			else
				local result=$(cmp_file "$file1" "$file2")
				if [[ $result != "changed 0%" ]]; then
					file_status+=("$rel_path: $result");
				fi
			fi
		done < <(find "$1" -type f -print0)
	fi

	if [[ "$aopt" == "false" ]]; then
		while IFS= read -r -d '' file2; do
			rel_path="${file2#$2/}"
			file1="$1/$rel_path"
			if [ ! -e "$file1" ]; then
				file_status+=("$rel_path create")
			fi
		done < <(find "$2" -type f -not -path '*/\.*' -print0)
	else
		while IFS= read -r -d '' file2; do
			rel_path="${file2#$2/}"
			file1="$1/$rel_path"
			if [ ! -e "$file1" ]; then
				file_status+=("$rel_path create")
			fi
		done < <(find "$2" -type f -print0)
	fi
	
	for status in "${file_status[@]}"; do
		echo "$status"
	done
	
	IFS=$'\n' sorted_status=($(printf '%s\n' "${file_status[@]}" | sort -V))

	for status in "${sorted_status[@]}"; do
		case $status in
			*create)
				echo "create ${status% create}"
				;;
			*delete)
				echo "delete ${status% delete}"
				;;
			*)
				echo "$status"
				;;
		esac
	done
}

check "$@"

# issue: is symb link but don't have -l

if [[ "$ropt" == "true" && ( -d "$path_a" && -d "$path_b") ]]; then 
	cmp_dir "$path_a" "$path_b"
elif diff -qr "$path_a" "$path_b" >/dev/null; then
	:
else
	cmp_file "$path_a" "$path_b"
fi

