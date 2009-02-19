/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_OPEN = 258,
     TK_END = 259,
     TK_AND = 260,
     TK_MATCH_TRUE = 261,
     ATOMSTRING = 262,
     OPTIONAL_NUMBER = 263,
     TK_APS_START = 264,
     TK_APS_END = 265,
     TK_APS_AND_TERM = 266,
     TK_APS_OR_TERM = 267,
     TK_APS_DELOCALIZED = 268,
     TK_APS_PURE_SINGLE_BOND = 269,
     TK_APS_PURE_DOUBLE_BOND = 270,
     TK_APS_TRIPLE_BOND = 271,
     TK_APS_AROMATIC_BOND = 272,
     TK_APS_ALL_SINGLE_BONDS = 273,
     TK_APS_ALL_DOUBLE_BONDS = 274,
     TK_APS_NOT_BOUND_TO_PARENT = 275,
     TK_APS_BOUND_TO_PARENT = 276,
     TK_APS_PURE_AROMATIC_RING = 277,
     TK_APS_PLANAR_RING = 278,
     TK_APS_PLANAR_RING_WITH_DB = 279,
     TK_APS_OTHER_RING_ATOM = 280,
     TK_APS_PURE_ALIPHATIC_RING = 281,
     TK_APS_THREE_RING = 282,
     TK_APS_FOUR_RING = 283,
     TK_APS_FIVE_RING = 284,
     TK_APS_SIX_RING = 285,
     TK_APS_SEVEN_RING = 286,
     TK_APS_EIGHT_RING = 287,
     TK_APS_NINE_RING = 288,
     TK_APS_RING = 289,
     TK_APS_NON_RING = 290,
     TK_APS_TRUE = 291,
     TK_APS_NUMBER = 292
   };
#endif
/* Tokens.  */
#define TK_OPEN 258
#define TK_END 259
#define TK_AND 260
#define TK_MATCH_TRUE 261
#define ATOMSTRING 262
#define OPTIONAL_NUMBER 263
#define TK_APS_START 264
#define TK_APS_END 265
#define TK_APS_AND_TERM 266
#define TK_APS_OR_TERM 267
#define TK_APS_DELOCALIZED 268
#define TK_APS_PURE_SINGLE_BOND 269
#define TK_APS_PURE_DOUBLE_BOND 270
#define TK_APS_TRIPLE_BOND 271
#define TK_APS_AROMATIC_BOND 272
#define TK_APS_ALL_SINGLE_BONDS 273
#define TK_APS_ALL_DOUBLE_BONDS 274
#define TK_APS_NOT_BOUND_TO_PARENT 275
#define TK_APS_BOUND_TO_PARENT 276
#define TK_APS_PURE_AROMATIC_RING 277
#define TK_APS_PLANAR_RING 278
#define TK_APS_PLANAR_RING_WITH_DB 279
#define TK_APS_OTHER_RING_ATOM 280
#define TK_APS_PURE_ALIPHATIC_RING 281
#define TK_APS_THREE_RING 282
#define TK_APS_FOUR_RING 283
#define TK_APS_FIVE_RING 284
#define TK_APS_SIX_RING 285
#define TK_APS_SEVEN_RING 286
#define TK_APS_EIGHT_RING 287
#define TK_APS_NINE_RING 288
#define TK_APS_RING 289
#define TK_APS_NON_RING 290
#define TK_APS_TRUE 291
#define TK_APS_NUMBER 292




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 20 "GAFFCESParserParser.y"
{
	char*		text;
	int			number;
}
/* Line 1489 of yacc.c.  */
#line 128 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE GAFFCESParserlval;

