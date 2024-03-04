#!/bin/bash
#set -x
export LC_ALL="en_US.UTF-8"

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
fe=true
exp=""
path_a=""
path_b=""

check() {
	local size="$#"
	if [[ "$size" -lt 2 ]]; then
		#echo "less than two input(err 8)"
		print_help
		exit 1
	fi

	for arg in "${@: -2}"; do
		if [ ! -e "$arg" ]; then
			#echo "<file A> or <file B> not found in the directory"
			print_help
			exit 1
		fi

		if [ ! -d "$arg" ] && [ ! -f "$arg" ] && [ ! -L "$arg" ]; then
			#echo "last two isn't eligible path(err 7 or err 8)"
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
				#echo "input -h(err 1)"
				print_help
				exit 1
				;;
			l)
				lopt=true
				;;
			n)
				nopt=true
				if [[ "$OPTARG" =~ "$exp" ]]; then
					exp="$OPTARG"
				else
					#echo "regex didn't come after -n(err 3)"
					print_help
					exit 1
				fi
				;;
			r)
				ropt=true
				;;
			\?)
				#echo "parameter not found(err 2)"
				print_help
				exit 1
				;;
		esac
	done

	shift $((OPTIND -1))
	size="$#"

	if [[ "$size" -ne 2 ]]; then
		#echo "read something useless(err 9)"
		print_help
		exit 1
	fi

	path_a="$1"
	path_b="$2"
	if [[ "$ropt" == "false" && ( "$nopt" == "true" || "$aopt" == "true" ) ]]; then
		print_help
		exit 1
	fi
	if [[ "$ropt" == "true" && (! -d "$path_a" || ! -d "$path_b") ]]; then
		#echo "-r but not both directory(err 5)"
		print_help
		exit 1
	fi
	
	if [[ "$ropt" == "false" && ( -d "$path_a" || -d "$path_b") ]]; then
		#echo "not -r but contain directory(err 6)"
		print_help
		exit 1
	fi
	
	if [[ "$lopt" == "false" && ( -L "$path_a" || -L "$path_b") ]]; then
		#echo "contain symb link but don't have -l"
		print_help
		exit 1
	fi
}

check_exist() {
	fe=true
	if [[ -h "$1" && "$lopt" == "false" ]]; then
		fe=false
	fi
	if echo "$2" | grep -qE '/\.[^/]*'; then
		if [[ "$aopt" == "false" ]]; then
			fe=false
		fi
	fi
	if [[ ! -e "$1" ]]; then
		fe=false
	fi
	if [[ -d "$1" && ( ! -h "$1" ) ]]; then
		fe=false
	fi
	if [[ ! "$2" =~ "$exp" ]] && [[ "$nopt" == "true" ]]; then
		fe=false
	fi
}

cmp_file() {
	local diffo_output=$(diff -d --no-dereference "$1" "$2")
	local diff_output=$(diff -d -u --no-dereference "$1" "$2" | grep -vE '^(---|\+\+\+|@@)' 2>&1)
	local result=$(echo "$diffo_output")
	local x
	if [[ ! -n "$result" ]]; then
		return
	fi
	#echo "$result"
	if [[ $result == *"Binary files"* || $result == *"二元碼檔"* || $result == *"symbolic link"* || $result == *"Symbolic link"* || $result == *"符號連結"* ]]; then 
		x=100
	else
		local a=$(echo "$diff_output" | grep -c "^-" )
		local b=$(echo "$diff_output" | grep -c "^+" )
		#local c=$(echo "$diff_output" | egrep -c "^ ")
		#local tota=$(wc -l < "$1");
		local tota=$(grep -a "^" "$1" | wc -l)
		#echo $diff_output
		local c=$((tota-a))
		local max=$((b > a ? b : a))
		if [[ $max == 0 && $c == 0 ]]; then
			x=0
		else
			x=$((100 * max / (max + c)))
		fi
	fi

	echo "changed ${x}%"
}

cmp_dir() {
	declare -a file_status=()
	
	acom=""
	lcom="-type f"
	if [[ "$aopt" == "false" ]]; then
		acom="-not -path '*/\.*'"
	fi
	if [[ "$lopt" == "true" ]]; then
		lcom="\( -type f -o -type l \)"
	fi
	ec1="cd $1 && find ./ $lcom $acom"
	ec2="cd $2 && find ./ $lcom $acom"
	en1=$(eval "$ec1")
	en2=$(eval "$ec2")
	
	while IFS= read -r file1; do
		rel_path="${file1#\./}"
		fa="$1/""$rel_path"
		fb="$2/""$rel_path"
		check_exist "$fb" "$rel_path"
		if [[ ! -n $file1 || ( "$nopt" == "true" && ! $rel_path =~ "$exp" ) ]]; then
			continue
		fi
		if [[ "$fe" == "true" ]]; then
			result="$(cmp_file "$fa" "$fb")"
			if [[ -n "$result" ]]; then
				file_status+=("$rel_path! $result")
			fi
		else
			file_status+=("$rel_path delete")
		fi
	done < <(echo "$en1")
	
	while IFS= read -r file2; do
		rel_path="${file2#\./}"
		fa="$1/""$rel_path"
		fb="$2/""$rel_path"
		check_exist "$fa" "$rel_path"
		if [[ ! -n $file2 || ( "$nopt" == "true" && ! $rel_path =~ "$exp" ) ]]; then
			continue
		fi
		if [[ "$fe" == "false" ]]; then
			file_status+=("$rel_path create")
		fi
	done < <(echo "$en2")
	
	LC_ALL=C
	IFS=$'\n' sorted_status=($(sort <<<"${file_status[*]}"))

	for status in "${sorted_status[@]}"; do
		case $status in
			*create)
				echo "create ${status% create}"
				;;
			*delete)
				echo "delete ${status% delete}"
				;;
			*)
				outp="$(echo "${status}" | sed 's/!/:/g' )"
				echo "$outp"
				;;
		esac
	done
}

check "$@"

remove_slash() {
	path="$1"
	while [[ "${path: -1}" == '/' ]]; do
		path="${path:0:-1}"
	done
}

remove_slash "$path_a"
path_a="$path"
remove_slash "$path_b"
path_b="$path"

if [[ "$ropt" == "true" && ( -d "$path_a" && -d "$path_b") ]]; then 
	cmp_dir "$path_a" "$path_b"
elif diff -qr "$path_a" "$path_b" >/dev/null; then
	:
else
	cmp_file "$path_a" "$path_b"
fi

