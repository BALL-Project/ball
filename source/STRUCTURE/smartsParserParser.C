/* A Bison parser, made by GNU Bison 2.0.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse SmartsParserparse
#define yylex   SmartsParserlex
#define yyerror SmartsParsererror
#define yylval  SmartsParserlval
#define yychar  SmartsParserchar
#define yydebug SmartsParserdebug
#define yynerrs SmartsParsernerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_ATOM = 258,
     TK_ORG_SUBSET_ATOM = 259,
     TK_HYDROGEN_EXPLICIT = 260,
     TK_HYDROGEN_IMPLICIT = 261,
     TK_RECURSIVE = 262,
     TK_AROMATIC_ATOM = 263,
     TK_ALIPHATIC_ATOM = 264,
     TK_ATOM_RING_SIZE = 265,
     TK_ATOM_NUM_RINGS = 266,
     TK_ATOM_VALENCE = 267,
     TK_ATOM_CONNECTED = 268,
     TK_ATOM_RING_CONNECTED = 269,
     TK_ATOM_DEGREE = 270,
     TK_ANY_ATOM = 271,
     TK_CHIRAL_CLASS_TH = 272,
     TK_CHIRAL_CLASS_AL = 273,
     TK_CHIRAL_CLASS_SP = 274,
     TK_CHIRAL_CLASS_TB = 275,
     TK_CHIRAL_CLASS_OH = 276,
     TK_DIGIT = 277,
     TK_ONE_TWO = 278
   };
#endif
#define TK_ATOM 258
#define TK_ORG_SUBSET_ATOM 259
#define TK_HYDROGEN_EXPLICIT 260
#define TK_HYDROGEN_IMPLICIT 261
#define TK_RECURSIVE 262
#define TK_AROMATIC_ATOM 263
#define TK_ALIPHATIC_ATOM 264
#define TK_ATOM_RING_SIZE 265
#define TK_ATOM_NUM_RINGS 266
#define TK_ATOM_VALENCE 267
#define TK_ATOM_CONNECTED 268
#define TK_ATOM_RING_CONNECTED 269
#define TK_ATOM_DEGREE 270
#define TK_ANY_ATOM 271
#define TK_CHIRAL_CLASS_TH 272
#define TK_CHIRAL_CLASS_AL 273
#define TK_CHIRAL_CLASS_SP 274
#define TK_CHIRAL_CLASS_TB 275
#define TK_CHIRAL_CLASS_OH 276
#define TK_DIGIT 277
#define TK_ONE_TWO 278




/* Copy the first part of user declarations.  */
#line 1 "smartsParserParser.y"


#include <BALL/STRUCTURE/smartsParser.h>
//#include <iostream>
#include <BALL/KERNEL/PTE.h>

using namespace BALL;
using namespace std;

extern int yylex();
extern void yyerror(char* s);

/* ??? */
/* There is some nasty problem with alloca under Intel/icc/Linux */
/* so we use (inefficient) malloc instead. */
#undef alloca
#define alloca malloc

#define YYDEBUG 1



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 22 "smartsParserParser.y"
typedef union YYSTYPE {
	char*														text;
	Index														number;
	SmartsParser::SPAtom*						atom;
	SmartsParser::SPNode*						node;
	SmartsParser::SPEdge*						edge;
	/*SmartsParser::ChiralDef*				chiral;*/
	SmartsParser::SPBond*						sp_bond;
	/*SmartsParser::LogicalOperator		logical;*/
	NamedProperty*									property;
} YYSTYPE;
/* Line 190 of yacc.c.  */
#line 164 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 213 of yacc.c.  */
#line 176 "y.tab.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  68
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   488

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  43
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  29
/* YYNRULES -- Number of rules. */
#define YYNRULES  123
/* YYNRULES -- Number of states. */
#define YYNSTATES  154

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   278

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     6,     2,     7,     2,     2,     5,     2,
      32,    33,     2,     9,     4,     8,    34,    35,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,     3,
       2,    38,     2,    37,    10,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    41,    36,    42,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    40,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     9,    13,    18,    21,    23,    26,
      30,    32,    37,    43,    45,    48,    50,    53,    57,    61,
      65,    67,    69,    71,    74,    77,    79,    81,    83,    85,
      87,    92,    96,    99,   101,   103,   105,   107,   109,   113,
     117,   121,   124,   126,   129,   131,   134,   138,   142,   145,
     148,   150,   152,   155,   158,   160,   163,   165,   169,   172,
     176,   179,   182,   186,   189,   193,   196,   200,   203,   207,
     209,   211,   213,   215,   217,   219,   221,   223,   225,   228,
     231,   234,   237,   240,   243,   245,   247,   249,   251,   253,
     255,   257,   260,   262,   265,   268,   271,   273,   276,   278,
     281,   283,   286,   288,   291,   293,   296,   298,   301,   303,
     306,   310,   313,   315,   318,   322,   325,   327,   330,   332,
     335,   337,   340,   343
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      44,     0,    -1,    46,    -1,    32,    44,    33,    -1,    44,
      34,    44,    -1,    15,    32,    46,    33,    -1,     6,    45,
      -1,    47,    -1,    47,    46,    -1,    47,    48,    46,    -1,
      51,    -1,    47,    32,    46,    33,    -1,    47,    32,    48,
      46,    33,    -1,    49,    -1,     6,    49,    -1,    50,    -1,
       6,    50,    -1,    48,     3,    48,    -1,    48,     4,    48,
      -1,    48,     5,    48,    -1,     8,    -1,    35,    -1,    36,
      -1,    35,    37,    -1,    36,    37,    -1,    38,    -1,     7,
      -1,    39,    -1,    40,    -1,    10,    -1,    41,    52,    42,
      69,    -1,    41,    52,    42,    -1,    53,    69,    -1,    53,
      -1,    45,    -1,    54,    -1,    53,    -1,    45,    -1,    52,
       3,    52,    -1,    52,     4,    52,    -1,    52,     5,    52,
      -1,     6,    57,    -1,    57,    -1,     6,    55,    -1,    55,
      -1,    53,    55,    -1,    53,     6,    55,    -1,    53,     6,
      67,    -1,    53,    67,    -1,     6,    67,    -1,    67,    -1,
      56,    -1,     6,    56,    -1,     6,    68,    -1,    68,    -1,
       6,    71,    -1,    71,    -1,    53,     6,    68,    -1,    53,
      68,    -1,    53,     6,    71,    -1,    53,    71,    -1,    54,
      55,    -1,    54,     6,    68,    -1,    54,    68,    -1,    54,
       6,    67,    -1,    54,    67,    -1,    54,     6,    71,    -1,
      54,    71,    -1,    54,     6,    55,    -1,    65,    -1,    66,
      -1,    61,    -1,    62,    -1,    58,    -1,    59,    -1,    64,
      -1,    63,    -1,    70,    -1,    70,    12,    -1,    70,    11,
      -1,    70,    16,    -1,    70,    17,    -1,    70,    24,    -1,
      70,    60,    -1,    12,    -1,    16,    -1,    17,    -1,    24,
      -1,    60,    -1,    11,    -1,    14,    -1,    14,    30,    -1,
      13,    -1,    13,    30,    -1,     7,    30,    -1,    60,    30,
      -1,    20,    -1,    20,    30,    -1,    21,    -1,    21,    30,
      -1,    22,    -1,    22,    30,    -1,    23,    -1,    23,    30,
      -1,    19,    -1,    19,    30,    -1,    18,    -1,    18,    30,
      -1,     9,    -1,     9,     9,    -1,     9,     9,     9,    -1,
       9,    30,    -1,     8,    -1,     8,     8,    -1,     8,     8,
       8,    -1,     8,    30,    -1,    30,    -1,    69,    30,    -1,
      30,    -1,    70,    30,    -1,    10,    -1,    10,    10,    -1,
      10,    37,    -1,    10,    10,    37,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   112,   112,   117,   122,   138,   144,   152,   156,   167,
     177,   181,   192,   202,   209,   217,   225,   234,   244,   254,
     294,   298,   302,   306,   310,   314,   318,   322,   326,   333,
     340,   346,   351,   357,   362,   370,   375,   380,   426,   431,
     436,   460,   465,   469,   474,   479,   484,   489,   494,   499,
     504,   512,   516,   521,   526,   531,   538,   544,   549,   554,
     565,   575,   580,   585,   590,   595,   600,   611,   621,   630,
     635,   640,   644,   648,   652,   656,   660,   667,   672,   677,
     682,   687,   692,   697,   705,   709,   714,   719,   723,   727,
     734,   738,   745,   749,   756,   760,   767,   771,   778,   782,
     789,   793,   800,   804,   811,   815,   822,   826,   833,   834,
     835,   836,   840,   841,   842,   843,   847,   851,   858,   859,
     863,   868,   873,   878
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "';'", "','", "'&'", "'!'", "'#'", "'-'",
  "'+'", "'@'", "TK_ATOM", "TK_ORG_SUBSET_ATOM", "TK_HYDROGEN_EXPLICIT",
  "TK_HYDROGEN_IMPLICIT", "TK_RECURSIVE", "TK_AROMATIC_ATOM",
  "TK_ALIPHATIC_ATOM", "TK_ATOM_RING_SIZE", "TK_ATOM_NUM_RINGS",
  "TK_ATOM_VALENCE", "TK_ATOM_CONNECTED", "TK_ATOM_RING_CONNECTED",
  "TK_ATOM_DEGREE", "TK_ANY_ATOM", "TK_CHIRAL_CLASS_TH",
  "TK_CHIRAL_CLASS_AL", "TK_CHIRAL_CLASS_SP", "TK_CHIRAL_CLASS_TB",
  "TK_CHIRAL_CLASS_OH", "TK_DIGIT", "TK_ONE_TWO", "'('", "')'", "'.'",
  "'/'", "'\\\\'", "'?'", "'='", "':'", "'~'", "'['", "']'", "$accept",
  "smarts", "rec_expression", "node_expression", "sp_node", "sp_edge",
  "bond", "ring_bond", "atom_node", "node", "unbraced_atom", "atom",
  "atom_property", "atom_symbol", "unbraced_atom_symbol",
  "implicit_hydrogens", "explicit_hydrogens", "atomic_number", "valence",
  "connected", "ring_connected", "degree", "num_rings", "ring_size",
  "pos_charge", "neg_charge", "connection_list", "isotope", "chirality", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,    59,    44,    38,    33,    35,    45,    43,
      64,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,    40,    41,    46,    47,    92,    63,    61,    58,
     126,    91,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    43,    44,    44,    44,    45,    45,    46,    46,    46,
      47,    47,    47,    48,    48,    48,    48,    48,    48,    48,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    50,
      51,    51,    51,    51,    51,    52,    52,    52,    52,    52,
      52,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    55,
      55,    55,    55,    55,    55,    55,    55,    56,    56,    56,
      56,    56,    56,    56,    57,    57,    57,    57,    57,    57,
      58,    58,    59,    59,    60,    60,    61,    61,    62,    62,
      63,    63,    64,    64,    65,    65,    66,    66,    67,    67,
      67,    67,    68,    68,    68,    68,    69,    69,    70,    70,
      71,    71,    71,    71
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     3,     3,     4,     2,     1,     2,     3,
       1,     4,     5,     1,     2,     1,     2,     3,     3,     3,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       4,     3,     2,     1,     1,     1,     1,     1,     3,     3,
       3,     2,     1,     2,     1,     2,     3,     3,     2,     2,
       1,     1,     2,     2,     1,     2,     1,     3,     2,     3,
       2,     2,     3,     2,     3,     2,     3,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     2,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     1,     2,     1,     2,     1,     2,
       3,     2,     1,     2,     3,     2,     1,     2,     1,     2,
       1,     2,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,   108,    89,    84,    92,    90,     0,    85,
      86,   106,   104,    96,    98,   100,   102,    87,     0,     0,
       0,    34,     2,     7,    10,    33,    44,    42,    73,    74,
      88,    71,    72,    76,    75,    69,    70,    50,     0,     6,
      43,    41,    49,    94,   109,   111,    93,    91,     0,   107,
     105,    97,    99,   101,   103,     0,     0,   112,   120,   118,
      37,     0,    36,    35,    51,    54,    77,    56,     1,     0,
       0,    26,    20,    29,     0,    21,    22,    25,    27,    28,
       8,     0,    13,    15,     0,   116,    45,    48,    32,    95,
     110,     0,     3,    52,    53,    55,   113,   115,   121,   122,
       0,     0,     0,    31,     0,    58,    60,     0,    61,    65,
      63,    67,    79,    78,    80,    81,    82,   119,    83,     4,
      14,    16,     0,     0,    23,    24,     0,     0,     0,     9,
      46,    47,   117,     5,   114,   123,    38,    39,    40,    30,
      57,    59,    68,    64,    62,    66,    11,     0,     0,    26,
      17,    18,    19,    12
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,    20,    21,    22,    23,    81,    82,    83,    24,    61,
      25,    63,    26,    64,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    65,    88,    66,    67
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -73
static const short int yypact[] =
{
     214,   397,   -20,     7,   -73,   -73,   -13,    -5,    14,   -73,
     -73,    19,    20,    22,    23,    24,    30,   -73,   214,   316,
       6,   -73,   -73,   142,   -73,   360,   -73,   -73,   -73,   -73,
      37,   -73,   -73,   -73,   -73,   -73,   -73,   -73,     3,   -73,
     -73,   -73,   -73,   -73,    59,   -73,   -73,   -73,   245,   -73,
     -73,   -73,   -73,   -73,   -73,   -11,   341,    13,     5,   -73,
     -73,     9,   416,   434,   -73,   -73,   377,   -73,   -73,   214,
     281,   -20,   -73,   -73,   178,    -6,     2,   -73,   -73,   -73,
     -73,   106,   -73,   -73,   465,   -73,   -73,   -73,    46,   -73,
     -73,    44,   -73,   -73,   -73,   -73,    71,   -73,    47,   -73,
     316,   316,   316,    60,   450,   -73,   -73,   450,   -73,   -73,
     -73,   -73,   -73,   -73,   -73,   -73,   -73,   -73,    37,    57,
     -73,   -73,    61,   106,   -73,   -73,   132,   132,   132,   -73,
     -73,   -73,   -73,   -73,   -73,   -73,    31,    88,   -73,    46,
     -73,   -73,   -73,   -73,   -73,   -73,   -73,    62,   235,   -73,
      40,    91,   -73,   -73
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -73,   -10,     0,   -16,   -73,   -54,   -67,   -66,   -73,   -72,
     -14,   -73,    -1,    41,    10,   -73,   -73,    32,   -73,   -73,
     -73,   -73,   -73,   -73,     1,   -29,    -4,   -73,   -15
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      40,    39,    42,   120,   121,    62,    68,    80,    55,    38,
      43,    41,   100,   101,   102,    98,    44,    46,     8,    60,
     123,    96,    92,    69,    86,    47,    87,    94,   136,   137,
     138,   124,    91,   105,   110,   101,   102,    45,    39,   125,
      69,    95,    99,    97,   127,   128,    48,   106,   111,    49,
      50,   103,    51,    52,    53,    40,    39,    42,   122,   119,
      54,    86,   108,    87,   109,   129,    41,    89,    90,    40,
      39,    42,   150,   151,   152,   140,   132,   133,   144,   134,
      41,   120,   121,   130,   135,   131,    62,    62,    62,   141,
      85,    69,   145,   102,   146,   153,   128,    93,   118,   139,
      60,    60,    60,   130,     0,   131,   142,   147,   143,   126,
     127,   128,     1,     2,     0,     3,     0,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,     0,     0,     0,     0,     0,     0,     0,   148,   149,
      72,     0,    73,     0,     0,     0,     0,    19,    70,    71,
      72,     3,    73,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    75,    76,     0,
      77,    78,    79,     0,    74,     0,     0,    75,    76,     0,
      77,    78,    79,    19,    70,    71,    72,     3,    73,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    75,    76,     0,    77,    78,    79,    19,
       1,     2,     0,     3,     0,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,     0,
       0,     0,   149,    72,     0,    73,    18,     0,     0,     0,
       0,     1,     2,     0,     3,    19,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      75,    76,     0,    77,    78,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,    38,    71,    72,
       3,    73,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    75,    76,     0,    77,
      78,    79,    56,     2,    57,     3,    58,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,     0,     0,     0,     0,     0,    59,    38,     2,    57,
       3,    58,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    84,     0,     0,     3,
       0,    59,     0,     6,     7,     0,     0,     0,    11,    12,
      13,    14,    15,    16,     2,     0,     0,     0,   112,   113,
      85,     0,     0,   114,   115,     0,     0,     0,     0,     0,
       0,   116,     0,    38,     2,     0,     3,   117,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,   104,     0,    57,     3,    58,     0,     0,     6,
       7,     0,     0,     0,    11,    12,    13,    14,    15,    16,
     107,     0,    57,     3,    58,     0,     0,     6,     7,     0,
       0,     0,    11,    12,    13,    14,    15,    16,    57,     3,
      58,     0,     0,     6,     7,     0,     0,     0,    11,    12,
      13,    14,    15,    16,     3,     0,     0,     0,     6,     7,
       0,     0,     0,    11,    12,    13,    14,    15,    16
};

static const short int yycheck[] =
{
       1,     1,     1,    70,    70,    19,     0,    23,    18,     6,
      30,     1,     3,     4,     5,    10,     9,    30,    15,    19,
      74,     8,    33,    34,    25,    30,    25,    56,   100,   101,
     102,    37,    48,    62,    63,     4,     5,    30,    38,    37,
      34,    56,    37,    30,     4,     5,    32,    62,    63,    30,
      30,    42,    30,    30,    30,    56,    56,    56,    74,    69,
      30,    62,    63,    62,    63,    81,    56,    30,     9,    70,
      70,    70,   126,   127,   128,   104,    30,    33,   107,     8,
      70,   148,   148,    84,    37,    84,   100,   101,   102,   104,
      30,    34,   107,     5,    33,    33,     5,    56,    66,   103,
     100,   101,   102,   104,    -1,   104,   107,   123,   107,     3,
       4,     5,     6,     7,    -1,     9,    -1,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     6,     7,
       8,    -1,    10,    -1,    -1,    -1,    -1,    41,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    35,    36,    -1,
      38,    39,    40,    -1,    32,    -1,    -1,    35,    36,    -1,
      38,    39,    40,    41,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    -1,    38,    39,    40,    41,
       6,     7,    -1,     9,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      -1,    -1,     7,     8,    -1,    10,    32,    -1,    -1,    -1,
      -1,     6,     7,    -1,     9,    41,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      35,    36,    -1,    38,    39,    40,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    36,    -1,    38,
      39,    40,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    -1,    -1,    -1,    -1,    -1,    30,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,     6,    -1,    -1,     9,
      -1,    30,    -1,    13,    14,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    23,     7,    -1,    -1,    -1,    11,    12,
      30,    -1,    -1,    16,    17,    -1,    -1,    -1,    -1,    -1,
      -1,    24,    -1,     6,     7,    -1,     9,    30,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     6,    -1,     8,     9,    10,    -1,    -1,    13,
      14,    -1,    -1,    -1,    18,    19,    20,    21,    22,    23,
       6,    -1,     8,     9,    10,    -1,    -1,    13,    14,    -1,
      -1,    -1,    18,    19,    20,    21,    22,    23,     8,     9,
      10,    -1,    -1,    13,    14,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    23,     9,    -1,    -1,    -1,    13,    14,
      -1,    -1,    -1,    18,    19,    20,    21,    22,    23
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     6,     7,     9,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    32,    41,
      44,    45,    46,    47,    51,    53,    55,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,     6,    45,
      55,    57,    67,    30,     9,    30,    30,    30,    32,    30,
      30,    30,    30,    30,    30,    44,     6,     8,    10,    30,
      45,    52,    53,    54,    56,    68,    70,    71,     0,    34,
       6,     7,     8,    10,    32,    35,    36,    38,    39,    40,
      46,    48,    49,    50,     6,    30,    55,    67,    69,    30,
       9,    46,    33,    56,    68,    71,     8,    30,    10,    37,
       3,     4,     5,    42,     6,    68,    71,     6,    55,    67,
      68,    71,    11,    12,    16,    17,    24,    30,    60,    44,
      49,    50,    46,    48,    37,    37,     3,     4,     5,    46,
      55,    67,    30,    33,     8,    37,    52,    52,    52,    69,
      68,    71,    55,    67,    68,    71,    33,    46,     6,     7,
      48,    48,    48,    33
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  register short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;


  yyvsp[0] = yylval;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 113 "smartsParserParser.y"
    {
			SmartsParser::state.current_parser->setRoot((yyvsp[0].node));
			(yyval.node) = (yyvsp[0].node);			
		}
    break;

  case 3:
#line 118 "smartsParserParser.y"
    {
			SmartsParser::state.current_parser->setNextComponentNumberToSubTree((yyvsp[-1].node));
			(yyval.node) = (yyvsp[-1].node);
		}
    break;

  case 4:
#line 123 "smartsParserParser.y"
    {
			SmartsParser::state.current_parser->setRoot((yyvsp[-2].node));
			SmartsParser::state.current_parser->setComponentGrouping(true);
			SmartsParser::SPBond* b = new SmartsParser::SPBond(SmartsParser::SPBond::NOT_NECESSARILY_CONNECTED);
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond(b);
			e->setFirstSPNode((yyvsp[-2].node));
			e->setSecondSPNode((yyvsp[0].node));
			(yyvsp[-2].node)->addSPEdge(e);
			(yyval.node) = (yyvsp[-2].node);
		}
    break;

  case 5:
#line 139 "smartsParserParser.y"
    {
			(yyvsp[-1].node)->setRecursive(true);
			SmartsParser::state.current_parser->setRecursive(true);
			(yyval.node) = (yyvsp[-1].node);
		}
    break;

  case 6:
#line 145 "smartsParserParser.y"
    {
			(yyvsp[0].node)->setNot(true);
			(yyval.node) = (yyvsp[0].node);
		}
    break;

  case 7:
#line 153 "smartsParserParser.y"
    {
			(yyval.node) = (yyvsp[0].node);
		}
    break;

  case 8:
#line 157 "smartsParserParser.y"
    {
			SmartsParser::SPBond* b = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_OR_AROMATIC);
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond(b);
			e->setFirstSPNode((yyvsp[-1].node));
			e->setSecondSPNode((yyvsp[0].node));
			(yyvsp[-1].node)->addSPEdge(e);
			(yyval.node) = (yyvsp[-1].node);
		}
    break;

  case 9:
#line 168 "smartsParserParser.y"
    {
			(yyvsp[-1].edge)->setFirstSPNode((yyvsp[-2].node));
			(yyvsp[-1].edge)->setSecondSPNode((yyvsp[0].node));
			(yyvsp[-2].node)->addSPEdge((yyvsp[-1].edge));
			(yyval.node) = (yyvsp[-2].node);
		}
    break;

  case 10:
#line 178 "smartsParserParser.y"
    {
			(yyval.node) = (yyvsp[0].node);
		}
    break;

  case 11:
#line 182 "smartsParserParser.y"
    {
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			SmartsParser::SPBond* b = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_OR_AROMATIC);
			e->setSPBond(b);
			e->setFirstSPNode((yyvsp[-3].node));
			e->setSecondSPNode((yyvsp[-1].node));
			(yyvsp[-3].node)->addSPEdge(e);
			(yyval.node) = (yyvsp[-3].node);
		}
    break;

  case 12:
#line 193 "smartsParserParser.y"
    {
			(yyvsp[-2].edge)->setFirstSPNode((yyvsp[-4].node));
			(yyvsp[-2].edge)->setSecondSPNode((yyvsp[-1].node));
			(yyvsp[-4].node)->addSPEdge((yyvsp[-2].edge));
			(yyval.node) = (yyvsp[-4].node);
		}
    break;

  case 13:
#line 203 "smartsParserParser.y"
    {
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond((yyvsp[0].sp_bond));
			(yyval.edge) = e;
		}
    break;

  case 14:
#line 210 "smartsParserParser.y"
    {
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond((yyvsp[0].sp_bond));
			e->setNot(true);
			(yyval.edge) = e;
		}
    break;

  case 15:
#line 218 "smartsParserParser.y"
    {
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond((yyvsp[0].sp_bond));
			SmartsParser::state.current_parser->setNeedsSSSR(true);
			(yyval.edge) = e;
		}
    break;

  case 16:
#line 226 "smartsParserParser.y"
    {
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setSPBond((yyvsp[0].sp_bond));
			SmartsParser::state.current_parser->setNeedsSSSR(true);
			(yyvsp[0].sp_bond)->setNot(true);
			(yyval.edge) = e;
		}
    break;

  case 17:
#line 235 "smartsParserParser.y"
    {
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setInternal(true);
			e->setFirstSPEdge((yyvsp[-2].edge));
			e->setSecondSPEdge((yyvsp[0].edge));
			e->setLogicalOperator(SmartsParser::AND_LOW);
			(yyval.edge) = e;
		}
    break;

  case 18:
#line 245 "smartsParserParser.y"
    {
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setInternal(true);
			e->setFirstSPEdge((yyvsp[-2].edge));
			e->setSecondSPEdge((yyvsp[0].edge));
			e->setLogicalOperator(SmartsParser::OR);
			(yyval.edge) = e;
		}
    break;

  case 19:
#line 255 "smartsParserParser.y"
    {
			SmartsParser::SPEdge* e = new SmartsParser::SPEdge();
			SmartsParser::state.current_parser->addEdge(e);
			e->setInternal(true);
			e->setFirstSPEdge((yyvsp[-2].edge));
			e->setSecondSPEdge((yyvsp[0].edge));
			e->setLogicalOperator(SmartsParser::AND);
			(yyval.edge) = e;
		}
    break;

  case 20:
#line 295 "smartsParserParser.y"
    {
			(yyval.sp_bond) = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE);
		}
    break;

  case 21:
#line 299 "smartsParserParser.y"
    {
      (yyval.sp_bond) = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_UP);
		}
    break;

  case 22:
#line 303 "smartsParserParser.y"
    {
      (yyval.sp_bond) = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_DOWN);
		}
    break;

  case 23:
#line 307 "smartsParserParser.y"
    {
			(yyval.sp_bond) = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_UP_OR_ANY);
		}
    break;

  case 24:
#line 311 "smartsParserParser.y"
    {
			(yyval.sp_bond) = new SmartsParser::SPBond(SmartsParser::SPBond::SINGLE_DOWN_OR_ANY);
		}
    break;

  case 25:
#line 315 "smartsParserParser.y"
    {
		  (yyval.sp_bond) = new SmartsParser::SPBond(SmartsParser::SPBond::DOUBLE);
		}
    break;

  case 26:
#line 319 "smartsParserParser.y"
    {
		  (yyval.sp_bond) = new SmartsParser::SPBond(SmartsParser::SPBond::TRIPLE);
		}
    break;

  case 27:
#line 323 "smartsParserParser.y"
    {
		  (yyval.sp_bond) = new SmartsParser::SPBond(SmartsParser::SPBond::AROMATIC);
		}
    break;

  case 28:
#line 327 "smartsParserParser.y"
    {
		  (yyval.sp_bond) = new SmartsParser::SPBond(SmartsParser::SPBond::ANY);
		}
    break;

  case 29:
#line 334 "smartsParserParser.y"
    {
			(yyval.sp_bond) = new SmartsParser::SPBond(SmartsParser::SPBond::IN_RING);
		}
    break;

  case 30:
#line 341 "smartsParserParser.y"
    {
			(yyvsp[-2].node)->setInBrackets();
			SmartsParser::state.current_parser->addRingConnection((yyvsp[-2].node), (yyvsp[0].number));
			(yyval.node) = (yyvsp[-2].node);
		}
    break;

  case 31:
#line 347 "smartsParserParser.y"
    {
			(yyvsp[-1].node)->setInBrackets();
			(yyval.node) = (yyvsp[-1].node);
		}
    break;

  case 32:
#line 352 "smartsParserParser.y"
    {
			(yyval.node) = new SmartsParser::SPNode((yyvsp[-1].atom));
			SmartsParser::state.current_parser->addNode((yyval.node));
			SmartsParser::state.current_parser->addRingConnection((yyval.node), (yyvsp[0].number));
		}
    break;

  case 33:
#line 358 "smartsParserParser.y"
    {
			(yyval.node) = new SmartsParser::SPNode((yyvsp[0].atom));
			SmartsParser::state.current_parser->addNode((yyval.node));
		}
    break;

  case 34:
#line 363 "smartsParserParser.y"
    {
			(yyval.node) = (yyvsp[0].node);
		}
    break;

  case 35:
#line 371 "smartsParserParser.y"
    {
			(yyval.node) = new SmartsParser::SPNode((yyvsp[0].atom));
			SmartsParser::state.current_parser->addNode((yyval.node));
		}
    break;

  case 36:
#line 376 "smartsParserParser.y"
    {
			(yyval.node) = new SmartsParser::SPNode((yyvsp[0].atom));
			SmartsParser::state.current_parser->addNode((yyval.node));
		}
    break;

  case 37:
#line 381 "smartsParserParser.y"
    {
			(yyval.node) = (yyvsp[0].node);
		}
    break;

  case 38:
#line 427 "smartsParserParser.y"
    {
			(yyval.node) = new SmartsParser::SPNode((yyvsp[-2].node), SmartsParser::AND_LOW, (yyvsp[0].node));
			SmartsParser::state.current_parser->addNode((yyval.node));
		}
    break;

  case 39:
#line 432 "smartsParserParser.y"
    {
			(yyval.node) = new SmartsParser::SPNode((yyvsp[-2].node), SmartsParser::OR, (yyvsp[0].node));
			SmartsParser::state.current_parser->addNode((yyval.node));
		}
    break;

  case 40:
#line 437 "smartsParserParser.y"
    {
			(yyval.node) = new SmartsParser::SPNode((yyvsp[-2].node), SmartsParser::AND, (yyvsp[0].node));
			SmartsParser::state.current_parser->addNode((yyval.node));
		}
    break;

  case 41:
#line 461 "smartsParserParser.y"
    {
			(yyvsp[0].atom)->addAtomProperty(NamedProperty("Not", true));
			(yyval.atom) = (yyvsp[0].atom);
		}
    break;

  case 42:
#line 466 "smartsParserParser.y"
    {
			(yyval.atom) = (yyvsp[0].atom);
		}
    break;

  case 43:
#line 470 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom();
			(yyval.atom)->addAtomProperty(NamedProperty("Not"+(yyvsp[0].property)->getName(), (yyvsp[0].property)->getUnsignedInt()));
		}
    break;

  case 44:
#line 475 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom();
			(yyval.atom)->addAtomProperty(*(yyvsp[0].property));
		}
    break;

  case 45:
#line 480 "smartsParserParser.y"
    {
			(yyvsp[-1].atom)->addAtomProperty(*(yyvsp[0].property));
			(yyval.atom) = (yyvsp[-1].atom);
		}
    break;

  case 46:
#line 485 "smartsParserParser.y"
    {
			(yyvsp[-2].atom)->addAtomProperty(NamedProperty("Not"+(yyvsp[0].property)->getName(), (yyvsp[0].property)->getUnsignedInt()));
			(yyval.atom) = (yyvsp[-2].atom);
		}
    break;

  case 47:
#line 490 "smartsParserParser.y"
    {
			(yyvsp[-2].atom)->addAtomProperty(NamedProperty("NotCharge", Index((yyvsp[0].number))));
			(yyval.atom) = (yyvsp[-2].atom);
		}
    break;

  case 48:
#line 495 "smartsParserParser.y"
    {
			(yyvsp[-1].atom)->addAtomProperty(NamedProperty("Charge", Index((yyvsp[0].number))));
			(yyval.atom) = (yyvsp[-1].atom);
		}
    break;

  case 49:
#line 500 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom("*");
			(yyval.atom)->addAtomProperty(NamedProperty("NotCharge", Index((yyvsp[0].number))));
		}
    break;

  case 50:
#line 505 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom("*");
			(yyval.atom)->addAtomProperty(NamedProperty("Charge", Index((yyvsp[0].number))));
		}
    break;

  case 51:
#line 513 "smartsParserParser.y"
    {
			(yyval.atom) = (yyvsp[0].atom);
		}
    break;

  case 52:
#line 517 "smartsParserParser.y"
    {
			(yyvsp[0].atom)->addAtomProperty(NamedProperty("Not", true));
			(yyval.atom) = (yyvsp[0].atom);
		}
    break;

  case 53:
#line 522 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom("*");
			(yyval.atom)->addAtomProperty(NamedProperty("NotCharge", Index((yyvsp[0].number))));
		}
    break;

  case 54:
#line 527 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom("*");
			(yyval.atom)->addAtomProperty(NamedProperty("Charge", Index((yyvsp[0].number))));
		}
    break;

  case 55:
#line 532 "smartsParserParser.y"
    {
			//$$ = new SmartsParser::SPAtom("*");
			(yyval.atom) = (yyvsp[0].atom);
			//$$->setChirality(*$2);
			(yyval.atom)->addAtomProperty(NamedProperty("NotChirality", true));
		}
    break;

  case 56:
#line 539 "smartsParserParser.y"
    {
			//$$ = new SmartsParser::SPAtom("*");
			(yyval.atom) = (yyvsp[0].atom);
			//$$->setChirality(*$1);
		}
    break;

  case 57:
#line 545 "smartsParserParser.y"
    {
			(yyvsp[-2].atom)->addAtomProperty(NamedProperty("NotCharge", Index((yyvsp[0].number))));
			(yyval.atom) = (yyvsp[-2].atom);
		}
    break;

  case 58:
#line 550 "smartsParserParser.y"
    {
			(yyvsp[-1].atom)->addAtomProperty(NamedProperty("Charge", Index((yyvsp[0].number))));
			(yyval.atom) = (yyvsp[-1].atom);
		}
    break;

  case 59:
#line 555 "smartsParserParser.y"
    {
			(yyvsp[-2].atom)->addAtomProperty(NamedProperty("NotChirality", true));
			//$1->setChirality(*$3);
			for (Size i = 0; i != (yyvsp[0].atom)->countProperties(); ++i)
			{
				(yyvsp[-2].atom)->addAtomProperty((yyvsp[0].atom)->getNamedProperty(i));
			}
			delete (yyvsp[0].atom);
			(yyval.atom) = (yyvsp[-2].atom);
		}
    break;

  case 60:
#line 566 "smartsParserParser.y"
    {
			//$1->setChirality(*$2);
			for (Size i = 0; i != (yyvsp[0].atom)->countProperties(); ++i)
			{
				(yyvsp[-1].atom)->addAtomProperty((yyvsp[0].atom)->getNamedProperty(i));
			}
			delete (yyvsp[0].atom);
			(yyval.atom) = (yyvsp[-1].atom);
		}
    break;

  case 61:
#line 576 "smartsParserParser.y"
    {
			(yyvsp[-1].atom)->addAtomProperty(*(yyvsp[0].property));
			(yyval.atom) = (yyvsp[-1].atom);
		}
    break;

  case 62:
#line 581 "smartsParserParser.y"
    {
			(yyvsp[-2].atom)->addAtomProperty(NamedProperty("NotCharge", Index((yyvsp[0].number))));
			(yyval.atom) = (yyvsp[-2].atom);
		}
    break;

  case 63:
#line 586 "smartsParserParser.y"
    {
			(yyvsp[-1].atom)->addAtomProperty(NamedProperty("Charge", Index((yyvsp[0].number))));
			(yyval.atom) = (yyvsp[-1].atom);
		}
    break;

  case 64:
#line 591 "smartsParserParser.y"
    {
			(yyvsp[-2].atom)->addAtomProperty(NamedProperty("NotCharge", Index((yyvsp[0].number))));
			(yyval.atom) = (yyvsp[-2].atom);
		}
    break;

  case 65:
#line 596 "smartsParserParser.y"
    {
			(yyvsp[-1].atom)->addAtomProperty(NamedProperty("Charge", Index((yyvsp[0].number))));
			(yyval.atom) = (yyvsp[-1].atom);
		}
    break;

  case 66:
#line 601 "smartsParserParser.y"
    {
			//$1->setChirality(*$3);
			(yyvsp[-2].atom)->addAtomProperty(NamedProperty("NotChirality", true));
			for (Size i = 0; i != (yyvsp[0].atom)->countProperties(); ++i)
			{
				(yyvsp[-2].atom)->addAtomProperty((yyvsp[0].atom)->getNamedProperty(i));
			}
			delete (yyvsp[0].atom);
			(yyval.atom) = (yyvsp[-2].atom);
		}
    break;

  case 67:
#line 612 "smartsParserParser.y"
    {
			//$1->setChirality(*$2);
			for (Size i = 0; i != (yyvsp[0].atom)->countProperties(); ++i)
			{
				(yyvsp[-1].atom)->addAtomProperty((yyvsp[0].atom)->getNamedProperty(i));
			}
			delete (yyvsp[0].atom);
			(yyval.atom) = (yyvsp[-1].atom);
		}
    break;

  case 68:
#line 622 "smartsParserParser.y"
    {
			(yyvsp[-2].atom)->addAtomProperty(NamedProperty("Not"+(yyvsp[0].property)->getName(), (yyvsp[0].property)->getUnsignedInt()));
			delete (yyvsp[0].property);
			(yyval.atom) = (yyvsp[-2].atom);
		}
    break;

  case 69:
#line 631 "smartsParserParser.y"
    {
			(yyval.property) = new NamedProperty("InNumRings", Size((yyvsp[0].number)));
			SmartsParser::state.current_parser->setNeedsSSSR(true);
		}
    break;

  case 70:
#line 636 "smartsParserParser.y"
    {
			(yyval.property) = new NamedProperty("InRingSize", Size((yyvsp[0].number)));
			SmartsParser::state.current_parser->setNeedsSSSR(true);
		}
    break;

  case 71:
#line 641 "smartsParserParser.y"
    {
			(yyval.property) = new NamedProperty("Valence", Size((yyvsp[0].number)));
		}
    break;

  case 72:
#line 645 "smartsParserParser.y"
    {
			(yyval.property) = new NamedProperty("Connectivity", Size((yyvsp[0].number)));
		}
    break;

  case 73:
#line 649 "smartsParserParser.y"
    {
			(yyval.property) = new NamedProperty("ImplicitHydrogens", Size((yyvsp[0].number)));
		}
    break;

  case 74:
#line 653 "smartsParserParser.y"
    {
			(yyval.property) = new NamedProperty("ExplicitHydrogens", Size((yyvsp[0].number)));
		}
    break;

  case 75:
#line 657 "smartsParserParser.y"
    {
			(yyval.property) = new NamedProperty("Degree", Size((yyvsp[0].number)));
		}
    break;

  case 76:
#line 661 "smartsParserParser.y"
    {
			(yyval.property) = new NamedProperty("RingConnected", Size((yyvsp[0].number)));
		}
    break;

  case 77:
#line 668 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom("*");
			(yyval.atom)->addAtomProperty(NamedProperty("Isotope", Size((yyvsp[0].number))));
		}
    break;

  case 78:
#line 673 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom((yyvsp[0].text));
			(yyval.atom)->addAtomProperty(NamedProperty("Isotope", Size((yyvsp[-1].number))));
		}
    break;

  case 79:
#line 678 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom((yyvsp[0].text));
			(yyval.atom)->addAtomProperty(NamedProperty("Isotope", Size((yyvsp[-1].number))));
		}
    break;

  case 80:
#line 683 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom((yyvsp[0].text));
			(yyval.atom)->addAtomProperty(NamedProperty("Isotope", Size((yyvsp[-1].number))));
		}
    break;

  case 81:
#line 688 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom((yyvsp[0].text));
			(yyval.atom)->addAtomProperty(NamedProperty("Isotope", Size((yyvsp[-1].number))));
		}
    break;

  case 82:
#line 693 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom((yyvsp[0].text));
			(yyval.atom)->addAtomProperty(NamedProperty("Isotope", Size((yyvsp[-1].number))));
		}
    break;

  case 83:
#line 698 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom((yyvsp[0].number));
			(yyval.atom)->addAtomProperty(NamedProperty("Isotope", Size((yyvsp[-1].number))));
		}
    break;

  case 84:
#line 706 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom((yyvsp[0].text));
		}
    break;

  case 85:
#line 710 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom("*");
			(yyval.atom)->addAtomProperty(NamedProperty("Aromatic", true));
		}
    break;

  case 86:
#line 715 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom("*");
			(yyval.atom)->addAtomProperty(NamedProperty("Aliphatic", true));
		}
    break;

  case 87:
#line 720 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom("*");
		}
    break;

  case 88:
#line 724 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom((yyvsp[0].number));
		}
    break;

  case 89:
#line 728 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom((yyvsp[0].text));
		}
    break;

  case 90:
#line 735 "smartsParserParser.y"
    {
			(yyval.number) = 1;
		}
    break;

  case 91:
#line 739 "smartsParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 92:
#line 746 "smartsParserParser.y"
    {
			(yyval.number) = 1;
		}
    break;

  case 93:
#line 750 "smartsParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 94:
#line 757 "smartsParserParser.y"
    { 
			(yyval.number) = (yyvsp[0].number); 
		}
    break;

  case 95:
#line 761 "smartsParserParser.y"
    {
			(yyval.number) = 10 * (yyvsp[-1].number) + (yyvsp[0].number); 
		}
    break;

  case 96:
#line 768 "smartsParserParser.y"
    {
			(yyval.number) = 0;
		}
    break;

  case 97:
#line 772 "smartsParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 98:
#line 779 "smartsParserParser.y"
    {
			(yyval.number) = 0;
		}
    break;

  case 99:
#line 783 "smartsParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 100:
#line 790 "smartsParserParser.y"
    {
			(yyval.number) = 0;
		}
    break;

  case 101:
#line 794 "smartsParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 102:
#line 801 "smartsParserParser.y"
    {
			(yyval.number) = 0;
		}
    break;

  case 103:
#line 805 "smartsParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 104:
#line 812 "smartsParserParser.y"
    {
			(yyval.number) = 999;
		}
    break;

  case 105:
#line 816 "smartsParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 106:
#line 823 "smartsParserParser.y"
    {
			(yyval.number) = 0;
		}
    break;

  case 107:
#line 827 "smartsParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 108:
#line 833 "smartsParserParser.y"
    { (yyval.number) = 1; }
    break;

  case 109:
#line 834 "smartsParserParser.y"
    { (yyval.number) = 2; }
    break;

  case 110:
#line 835 "smartsParserParser.y"
    { (yyval.number) = 3; }
    break;

  case 111:
#line 836 "smartsParserParser.y"
    { (yyval.number) = (yyvsp[0].number); }
    break;

  case 112:
#line 840 "smartsParserParser.y"
    {	(yyval.number) = -1; }
    break;

  case 113:
#line 841 "smartsParserParser.y"
    { (yyval.number) = -2; }
    break;

  case 114:
#line 842 "smartsParserParser.y"
    { (yyval.number) = -3; }
    break;

  case 115:
#line 843 "smartsParserParser.y"
    { (yyval.number) = -(yyvsp[0].number); }
    break;

  case 116:
#line 848 "smartsParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 117:
#line 852 "smartsParserParser.y"
    {
			(yyval.number) = (yyvsp[-1].number) + 10 * (yyvsp[0].number);
		}
    break;

  case 118:
#line 858 "smartsParserParser.y"
    { (yyval.number) = (yyvsp[0].number);	}
    break;

  case 119:
#line 859 "smartsParserParser.y"
    { (yyval.number) = (yyvsp[-1].number) + 10 * (yyvsp[0].number); }
    break;

  case 120:
#line 864 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom();
			(yyval.atom)->addAtomProperty(NamedProperty("Chirality", SmartsParser::CCW_DEFAULT));
		}
    break;

  case 121:
#line 869 "smartsParserParser.y"
    { 
			(yyval.atom) = new SmartsParser::SPAtom();
			(yyval.atom)->addAtomProperty(NamedProperty("Chirality", SmartsParser::CW_DEFAULT));
		}
    break;

  case 122:
#line 874 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom();
			(yyval.atom)->addAtomProperty(NamedProperty("Chirality", SmartsParser::CCW_DEFAULT_OR_UNSPECIFIED));
		}
    break;

  case 123:
#line 879 "smartsParserParser.y"
    {
			(yyval.atom) = new SmartsParser::SPAtom();
			(yyval.atom)->addAtomProperty(NamedProperty("Chirality", SmartsParser::CW_DEFAULT_OR_UNSPECIFIED));
		}
    break;


    }

/* Line 1037 of yacc.c.  */
#line 2296 "y.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {

		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 yydestruct ("Error: popping",
                             yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yydestruct ("Error: discarding lookahead",
              yytoken, &yylval);
  yychar = YYEMPTY;
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 910 "smartsParserParser.y"


void yyerror(char* s)
{
	throw Exception::ParseError(__FILE__, 0, 
															SmartsParser::state.buffer, 
															String(s) + String(" (at position ") 
																+ String(SmartsParser::state.char_count) + String(")"));
}



