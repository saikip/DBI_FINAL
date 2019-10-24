/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YYSQL_YYSQL_TAB_H_INCLUDED
# define YY_YYSQL_YYSQL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yysqldebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    Name = 258,
    QualifiedName = 259,
    FileName = 260,
    Float = 261,
    Int = 262,
    String = 263,
    QUIT = 264,
    CREATE_TABLE = 265,
    HEAP = 266,
    SORTED = 267,
    INSERT = 268,
    INTO = 269,
    DROP_TABLE = 270,
    SET_OUTPUT = 271,
    STDOUT = 272,
    NONE = 273,
    INTEGER = 274,
    DBL = 275,
    STR = 276,
    SELECT = 277,
    GROUP = 278,
    DISTINCT = 279,
    BY = 280,
    FROM = 281,
    WHERE = 282,
    SUM = 283,
    AS = 284,
    AND = 285,
    OR = 286
  };
#endif
/* Tokens.  */
#define Name 258
#define QualifiedName 259
#define FileName 260
#define Float 261
#define Int 262
#define String 263
#define QUIT 264
#define CREATE_TABLE 265
#define HEAP 266
#define SORTED 267
#define INSERT 268
#define INTO 269
#define DROP_TABLE 270
#define SET_OUTPUT 271
#define STDOUT 272
#define NONE 273
#define INTEGER 274
#define DBL 275
#define STR 276
#define SELECT 277
#define GROUP 278
#define DISTINCT 279
#define BY 280
#define FROM 281
#define WHERE 282
#define SUM 283
#define AS 284
#define AND 285
#define OR 286

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 33 "ParserSQL.y" /* yacc.c:1909  */

 	struct FuncOperand *myOperand;
	struct FuncOperator *myOperator; 
	struct TableList *myTables;
	struct ComparisonOp *myComparison;
	struct Operand *myBoolOperand;
	struct OrList *myOrList;
	struct AndList *myAndList;
	struct NameList *myNames;
	struct CreateTable* createTable;
	struct AttDesc* attDesc;
	char *actualChars;
	char whichOne;
	int myInt;

#line 132 "yysql.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yysqllval;

int yysqlparse (void);

#endif /* !YY_YYSQL_YYSQL_TAB_H_INCLUDED  */
