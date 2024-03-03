#!/usr/bin/env bash
#set -x

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
		#echo "less than two input(err 8)"
		print_help
		exit 1
	fi

	for arg in "${@: -2}"; do
		if [ ! -e "$arg" ]; then
			#echo "$arg"
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
	#echo "accepted"
}

cmp_sym() {
	if [[ "$lopt" == "true" ]]; then
		if [[ -L "$1" ]]; then
			tar1=$(readlink "$1")
		fi
		if [[ -L "$2" ]]; then
			tar2=$(readlink "$2")
		fi
		if [[ ( ! -L "$1" ) || ( ! -L "$2" ) || "$tar1" != "$tar2" ]]; then
			echo "changed 100%"
		else
			echo "changed 0%"
		fi
	else
		if [[ -L "$1" ]]; then
			if [[ -L "$2" ]]; then
				echo "changed 0%"
			else
				echo "create" # $2
			fi
		elif [[ -L "$2" ]]; then
			echo "delete"	# $1
		fi
	fi
}

cmp_file() {
	if [[ "$lopt" == "true" ]]; then
		if [[  -L "$1" || -L "$2" ]]; then
			local result=$(cmp_sym "$1" "$2")
			if [[ $result != "changed 0%" ]]; then
				if [[ "$result" == "delete" ]]; then
					echo "$1 delete"
				elif [[ "$result" == "create" ]]; then
					echo "$2 create"
				else
					echo "$result"
				fi
			fi
			return
		fi
	fi

	local diffo_output=$(diff -d "$1" "$2")
	local diff_output=$(diff -d -u "$1" "$2" | grep -vE '^(---|\+\+\+|@@)' 2>&1)
	local result=$(echo "$diffo_output")
	local x
	if [[ $result == *"Binary files"* ]]; then 
		x=100
	else
		local a=$(echo "$diff_output" | grep -c "^-" )
		local b=$(echo "$diff_output" | grep -c "^+" )
		# local c=$(echo "$diff_output" | egrep -c "^ ")
		#local tota=$(wc -l < "$1");
		local tota=$(grep -a "^" "$1" | wc -l)
		#echo $diff_output
		#echo "$a:$b:$c"
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

	if [[ "$lopt" == "true" ]]; then
		local symlinks=$(find "$1"/* -type l)
		for symlink1 in $symlinks; do
			#echo "$(basename "${symlink1#$1/}"): "$exp" "
			if [[ ( "$(basename "${symlink1#$1/}")" =~ "$exp" && "$nopt" == "true" ) || ( "$nopt" == "false" ) ]]; then
				if [ -L "$symlink1" ]; then
					symlink2="$2/$(basename "$symlink1")"
					#echo "$symlink1:$symlink2"
					if [[ -e "$symlink2" ]]; then
						local result=$(cmp_sym "$symlink1" "$symlink2")
						if [[ $result != "changed 0%" ]]; then
							local now=$(basename "$symlink1");
							local now2=$(basename "$symlink2");

							if [[ "$result" == "delete" ]]; then
								file_status+=("$now delete")
							elif [[ "$result" == "create" ]]; then
								file_status+=("$now2 create")
							else
								file_status+=("$now: $result")
							fi
						fi
					else 
						file_status+=("${symlink1#$1/} delete");
					fi
				fi
			fi
		done
		symlinks=$(find "$2"/* -type l)
		for symlink2 in $symlinks; do
			if [[ ( "$(basename "${symlink2#$2/}")" =~ "$exp" && "$nopt" == "true" ) || ( "$nopt" == "false" ) ]]; then
				if [ -L "$symlink2" ]; then
					symlink1="$1/$(basename "$symlink2")"
					if [[ ! -e "$symlink1" ]]; then
						file_status+=("${symlink2#$2/} create")
					fi
				fi
			fi
		done
	fi

	while IFS= read -r -d '' file1; do
		rel_path="${file1#$1/}"
		file2="$2/$rel_path"
		path_to_file="$(dirname $file1)"
		if [[ "./$rel_path" == *"/."* ]]; then
			areal="true" #hidden
		else
			areal="false" #not hidden
		fi
		# echo "---" $rel_path
#		while [[ "$path_to_file" != "/" ]]; do
#			if [[ "$(basename "$path_to_file")" == .* ]]; then
#				areal="true"
#				break
#			fi
#			path_to_file="$(dirname "$path_to_file")"
#		done
		# echo "$areal: $file1 / "$(basename "${file1#$1/}")""
		if [[ ( ( "$aopt" == "false" && "$areal" == "false") || ( "$aopt" == "true" ) ) && ( ( "$(basename "${file1#$1/}")" =~ "$exp" && "$nopt" == "true" ) || ( "$nopt" == "false" ) ) ]]; then
			if [[ -L "$file1" || -L "$file2" ]]; then
				local result=$(cmp_sym "$file1" "$file2")
				if [[ $result != "changed 0%" ]]; then
					if [[ "$result" == "delete" ]]; then
						file_status+=("$rel_path delete")
					elif [[ "$result" == "create" ]]; then
						rel_path2="${file2#$1/}"
						file_status+=("$rel_path2 create")
					else
						file_status+=("$rel_path: $result")
					fi
				fi
			elif [[ ! -e "$file2" || "$(basename "${file2#$2/}")" == .* || -L "$file2" ]]; then
				file_status+=("$rel_path delete")
			else
				local result=$(cmp_file "$file1" "$file2")
				if [[ $result != "changed 0%" ]]; then
					file_status+=("$rel_path: $result");
				fi
			fi
		fi
	done < <(find "$1" -type f -print0)

	while IFS= read -r -d '' file2; do
		rel_path="${file2#$2/}"
		file1="$1/$rel_path"
		if [[ ( ( "$aopt" == "false" && "$file2" != .* ) || ( "$aopt" == "true" ) ) && ( ( "$(basename "${file1#$2/}")" =~ "$exp" && "$nopt" == "true" ) || ( "$nopt" == "false" ) ) ]]; then
			if [[ ! -e "$file1" || "$(basename "${file1#$1/}")" == .* || -L "$file1" ]]; then
					file_status+=("$rel_path create")
				fi
			fi
	done < <(find "$2" -type f -print0)
	
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

if [[ "$ropt" == "true" && ( -d "$path_a" && -d "$path_b") ]]; then 
	cmp_dir "$path_a" "$path_b"
elif diff -qr "$path_a" "$path_b" >/dev/null; then
	:
else
	cmp_file "$path_a" "$path_b"
fi

