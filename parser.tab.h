/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 25 "parser.y"


	#include<bits/stdc++.h>
	using namespace std;

	struct typeA{
		int lineno;
		char* name;
	};

	struct typeB{
		char* name;
		char* type;
	};


#line 66 "parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NEWLINE = 258,                 /* NEWLINE  */
    ENDMARKER = 259,               /* ENDMARKER  */
    INDENT = 260,                  /* INDENT  */
    DEDENT = 261,                  /* DEDENT  */
    LIST = 262,                    /* LIST  */
    KW_False = 263,                /* KW_False  */
    KW_await = 264,                /* KW_await  */
    KW_else = 265,                 /* KW_else  */
    KW_import = 266,               /* KW_import  */
    KW_None = 267,                 /* KW_None  */
    KW_break = 268,                /* KW_break  */
    KW_except = 269,               /* KW_except  */
    KW_in = 270,                   /* KW_in  */
    KW_raise = 271,                /* KW_raise  */
    KW_True = 272,                 /* KW_True  */
    KW_class = 273,                /* KW_class  */
    KW_finally = 274,              /* KW_finally  */
    KW_is = 275,                   /* KW_is  */
    KW_return = 276,               /* KW_return  */
    KW_continue = 277,             /* KW_continue  */
    KW_for = 278,                  /* KW_for  */
    KW_lambda = 279,               /* KW_lambda  */
    KW_tryas = 280,                /* KW_tryas  */
    KW_def = 281,                  /* KW_def  */
    KW_from = 282,                 /* KW_from  */
    KW_nonlocal = 283,             /* KW_nonlocal  */
    KW_while = 284,                /* KW_while  */
    KW_assert = 285,               /* KW_assert  */
    KW_del = 286,                  /* KW_del  */
    KW_global = 287,               /* KW_global  */
    KW_with = 288,                 /* KW_with  */
    KW_async = 289,                /* KW_async  */
    KW_elif = 290,                 /* KW_elif  */
    KW_if = 291,                   /* KW_if  */
    KW_yield = 292,                /* KW_yield  */
    DT_int = 293,                  /* DT_int  */
    DT_float = 294,                /* DT_float  */
    DT_bool = 295,                 /* DT_bool  */
    DT_str = 296,                  /* DT_str  */
    IDENTIFIER = 297,              /* IDENTIFIER  */
    LITERAL = 298,                 /* LITERAL  */
    OP_ATH_ADD = 299,              /* OP_ATH_ADD  */
    OP_ATH_SUB = 300,              /* OP_ATH_SUB  */
    OP_ATH_MUL = 301,              /* OP_ATH_MUL  */
    OP_ATH_DIV = 302,              /* OP_ATH_DIV  */
    OP_ATH_FDIV = 303,             /* OP_ATH_FDIV  */
    OP_ATH_MOD = 304,              /* OP_ATH_MOD  */
    OP_ATH_POW = 305,              /* OP_ATH_POW  */
    OP_REL_EQ = 306,               /* OP_REL_EQ  */
    OP_REL_NEQ = 307,              /* OP_REL_NEQ  */
    OP_REL_GT = 308,               /* OP_REL_GT  */
    OP_REL_LT = 309,               /* OP_REL_LT  */
    OP_REL_GTE = 310,              /* OP_REL_GTE  */
    OP_REL_LTE = 311,              /* OP_REL_LTE  */
    OP_LOG_AND = 312,              /* OP_LOG_AND  */
    OP_LOG_OR = 313,               /* OP_LOG_OR  */
    OP_LOG_NOT = 314,              /* OP_LOG_NOT  */
    OP_BIT_AND = 315,              /* OP_BIT_AND  */
    OP_BIT_OR = 316,               /* OP_BIT_OR  */
    OP_BIT_XOR = 317,              /* OP_BIT_XOR  */
    OP_BIT_NEG = 318,              /* OP_BIT_NEG  */
    OP_BIT_LS = 319,               /* OP_BIT_LS  */
    OP_BIT_RS = 320,               /* OP_BIT_RS  */
    OP_ASN_ASN = 321,              /* OP_ASN_ASN  */
    OP_ASN_ADD = 322,              /* OP_ASN_ADD  */
    OP_ASN_SUB = 323,              /* OP_ASN_SUB  */
    OP_ASN_MUL = 324,              /* OP_ASN_MUL  */
    OP_ASN_DIV = 325,              /* OP_ASN_DIV  */
    OP_ASN_FDIV = 326,             /* OP_ASN_FDIV  */
    OP_ASN_MOD = 327,              /* OP_ASN_MOD  */
    OP_ASN_POW = 328,              /* OP_ASN_POW  */
    OP_ASN_AND = 329,              /* OP_ASN_AND  */
    OP_ASN_OR = 330,               /* OP_ASN_OR  */
    OP_ASN_XOR = 331,              /* OP_ASN_XOR  */
    OP_ASN_LS = 332,               /* OP_ASN_LS  */
    OP_ASN_RS = 333,               /* OP_ASN_RS  */
    DLM_LFT_PRN = 334,             /* DLM_LFT_PRN  */
    DLM_RGT_PRN = 335,             /* DLM_RGT_PRN  */
    DLM_LFT_SQ = 336,              /* DLM_LFT_SQ  */
    DLM_RGT_SQ = 337,              /* DLM_RGT_SQ  */
    DLM_LFT_CRLY = 338,            /* DLM_LFT_CRLY  */
    DLM_RGT_CRLY = 339,            /* DLM_RGT_CRLY  */
    DLM_COMMA = 340,               /* DLM_COMMA  */
    DLM_COLON = 341,               /* DLM_COLON  */
    DLM_DOT = 342,                 /* DLM_DOT  */
    DLM_SM_COL = 343,              /* DLM_SM_COL  */
    DLM_AT = 344,                  /* DLM_AT  */
    DLM_TO = 345                   /* DLM_TO  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 42 "parser.y"

		
	// Used for KEYWORD, DATATYPE, DELIMITER
	char* str;

	// Used for IDENTIFIER
	struct typeA aType;

	// USED for LITERAL, OPERATOR
	struct typeB bType;

#line 185 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
