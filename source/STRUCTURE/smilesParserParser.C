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
#define yyparse SmilesParserparse
#define yylex   SmilesParserlex
#define yyerror SmilesParsererror
#define yylval  SmilesParserlval
#define yychar  SmilesParserchar
#define yydebug SmilesParserdebug
#define yynerrs SmilesParsernerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TK_ATOM = 258,
     TK_ORG_SUBSET_ATOM = 259,
     TK_HYDROGEN = 260,
     TK_DIGIT = 261,
     TK_ONE_TWO = 262,
     TK_ONE_TWO_THREE = 263,
     TK_CHIRAL_CLASS_TH = 264,
     TK_CHIRAL_CLASS_AL = 265,
     TK_CHIRAL_CLASS_SP = 266,
     TK_CHIRAL_CLASS_TB = 267,
     TK_CHIRAL_CLASS_OH = 268
   };
#endif
#define TK_ATOM 258
#define TK_ORG_SUBSET_ATOM 259
#define TK_HYDROGEN 260
#define TK_DIGIT 261
#define TK_ONE_TWO 262
#define TK_ONE_TWO_THREE 263
#define TK_CHIRAL_CLASS_TH 264
#define TK_CHIRAL_CLASS_AL 265
#define TK_CHIRAL_CLASS_SP 266
#define TK_CHIRAL_CLASS_TB 267
#define TK_CHIRAL_CLASS_OH 268




/* Copy the first part of user declarations.  */
#line 1 "smilesParserParser.y"


#include <BALL/STRUCTURE/smilesParser.h>

using namespace BALL;
using namespace std;

extern int yylex();
extern void yyerror(char* s);

/* ??? */
/* There is some nasty problem with alloca under Intel/icc/Linux */
/* so we use (inefficient) malloc instead. */
#undef alloca
#define alloca malloc



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
#line 18 "smilesParserParser.y"
typedef union YYSTYPE {
	char*														text;
	Index														number;
	SmilesParser::SPAtom*						atom;
	SmilesParser::ConnectionList*		conns;
	SmilesParser::ChiralDef*				chiral;
} YYSTYPE;
/* Line 190 of yacc.c.  */
#line 136 "y.tab.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 213 of yacc.c.  */
#line 148 "y.tab.c"

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
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   66

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  27
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  17
/* YYNRULES -- Number of rules. */
#define YYNRULES  51
/* YYNRULES -- Number of states. */
#define YYNSTATES  68

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   268

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    23,     2,     2,     2,     2,
      14,    15,     2,    18,     2,    19,    20,    24,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    21,     2,
       2,    22,     2,     2,    26,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    16,    25,    17,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     5,     7,    11,    14,    16,    21,    27,
      30,    39,    45,    46,    48,    51,    53,    55,    57,    58,
      60,    62,    64,    67,    71,    74,    76,    79,    83,    86,
      87,    89,    91,    94,    95,    97,    99,   102,   104,   106,
     108,   110,   112,   114,   116,   117,   119,   122,   125,   128,
     131,   134
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      28,     0,    -1,    29,    -1,    30,    -1,    30,    42,    29,
      -1,    30,    29,    -1,    31,    -1,    30,    14,    29,    15,
      -1,    30,    14,    42,    29,    15,    -1,    33,    38,    -1,
      16,    40,    34,    43,    32,    35,    17,    38,    -1,    16,
      40,     5,    35,    17,    -1,    -1,     5,    -1,     5,     6,
      -1,     4,    -1,    33,    -1,     3,    -1,    -1,    36,    -1,
      37,    -1,    18,    -1,    18,    18,    -1,    18,    18,    18,
      -1,    18,     6,    -1,    19,    -1,    19,    19,    -1,    19,
      19,    19,    -1,    19,     6,    -1,    -1,    39,    -1,     6,
      -1,    39,     6,    -1,    -1,    41,    -1,     6,    -1,    41,
       6,    -1,    20,    -1,    19,    -1,    21,    -1,    22,    -1,
      23,    -1,    24,    -1,    25,    -1,    -1,    26,    -1,    26,
      26,    -1,     9,     7,    -1,    10,     7,    -1,    12,     6,
      -1,    11,     6,    -1,    13,     6,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned char yyrline[] =
{
       0,    67,    67,    73,    74,    75,    79,    80,    81,    85,
      93,   117,   127,   128,   132,   139,   143,   144,   148,   151,
     155,   162,   163,   164,   165,   169,   170,   171,   172,   176,
     177,   184,   189,   197,   198,   203,   204,   208,   209,   210,
     211,   212,   213,   214,   218,   219,   223,   227,   231,   235,
     239,   243
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_ATOM", "TK_ORG_SUBSET_ATOM",
  "TK_HYDROGEN", "TK_DIGIT", "TK_ONE_TWO", "TK_ONE_TWO_THREE",
  "TK_CHIRAL_CLASS_TH", "TK_CHIRAL_CLASS_AL", "TK_CHIRAL_CLASS_SP",
  "TK_CHIRAL_CLASS_TB", "TK_CHIRAL_CLASS_OH", "'('", "')'", "'['", "']'",
  "'+'", "'-'", "'.'", "':'", "'='", "'#'", "'/'", "'\\\\'", "'@'",
  "$accept", "smiles", "expression", "node", "atom", "opt_hydrogens",
  "org_subset_atom_symbol", "atom_symbol", "opt_charge", "pos_charge",
  "neg_charge", "opt_connection_list", "connection_list", "opt_isotope",
  "isotope", "bond", "opt_chirality", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,    40,    41,    91,    93,    43,    45,
      46,    58,    61,    35,    47,    92,    64
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    27,    28,    29,    29,    29,    30,    30,    30,    31,
      31,    31,    32,    32,    32,    33,    34,    34,    35,    35,
      35,    36,    36,    36,    36,    37,    37,    37,    37,    38,
      38,    39,    39,    40,    40,    41,    41,    42,    42,    42,
      42,    42,    42,    42,    43,    43,    43,    43,    43,    43,
      43,    43
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     3,     2,     1,     4,     5,     2,
       8,     5,     0,     1,     2,     1,     1,     1,     0,     1,
       1,     1,     2,     3,     2,     1,     2,     3,     2,     0,
       1,     1,     2,     0,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     2,     2,     2,     2,
       2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,    15,    33,     0,     2,     3,     6,    29,    35,     0,
      34,     1,     0,    38,    37,    39,    40,    41,    42,    43,
       5,     0,    31,     9,    30,    17,    18,    16,    44,    36,
       0,     0,     4,    32,    21,    25,     0,    19,    20,     0,
       0,     0,     0,     0,    45,    12,     7,     0,    24,    22,
      28,    26,    11,    47,    48,    50,    49,    51,    46,    13,
      18,     8,    23,    27,    14,     0,    29,    10
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     3,     4,     5,     6,    60,     7,    28,    36,    37,
      38,    23,    24,     9,    10,    21,    45
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -15
static const yysigned_char yypact[] =
{
      -1,   -15,     4,    27,   -15,    -2,   -15,    34,   -15,     1,
      36,   -15,     9,   -15,   -15,   -15,   -15,   -15,   -15,   -15,
     -15,    -1,   -15,   -15,    37,   -15,   -10,   -15,    26,   -15,
     -14,    -1,   -15,   -15,    35,     5,    28,   -15,   -15,    39,
      40,    38,    42,    43,    24,    46,   -15,    41,   -15,    44,
     -15,    45,   -15,   -15,   -15,   -15,   -15,   -15,   -15,    48,
     -10,   -15,   -15,   -15,   -15,    49,    34,   -15
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -15,   -15,    -5,   -15,   -15,   -15,    50,   -15,    -3,   -15,
     -15,   -11,   -15,   -15,   -15,    51,   -15
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      20,    46,     1,     1,    25,     1,    26,    30,    34,    35,
       8,    50,    12,     1,     2,     2,    32,    13,    14,    15,
      16,    17,    18,    19,    51,     2,    47,    11,    13,    14,
      15,    16,    17,    18,    19,    39,    40,    41,    42,    43,
      22,    48,    29,    33,    55,    52,    53,    54,    56,    57,
      58,    59,    44,    49,    64,    67,    61,    65,     0,    27,
       0,     0,    62,    31,    63,     0,    66
};

static const yysigned_char yycheck[] =
{
       5,    15,     4,     4,     3,     4,     5,    12,    18,    19,
       6,     6,    14,     4,    16,    16,    21,    19,    20,    21,
      22,    23,    24,    25,    19,    16,    31,     0,    19,    20,
      21,    22,    23,    24,    25,     9,    10,    11,    12,    13,
       6,     6,     6,     6,     6,    17,     7,     7,     6,     6,
      26,     5,    26,    18,     6,    66,    15,    60,    -1,     9,
      -1,    -1,    18,    12,    19,    -1,    17
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     4,    16,    28,    29,    30,    31,    33,     6,    40,
      41,     0,    14,    19,    20,    21,    22,    23,    24,    25,
      29,    42,     6,    38,    39,     3,     5,    33,    34,     6,
      29,    42,    29,     6,    18,    19,    35,    36,    37,     9,
      10,    11,    12,    13,    26,    43,    15,    29,     6,    18,
       6,    19,    17,     7,     7,     6,     6,     6,    26,     5,
      32,    15,    18,    19,     6,    35,    17,    38
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
#line 68 "smilesParserParser.y"
    { 
		}
    break;

  case 3:
#line 73 "smilesParserParser.y"
    { (yyval.atom) = (yyvsp[0].atom); }
    break;

  case 4:
#line 74 "smilesParserParser.y"
    { new SmilesParser::SPBond((yyvsp[-2].atom), (yyvsp[0].atom), (yyvsp[-1].number)); (yyval.atom) = (yyvsp[-2].atom); }
    break;

  case 5:
#line 75 "smilesParserParser.y"
    { new SmilesParser::SPBond((yyvsp[-1].atom), (yyvsp[0].atom)); (yyval.atom) = (yyvsp[-1].atom); }
    break;

  case 6:
#line 79 "smilesParserParser.y"
    { (yyval.atom) = (yyvsp[0].atom); }
    break;

  case 7:
#line 80 "smilesParserParser.y"
    { new SmilesParser::SPBond((yyvsp[-1].atom), (yyvsp[-3].atom), 1); (yyval.atom) = (yyvsp[-3].atom); }
    break;

  case 8:
#line 81 "smilesParserParser.y"
    { new SmilesParser::SPBond((yyvsp[-1].atom), (yyvsp[-4].atom), (yyvsp[-2].number)); (yyval.atom) = (yyvsp[-4].atom); }
    break;

  case 9:
#line 86 "smilesParserParser.y"
    {
			(yyval.atom) = (yyvsp[-1].atom); 
			if ((yyvsp[0].conns) != 0) 
			{ 
				SmilesParser::state.current_parser->createBonds((yyvsp[-1].atom), (yyvsp[0].conns)); 
			} 
		}
    break;

  case 10:
#line 94 "smilesParserParser.y"
    { 
			(yyvsp[-5].atom)->setFormalCharge((yyvsp[-2].number)); 
			(yyvsp[-5].atom)->setIsotope((yyvsp[-6].number));
			(yyvsp[-5].atom)->setInBrackets(true);
			if ((yyvsp[-3].number) != 0)
			{
				for (int i = 0; i < (yyvsp[-3].number); i++)
				{
					new SmilesParser::SPBond(SmilesParser::state.current_parser->createAtom("H"), (yyvsp[-5].atom));
				}
			}
			if ((yyvsp[-4].chiral) != 0)
			{	
				(yyvsp[-5].atom)->setChirality(*(yyvsp[-4].chiral));
				delete (yyvsp[-4].chiral);
			}
			if ((yyvsp[0].conns) != 0) 
			{ 
				SmilesParser::state.current_parser->createBonds((yyvsp[-5].atom), (yyvsp[0].conns)); 
				delete  (yyvsp[0].conns);
			} 
			(yyval.atom) = (yyvsp[-5].atom); 
		}
    break;

  case 11:
#line 118 "smilesParserParser.y"
    { 
			(yyval.atom) = SmilesParser::state.current_parser->createAtom("H"); 
			(yyval.atom)->setFormalCharge((yyvsp[-1].number)); 
			(yyval.atom)->setIsotope((yyvsp[-3].number));
			(yyval.atom)->setInBrackets(true); 
		}
    break;

  case 12:
#line 127 "smilesParserParser.y"
    { (yyval.number) = 0; }
    break;

  case 13:
#line 129 "smilesParserParser.y"
    {
			(yyval.number) = 1;
		}
    break;

  case 14:
#line 133 "smilesParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 15:
#line 139 "smilesParserParser.y"
    { (yyval.atom) = SmilesParser::state.current_parser->createAtom(yylval.text); }
    break;

  case 16:
#line 143 "smilesParserParser.y"
    { (yyval.atom) = (yyvsp[0].atom); }
    break;

  case 17:
#line 144 "smilesParserParser.y"
    { (yyval.atom) = SmilesParser::state.current_parser->createAtom(yylval.text); }
    break;

  case 18:
#line 148 "smilesParserParser.y"
    { 
			(yyval.number) = 0; 
		}
    break;

  case 19:
#line 152 "smilesParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 20:
#line 156 "smilesParserParser.y"
    {
			(yyval.number) = (yyvsp[0].number);
		}
    break;

  case 21:
#line 162 "smilesParserParser.y"
    { (yyval.number) = 1; }
    break;

  case 22:
#line 163 "smilesParserParser.y"
    { (yyval.number) = 2; }
    break;

  case 23:
#line 164 "smilesParserParser.y"
    { (yyval.number) = 3; }
    break;

  case 24:
#line 165 "smilesParserParser.y"
    { (yyval.number) = (yyvsp[0].number); }
    break;

  case 25:
#line 169 "smilesParserParser.y"
    {	(yyval.number) = -1; }
    break;

  case 26:
#line 170 "smilesParserParser.y"
    { (yyval.number) = -2; }
    break;

  case 27:
#line 171 "smilesParserParser.y"
    { (yyval.number) = -3; }
    break;

  case 28:
#line 172 "smilesParserParser.y"
    { (yyval.number) = -(yyvsp[0].number); }
    break;

  case 29:
#line 176 "smilesParserParser.y"
    {	(yyval.conns) = 0; }
    break;

  case 30:
#line 178 "smilesParserParser.y"
    {	
			(yyval.conns) = (yyvsp[0].conns); 
		}
    break;

  case 31:
#line 185 "smilesParserParser.y"
    {
			(yyval.conns) = new SmilesParser::ConnectionList;
			(yyval.conns)->push_back((yyvsp[0].number));
		}
    break;

  case 32:
#line 190 "smilesParserParser.y"
    {
			(yyval.conns) = (yyvsp[-1].conns);
			(yyval.conns)->push_back((yyvsp[0].number));
		}
    break;

  case 33:
#line 197 "smilesParserParser.y"
    { (yyval.number) = 0; }
    break;

  case 34:
#line 198 "smilesParserParser.y"
    { (yyval.number) = (yyvsp[0].number); }
    break;

  case 35:
#line 203 "smilesParserParser.y"
    { (yyval.number) = (yyvsp[0].number);	}
    break;

  case 36:
#line 204 "smilesParserParser.y"
    { (yyval.number) = 10 * (yyvsp[-1].number) + (yyvsp[0].number); }
    break;

  case 37:
#line 208 "smilesParserParser.y"
    { (yyval.number) = 0; }
    break;

  case 38:
#line 209 "smilesParserParser.y"
    { (yyval.number) = 1; }
    break;

  case 39:
#line 210 "smilesParserParser.y"
    { (yyval.number) = 1; }
    break;

  case 40:
#line 211 "smilesParserParser.y"
    { (yyval.number) = 2; }
    break;

  case 41:
#line 212 "smilesParserParser.y"
    { (yyval.number) = 3; }
    break;

  case 42:
#line 213 "smilesParserParser.y"
    { (yyval.number) = 1; }
    break;

  case 43:
#line 214 "smilesParserParser.y"
    { (yyval.number) = 1; }
    break;

  case 44:
#line 218 "smilesParserParser.y"
    { (yyval.chiral) = 0; }
    break;

  case 45:
#line 220 "smilesParserParser.y"
    { 
			(yyval.chiral) = new SmilesParser::ChiralDef(SmilesParser::TH, 1); 
		}
    break;

  case 46:
#line 224 "smilesParserParser.y"
    { 
			(yyval.chiral) = new SmilesParser::ChiralDef(SmilesParser::TH, 1); 
		}
    break;

  case 47:
#line 228 "smilesParserParser.y"
    {
			(yyval.chiral) = new SmilesParser::ChiralDef(SmilesParser::TH, (yyvsp[0].number)); 
		}
    break;

  case 48:
#line 232 "smilesParserParser.y"
    {
			(yyval.chiral) = new SmilesParser::ChiralDef(SmilesParser::AL, (yyvsp[0].number)); 
		}
    break;

  case 49:
#line 236 "smilesParserParser.y"
    {
			(yyval.chiral) = new SmilesParser::ChiralDef(SmilesParser::TB, (yyvsp[0].number)); 
		}
    break;

  case 50:
#line 240 "smilesParserParser.y"
    {
			(yyval.chiral) = new SmilesParser::ChiralDef(SmilesParser::TB, (yyvsp[0].number)); 
		}
    break;

  case 51:
#line 244 "smilesParserParser.y"
    {
			(yyval.chiral) = new SmilesParser::ChiralDef(SmilesParser::OH, (yyvsp[0].number)); 
		}
    break;


    }

/* Line 1037 of yacc.c.  */
#line 1433 "y.tab.c"

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


#line 249 "smilesParserParser.y"


void yyerror(char* s)
{
	throw Exception::ParseError(__FILE__, 0, 
															SmilesParser::state.buffer, 
															String(s) + String(" (at position ") 
																+ String(SmilesParser::state.char_count) + String(")"));
}


