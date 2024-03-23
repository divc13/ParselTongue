# ParselTongue: Translating to a Python's Language

## Introduction: Speaking to a Serpent

In this milestone, we constructed a scanner and a parser for a statically typed subset of the Python language. The output of the compiler is a graphical representation of the abstract syntax tree of the input program. 

Read the complete documentation [here]()

## Roadmap: How we built?

1. Formulated the patterns for converting lexemes into tokens and built a lexer
2. Modifed the [grammar of Python 3.12](https://docs.python.org/3/reference/grammar.html) for the required statically typed subset of Python.
3. Removed all the shift-reduce and reduce-reduce conflicts int grammar to use the Bison-LALR parser.
4. Interfaced the lexer and parser and added semantic actions to the lexer to return appropriate tokens to the parser.
5. Developed another lexer to add semantic actions to the parser directly to generate the parse tree.
6. Cleaned up the parse tree in a systematic way to build an abstrct syntax tree (AST) out of it.
7. Generated DOT code from the AST and visualized the AST.

## Installation and Prerequisites: How do you acquire the superpower

To acquire the superpower for speaking to serpents, you must have the following pre-requisites installed:

1. Bison: Install using the following command 
     ```sudo apt-get install bison```
2. Flex: Install using the following command
    ```sudo apt-get install flex```
3. Graphviz: Install using the following command
   ```sudo apt-get install graphviz```
4. ParselTongue: 
```git clone https://git.cse.iitk.ac.in/divyanshc/python-compiler```


## Usage: Using the superpower
The source files for milestone 1 are all inside the directory milestone1/src. A bash wrapper ```ParselTongue.sh``` is provided for compilation and execution. This wrapper uses the Makefile for compilation and also refactors command line arguments for the main compiler.
The options provided are as follows:
* ```-h``` or ```-help``` or ```--help``` : Shows a manual for usage of compiler
* ```-i``` or ```-input``` or ```--input``` : One can specify multiple input Python files for compiler
* ```-o``` or ```-output``` or ```--output``` : One can specify multiple output PDF files for compiler
* ```-v``` or ```-verbose``` or ```--verbose``` : Passing this option will generate a Parse Tree at the output file instead of classic AST

## Execution Examples

Inside ```milestone1/src``` :
* ```./ParselTongue.sh ../tests/test1.py```
* ```./ParselTongue.sh test1.py```
* ```./ParselTongue.sh -i ../tests/test1.py```
* ```./ParselTongue.sh -i ../tests/test1.py -o ../output/test1.pdf```
* ```./ParselTongue.sh -v -i ../tests/test1.py -o ../output/test1.pdf```
* ```./ParselTongue.sh ../tests/test1.py ../tests/test2.py -o ../output/test1.pdf```
* ```./ParselTongue.sh ../tests/test1.py -v -i ../tests/test2.py -o ../output/test1.pdf```

Parsel Tongue will make new output file with the help of input files if there are not enough output files. All the files specified before ```-o``` are by default taken as input files. If an input file is not found in the specified-path, then the script looks for ```../tests/specified-path``` and takes the input file from there if it exists. File extensions are not checked in the script so one can even pass file not ending in ```.py``` and still get the desired output.


<!-- ## Authors and acknowledgment -->
