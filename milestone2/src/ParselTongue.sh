#!/bin/bash

input_files=()
output_files=()
output_files_without_extention=()
verbose_flag=0
dot_flag=0
ast_flag=0
ptree_flag=0
csv_flag=1
output_flag=0

display_help() {
	echo ""
	echo -e "\e[38;5;208mUsage: $0 [options] [-i input_files...] [-o output_files...]"
	echo -e "Options:"
	echo -e "	-v,	--verbose			: Output a .debug file at the output folder"
	echo -e "	-i,	--input				: Specify the Input PYTHON file(s), any files specified after this option are input files till -o is specified"
	echo -e "	-o,	--output			: Specify the Output .tac file(s), any files specified after this option are output files till -i is specified"
	echo -e "	-d,	--dot				: Output a .DOT file at the output folder"
	echo -e "	-a,	--ast				: Output an AST pdf file at the output folder"
	echo -e "	-p,	--ptree				: Output a Parse Tree pdf file at the output folder"
	echo -e "	-t,	-table				: Output a CSV file containing Symbol Tables at the output folder"
	echo -e "	-h,	--help				: Display this help message\e[0m"
	echo ""
}

file_exists() {
	if [ -f "$1" ]; then
		input_files+=("$1")
	elif [ -f "../tests/$1" ]; then
		echo -e "\e[33mWarning: File '$1' not found in specified path but found in ../tests/$1\e[0m"
		input_files+=("../tests/$1")
	else
		echo -e "\e[31mError: File '$1' not found in specified path nor in ../tests/$1\e[0m"
		exit 1
	fi
}

create_directory_if_not_exists() {
	if [ ! -d "$1" ]; then
		mkdir -p "$1"
	fi
}

for ((i=1; i<=$#; i++)); do
	arg="${!i}" 
	case "$arg" in
	--input )
		output_flag=0
		;;
	--output )
		output_flag=1
		;;
	--help )
		display_help
		exit 0
		;;
	--verbose )
		verbose_flag=1
		;;
	--dot )
		dot_flag=1
		;;
	--ast )
		ast_flag=1
		;;
	--ptree )
		ptree_flag=1
		;;
	--csv )
		csv_flag=1
		;;
	--* )
		echo -e "\e[31mUnknown flag: -${arg:j:1}\e[0m"
		exit 1
		;;
	-* )
		for ((j=1; j<${#arg}; j++)); do
			case "${arg:j:1}" in
			v)
				verbose_flag=1
				;;
			i)
				output_flag=0
				;;
			o)
				output_flag=1
				;;
			h)
				display_help
				exit 0
				;;
			d)
				dot_flag=1
				;;
			a)
				ast_flag=1
				;;
			p)
				ptree_flag=1
				;;
			c)
				csv_flag=1
				;;
			*)
				echo -e "\e[31mUnknown flag: -${arg:j:1}\e[0m"
				exit 1
				;;
			esac
		done
		;;
	* )
		if [ "$output_flag" -eq 1 ]; then
			output_directory=$(dirname "$arg")
			create_directory_if_not_exists "$output_directory"
			filename=$(basename "$arg")
			filename_no_extension="${filename%.*}"
			output_files_without_extention+=("${output_directory}/${filename_no_extension}")
			output_files+="${arg}"
		else
			file_exists "$arg"
		fi
	;;
	esac
done

if ! make -q; then
	make
	if [ $? -ne 0 ]; then
		exit 1
	fi
fi

for ((i = 0; i < ${#input_files[@]}; i++)); do
	if [ "$i" -lt "${#output_files[@]}" ]; then
		output_file="${output_files[$i]}"
	else
		filename=$(basename "$input_files[$i]")
		filename_no_extension="${filename%.*}"
		output_file="../output/${filename_no_extension}.tac"
	fi
	output_directory=$(dirname "$output_file")
	create_directory_if_not_exists "$output_directory"
	filename=$(basename "$output_file")
	filename_no_extension="${filename%.*}"
	output_file_without_extention="${output_directory}/${filename_no_extension}"
	output_files_without_extention+=("$output_file_without_extention")
done

if [ "${#input_files[@]}" -eq "${#output_files[@]}" ]; then
	for ((i = 0; i < ${#input_files[@]}; i++)); do
		input_file="${input_files[$i]}"
		output_file1="${output_files[$i]}"
		output_file2="${output_files_without_extention[$i]}"

		echo ""
		./compiler "$input_file" "$output_file1" "$output_file2" "${verbose_flag}" "${ast_flag}" "${ptree_flag}" "${csv_flag}"
	done
elif [ "${#input_files[@]}" -lt "${#output_files[@]}" ]; then
	echo -e "\e[33mWarning: Less input files provided than output files. The additional output files wont be used.\e[0m"

	for ((i = 0; i < ${#input_files[@]}; i++)); do
		input_file="${input_files[$i]}"
		output_file1="${output_files[$i]}"
		output_file2="${output_files_without_extention[$i]}"

		echo ""
		./compiler "$input_file" "$output_file1" "$output_file2" "${verbose_flag}" "${ast_flag}" "${ptree_flag}" "${csv_flag}"
	done
else
	echo -e "\e[33mWarning: More input files provided than output files. Output will be redirected to default output/ directory \e[0m"
	for ((i = 0; i < ${#input_files[@]}; i++)); do
		if [ "$i" -lt "${#output_files[@]}" ]; then
			output_file="${output_files[$i]}"
		else
			output_file="${output_files_without_extention[$i]}.tac"
			output_files+="${output_file}"
			echo -e "\e[34m${output_file}\e[0m"
		fi
	done

	for ((i = 0; i < ${#input_files[@]}; i++)); do
		input_file="${input_files[$i]}"
		output_file1="${output_files[$i]}"
		output_file2="${output_files_without_extention[$i]}"

		echo ""
		./compiler "$input_file" "$output_file1" "$output_file2" "${verbose_flag}" "${ast_flag}" "${ptree_flag}" "${csv_flag}"
	done	
fi



if [ "$ptree_flag" -eq 1 ]; then
	for ((i = 0; i < ${#input_files[@]}; i++)); do
		output_file_without_extention="${output_files_without_extention[$i]}"

		pdf_output_file="${output_file_without_extention}_ptree.pdf"
		dot_output_file="${output_file_without_extention}_ptree.dot"

		if [ -f "$dot_output_file" ]; then
			dot -Tpdf "${dot_output_file}" -o "${pdf_output_file}" &
		fi
	done
fi


if [ "$ast_flag" -eq 1 ]; then
	for ((i = 0; i < ${#input_files[@]}; i++)); do
		output_file_without_extention="${output_files_without_extention[$i]}"

		pdf_output_file="${output_file_without_extention}_ast.pdf"
		dot_output_file="${output_file_without_extention}_ast.dot"

		if [ -f "$dot_output_file" ]; then
			dot -Tpdf "${dot_output_file}" -o "${pdf_output_file}" &
		fi
	done
fi

wait

if [ "$dot_flag" -eq 0 ]; then
	for ((i = 0; i < ${#input_files[@]}; i++)); do
		output_file_without_extention="${output_files_without_extention[$i]}"

		ptree_dot_output_file="${output_file_without_extention}_ptree.dot"
		ast_dot_output_file="${output_file_without_extention}_ast.dot"

		if [ -f "$ast_dot_output_file" ]; then
			rm "${ast_dot_output_file}" &
		fi

		if [ -f "$ptree_dot_output_file" ]; then
			rm "${ptree_dot_output_file}" &
		fi
	done
fi

wait