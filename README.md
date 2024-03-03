# ParselTongue: Translating to a Python's Language

## Introduction: Speaking to a Serpent

In the magical world of Harry Potter, snakes possess the ability to communicate using ParselTongue, a language characterized by its hissing sounds. This skill is considered rare and is typically passed down through generations, known as a hereditary trait. Harry Potter, the famous wizard, had the unique ability to understand and speak ParselTongue. 

But only very few of us have this ability to understand a serpent's language. Do we all need to learn ParselTongue to talk to serpents? The answer to this question cannot be a yes. While some of us could learn ParselTongue, others could be helped by these ParselTongue learners, in translating their message. But who would make the first move to learn ParselTongue? 

Here we come to help you, and provide you with the magical powers to talk to serpents, atleast Python, if not all. On gaining this magical power, you would feel that you are speaking your everyday language, and on your behalf, we would translate your daily language to the ParselTongue, the language of serpents.

The analogy becomes clearer if you consider your daily language to be Python and ParselTongue, the languages of serpents, to be the x86-64 assembly code. It is much easier for us to program in a high level language like Python, but it would be tremendous amount of work to program directly in assembly. Hence our objective and motivation for this project is to create a compiler to translate specifically Python 3.12 to x86-64 assembly code.  

## Roadmap: Building the superpower for you

We aim to build the translator in three phases as below:

- [x] [Milestone 1: From Python to Abstract Syntax Tree](https://git.cse.iitk.ac.in/divyanshc/python-compiler/-/tree/main/milestone1)
In this milestone, we constructed a scanner and a parser for a statically typed subset of the Python language. The output of the compiler is a graphical representation of the abstract syntax tree of the input program. 

- [ ] [Milestone 2: From Abstract Syntax Tree to 3AC IR]()
 In this milestone, we plan to generate 3AC IR for the input source program and add runtime support for making function calls. We would also need to 
(i) implement support for the symbol table
(ii) perform semantic analysis to do limited error checking on types and function signatures. 

- [ ] [Milestone 3: From 3AC IR to x86-64]()
In this milestone, we plan to generate the correct x86_64 assembly from the 3AC which can be run via GAS on Linux.

## Language Features Supported:

1. Primitive data types (e.g., int, float, str, and bool)
2. 1D list (ignore dictionaries, tuples, and sets)
3. Basic operators:
    * Arithmetic operators: +, -, *, /, //, %, **
    * Relational operators: ==, !=, >, <, >=, <=
    * Logical operators: and, or, not
    * Bitwise operators: &, |, ˆ, ˜, <<, >>
    * Assignment operators: =, +=, -=, *=, /=, //=, %=, **=, &=, |=, ˆ=, «=, »=
4. Control flow via if-elif-else, for, while, break and continue (ignore pass, do-while and
switch)
    * Support iterating over ranges specified using the range() function.
5. Support for recursion
6. Support the library function print() for only printing the primitive Python types, one at a time
7. Support for classes and objects, including multilevel inheritance and constructors. Ignore multiple inheritance (i.e., a class can have only one parent class).
8. Methods and method calls, including both static and non-static methods
9. Static polymorphism via method overloading


## Installation and Prerequisites: How do you acquire the superpower

To acquire the superpower for speaking to serpents, you must have the following pre-requisites installed:

1. Bison: Install using the following command 
     ```sudo apt-get install bison```
2. Flex: Install using the following command
    ```sudo apt-get install flex```
3. Graphviz: Install using the following command
   ```sudo apt-get install graphviz```
4. ParselTongue: Get the superpower by cloning the repository
```git clone https://git.cse.iitk.ac.in/divyanshc/python-compiler```


## Usage: Using the superpower
While the development is in progress, you can refer to [milestone 1](https://git.cse.iitk.ac.in/divyanshc/python-compiler/-/tree/main/milestone1) documentation for building the abstract syntax tree.

<!-- ## Authors and acknowledgment -->
