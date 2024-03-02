#!/bin/bash

input_files=()
output_pdf_files=()
output_dot_files=()
verbose_flag=0



display_help() {
    echo -e "\e[32mUsage: $0 [-v] [-i input_files...] [-o output_pdf_files...]"
    echo -e "Options:"
    echo -e "  -v, --verbose         : Make Parse Tree instead of the AST at the output files"
    echo -e "  -i, --input           : Input PYTHON file(s)"
    echo -e "  -o, --output          : Output PDF file(s)"
    echo -e "  --help, -h            : Display this help message\e[0m"
}

file_exists() {
	if [ ! -f "$1" ]; then
	echo "Error: File '$1' not found"
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
	--input|-input|-i )
		for ((j=i+1; j<=$#; j++)); do
		next_arg="${!j}"
		if [[ "$next_arg" == -* ]]; then
			break
		fi
		file_exists "$next_arg"
		input_files+=("$next_arg")
		done
		i=j-1
		;;
	--output|-output|-o )
		for ((j=i+1; j<=$#; j++)); do
		next_arg="${!j}"
		if [[ "$next_arg" == -* ]]; then
			break
		fi
		output_directory=$(dirname "$next_arg")
        if [ -z "$output_directory" ]; then
            output_directory="output"
        fi
		create_directory_if_not_exists "$output_directory"
		filename_no_extension=$(basename "$next_arg" .pdf)
		output_pdf_files+=("${output_directory}/${filename_no_extension}.pdf")
		done
		i=j-1
		;;
	--help|-help|-h )
		display_help
		exit 0
		;;
	--verbose|-verbose|-v )
		verbose_flag=1
		;;
	-* )
		echo -e "\e[31mUnknown option: $arg\e[0m"
		exit 1
		;;
	* )
		for ((j = i; j<=$#; j++)); do
		next_arg="${!j}"
		if [[ "$next_arg" == -* ]]; then
			break
		fi
		file_exists "$next_arg"
		input_files+=("$next_arg")
		done
		i=j-1
	;;
	esac
done

make

for ((i = 0; i < ${#input_files[@]}; i++)); do
	if [ "$i" -lt "${#output_pdf_files[@]}" ]; then
		output_pdf_file="${output_pdf_files[$i]}"
	else
		output_pdf_file="output/output_PT_${i}.pdf"
	fi
    output_directory=$(dirname "$output_pdf_file")
	create_directory_if_not_exists "$output_directory"
	filename_no_extension=$(basename "$output_pdf_file" .pdf)
	dot_output_file="${output_directory}/${filename_no_extension}.dot"

    output_dot_files+=("$dot_output_file")
done

if [ "${#input_files[@]}" -eq "${#output_pdf_files[@]}" ]; then
    for ((i = 0; i < ${#input_files[@]}; i++)); do
        input_file="${input_files[$i]}"
        output_file="${output_dot_files[$i]}"

		./parser "${verbose_flag}" "$input_file" "$output_file" &
    done
elif [ "${#input_files[@]}" -lt "${#output_pdf_files[@]}" ]; then
    echo -e "\e[33mWarning: Less input files provided than output files. The additional output files wont be used.\e[0m"

    for ((i = 0; i < ${#input_files[@]}; i++)); do
        input_file="${input_files[$i]}"
        output_file="${output_dot_files[$i]}"

		./parser "${verbose_flag}" "$input_file" "$output_file" &
    done
else
    echo -e "\e[33mWarning: More input files provided than output files.\e[0m"
	for ((i = 0; i < ${#input_files[@]}; i++)); do
        if [ "$i" -lt "${#output_pdf_files[@]}" ]; then
			output_file="${output_pdf_files[$i]}"
		else
			output_file="output/output_PT_${i}.pdf"
			echo -e "\e[34m${output_file}\e[0m"
		fi
	done
    read -p "Do you want to proceed with the above additional default output files? (y/n): " response

    if [ "$response" != "y" ] && [ "$response" != "Y" ]; then
        echo "Exiting script."
        exit 1
    fi

    for ((i = 0; i < ${#input_files[@]}; i++)); do
        input_file="${input_files[$i]}"
		output_file="${output_dot_files[$i]}"

		./parser "${verbose_flag}" "$input_file" "$output_file" &
    done
fi

wait

for ((i = 0; i < ${#input_files[@]}; i++)); do
	dot_output_file="${output_dot_files[$i]}"

	output_directory=$(dirname "$dot_output_file")
	filename_no_extension=$(basename "$dot_output_file" .dot)
	pdf_output_file="${output_directory}/${filename_no_extension}.pdf"

    if [ -f "$dot_output_file" ]; then
    	dot -Tpdf "${dot_output_file}" -o "${pdf_output_file}" &
	fi
done

wait

for ((i = 0; i < ${#input_files[@]}; i++)); do
	dot_output_file="${output_dot_files[$i]}"

done