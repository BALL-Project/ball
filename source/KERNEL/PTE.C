// $Id: PTE.C,v 1.6 2000/12/10 20:09:43 oliver Exp $

#include <BALL/KERNEL/PTE.h>

#include <ctype.h>			// toupper, tolower
#include <stdlib.h>			// bsearch
#include <string.h>

#include <iostream>

using namespace std;

namespace BALL 
{

	extern "C" int PTEcompare_
		(const void* a_ptr, const void* b_ptr)
	{
		return strcmp(((PTE_::SymbolToElement*)a_ptr)->symbol,
									((PTE_::SymbolToElement*)b_ptr)->symbol);
	}


	Element Element::UNKNOWN
						("Unknown",                  "?",         0,    0,       0,       0.0,       0.0,   0.0,  0.0,  0.0);



	Element PTE_::element_[] =
	{
		//Element      NAME              SYMBOL       GROUP  PERIOD NUMBER   WEIGHT      ATOMIC COV.  VDW   EN    
		Element("Actinium",                "Ac",				 3,    7,      89,     227.0278,    1.88,  0.0,  0.0,  1.3),
		Element("Aluminum",                "Al",        13,    3,      13,      26.981539,  1.43,  1.25, 2.05, 1.61),
		Element("Americium",               "Am",				 0,    7,      95,     243.0614,    1.73,  0.0,  0.0,  1.3), 
		Element("Antimony",								 "Sb",        15,    5,      51,     121.76,      1.82,  1.41, 2.2,  2.05),
		Element("Argon",                   "Ar",        18,    3,      18,      39.948,     1.74,  0.0,  1.91, 0.0),
		Element("Arsenic",                 "As",        15,    4,      33,      74.92159,   1.25,  1.21, 2.0,  2.18),
		Element("Astatine",                "At",				17,    6,      85,     209.9871,    0.0,   0.0,  0.0,  1.96), 
		Element("Barium",                  "Ba",         2,    6,      56,     137.327,     2.17,  1.98, 0.0,  0.89),
		Element("Berkelium",               "Bk",				 0,    7,      97,     247.0703,    1.70,  0.0,  0.0,  1.3), 
		Element("Beryllium",               "Be",         2,    2,       4,       9.012182,  1.13,  0.89, 0.0,  1.57),
		Element("Bismuth",								 "Bi",        15,    6,      83,     208.98037,   1.55,  1.52, 2.4,  2.0),
		Element("Bohrium",                 "Bh",				 7,    7,     107,     262.12,      0.0,   0.0,  0.0,  0.0),
		Element("Boron",                   "B",         13,    2,       5,      10.811,     0.83,  0.88, 2.08, 2.04),
		Element("Bromine",                 "Br",        17,    4,      35,      79.904,     0.0,   1.14, 1.95, 2.96),
		Element("Cadmium",                 "Cd",        12,    5,      48,     112.411,     1.49,  1.41, 0.0,  1.69),
		Element("Caesium",                 "Cs",         1,    6,      55,     132.90543,   2.654, 2.35, 2.62, 0.79),
		Element("Calcium",                 "Ca",         2,    4,      20,      40.078,     1.97,  1.74, 0.0,  1.0),
		Element("Californium",             "Cf",				 0,    7,      98,     251.0796,    1.69,  0.0,  0.0,  1.3), 
		Element("Carbon",									 "C",         14,    2,       6,      12.011,     0.77,  0.77, 1.85, 2.55),
		Element("Cerium",                  "Ce",         0,    6,      58,     140.115,     1.825, 1.65, 0.0,  1.12),
		Element("Chlorine",                "Cl",        17,    3,      17,      35.4527,    0.0,   0.99, 1.81, 3.16),
		Element("Chromium",                "Cr",         6,    4,      24,      51.9961,    1.25,  0.0,  0.0,  1.66),
		Element("Cobalt",                  "Co",         9,    4,      27,      58.9332,    1.25,  1.16, 0.0,  1.88),
		Element("Copper",									 "Cu",        11,    4,      29,      63.546,     1.28,  1.17, 0.0,  1.9),
		Element("Curium",                  "Cm",				 0,    7,      96,     247.0703,    1.74,  0.0,  0.0,  1.3), 
		Element("Dubnium",                 "Db",				 4,    7,     104,     261.11,      0.0,   0.0,  0.0,  0.0),
		Element("Dysprosium",              "Dy",         0,    6,      66,     162.5,       1.77,  1.59, 0.0,  1.23),
		Element("Einsteinium",             "Es",				 0,    7,      99,     252.083,     2.03,  0.0,  0.0,  1.3), 
		Element("Erbium",                  "Er",         0,    6,      68,     167.26,      1.76,  1.57, 0.0,  1.25),
		Element("Europium",                "Eu",         0,    6,      63,     151.965,     2.04,  1.85, 0.0,  1.2),
		Element("Fermium",                 "Fm",				 0,    7,     100,     257.0951,    0.0,   0.0,  0.0,  1.3), 
		Element("Fluorine",                "F",         17,    2,       9,      18.9984032, 0.709, 0.58, 1.35, 3.98),
		Element("Francium",                "Fr",				 1,    7,      87,     223.0197,    2.7,   0.0,  0.0,  0.7), 
		Element("Gadolinium",              "Gd",         0,    6,      64,     157.25,      1.8,   1.61, 0.0,  0.94),
		Element("Gallium",                 "Ga",        13,    4,      31,      69.723,     1.22,  1.25, 0.0,  1.81),
		Element("Germanium",               "Ge",        14,    4,      32,      72.61,      1.23,  1.22, 0.0,  2.01),
		Element("Gold"/*Aurum*/,           "Au",        11,    6,      79,     196.96654,   1.44,  1.34, 0.0,  2.0),
		Element("Hafnium",                 "Hf",         4,    6,      72,     178.49,      1.56,  1.44, 0.0,  1.5),
		Element("Hahnium",                 "Hn",				 8,    7,     108,       0.0,       0.0,   0.0,  0.0,  0.0),
		Element("Helium",                  "He",        18,    1,       2,       4.002602,  1.28,  0.0,  1.22, 0.0),
		Element("Holmium",                 "Ho",         0,    6,      67,     164.93032,   1.77,  1.58, 0.0,  1.24),
		Element("Hydrogen"/*Hydrogenium*/, "H",          1,    1,       1,       1.00797,   0.78,  0.3,  1.2,  2.2),
		Element("Indium",                  "In",        13,    5,      49,     114.818,     1.63,  1.5,  0.0,  1.78),
		Element("Iodine"/*Jod*/,           "I",         17,    5,      53,     126.90447,   0.0,   1.33, 2.15, 2.66),
		Element("Iridium",                 "Ir",         9,    6,      77,     192.217,     1.36,  1.26, 0.0,  2.28),
		Element("Iron"/*Ferrum*/,          "Fe",         8,    4,      26,      55.845,     1.24,  1.16, 0.0,  1.83),
		Element("Joliotium",               "Jl"/*262*/,  5,    7,     105,     262.114,     0.0,   0.0,  0.0,  0.0),
		Element("Krypton",                 "Kr",        18,    4,      36,      83.80,      0.0,   1.89, 1.98, 0.0),
		Element("Lanthanum",               "La",         3,    6,      57,     138.9055,    1.88,  1.69, 0.0,  1.1),
		Element("Lawrencium",              "Lr"/*262*/,  3,    7,     103,     262.11,      0.0,   0.0,  0.0,  0.0), 
		Element("Lead"/*Plumbum*/,         "Pb",        14,    6,      82,     207.2,       1.75,  1.54, 0.0,  2.02),
		Element("Lithium",                 "Li",         1,    2,       3,       6.941,     1.52,  1.23, 0.0,  0.98),
		Element("Lutetium",                "Lu",         3,    6,      71,     174.967,     1.72,  1.56, 0.0,  1.3),
		Element("Magnesium",               "Mg",         2,    3,      12,      24.30506,   1.6,   1.36, 0.0,  1.31),
		Element("Manganese"/*Mangan*/,     "Mn",         7,    4,      25,      54.93805,   1.24,  1.77, 0.0,  1.55),
		Element("Meitnerium",              "Mt"/*266*/,  9,    7,     109,       0.0,       0.0,   0.0,  0.0,  0.0),
		Element("Mendelevium",             "Md"/*258*/,  0,    7,     101,     258.1,       0.0,   0.0,  0.0,  1.3), 
		Element("Mercury"/*Hydrargyrum*/,  "Hg",        12,    6,      80,     200.59,      1.60,  1.44, 0.0,  1.8),
		Element("Molybdenum",              "Mo",         6,    5,      42,      95.94,      1.36,  1.29, 0.0,  2.16),
		Element("Neodymium",               "Nd",         0,    6,      60,     144.24,      1.82,  1.64, 0.0,  1.14),
		Element("Neon",                    "Ne",        18,    2,      10,      20.1797,    0.0,   0.0,  1.6,  0.0),
		Element("Neptunium",               "Np"/*237*/,  0,    7,      93,     237.0482,    1.5,   0.0,  0.0,  1.28), 
		Element("Nickel",                  "Ni",        10,    4,      28,      58.6934,    1.25,  1.15, 0.0,  1.91),
		Element("Niobium",                 "Nb",         5,    5,      41,      92.90638,   1.43,  1.34, 0.0,  1.6),
		Element("Nitrogen"/*Nitrogenium*/, "N",         15,    2,       7,      14.00674,   0.71,  0.7,  1.54, 3.04),
		Element("Nobelium",                "No"/*259*/,  0,    7,     102,     259.1009,    0.0,   0.0,  0.0,  0.0), 
		Element("Osmium",                  "Os",         8,    6,      76,     190.23,      1.35,  1.26, 0.0,  2.2),
		Element("Oxygen"/*Oxygenium*/,     "O",         16,    2,       8,      15.9994,    0.0,   0.66, 1.4,  3.44),
		Element("Palladium",               "Pd",        10,    5,      46,     106.42,      1.38,  1.28, 0.0,  2.2),
		Element("Phosphorus",              "P",         15,    3,      15,      30.973762,  1.15,  1.10, 1.9,  2.19),
		Element("Platinum",                "Pt",        10,    6,      78,     195.08,      1.38,  1.29, 0.0,  2.54),
		Element("Plutonium",               "Pu"/*244*/,  7,    0,      94,     244.0642,    0.0,   0.0,  0.0,  1.3),
		Element("Polonium",                "Po"/*209*/, 16,    6,      84,     208.9824,    1.67,  1.53, 0.0,  2.2), 
		Element("Potassium"/*Kalium*/,     "K",          1,    4,      19,      39.0983,    2.27,  2.03, 2.31, 0.82),
		Element("Praseodymium",            "Pr",         0,    6,      59,     140.90765,   1.83,  1.65, 0.0,  1.13),
		Element("Promethium",              "Pm"/*145*/,  0,    6,      61,     144.9127,    1.81,  0.0,  0.0,  0.94), 
		Element("Protactinium",            "Pa",         0,    7,      91,     231.03588,   1.61,  0.0,  0.0,  1.38),
		Element("Radium",                  "Ra"/*226*/,  2,    7,      88,     226.0254,    2.23,  0.0,  0.0,  0.89), 
		Element("Radon",                   "Rn"/*222*/, 18,    6,      86,     222.0176,    0.0,   0.0,  0.0,  0.7), 
		Element("Rhenium",                 "Re",         7,    6,      75,     186.207,     1.37,  1.28, 0.0,  2.2),
		Element("Rhodium",                 "Rh",         9,    5,      45,     102.9055,    1.34,  1.25, 0.0,  2.28),
		Element("Rubidium",                "Rb",         1,    5,      37,      85.4678,    1.475, 0.0,  2.44, 0.82),
		Element("Ruthenium",               "Ru",         8,    5,      44,     101.07,      1.34,  1.24, 0.0,  2.2),
		Element("Rutherfordium",           "Rf"/*263*/,  6,    7,     106,     263.118,     0.0,   0.0,  0.0,  0.0),
		Element("Samarium",                "Sm",         0,    6,      62,     150.36,      1.8,   1.66, 0.0,  1.17),
		Element("Scandium",                "Sc",         3,    4,      21,      44.95591,   1.61,  1.44, 0.0,  1.36),
		Element("Selenium",                "Se",        16,    4,      34,      78.96,      2.15,  1.17, 2.0,  2.55),
		Element("Silicon"/*Silicium*/,     "Si",        14,    3,      14,      28.0855,    1.17,  1.17, 2.0,  1.9),
		Element("Silver"/*Argentum*/,      "Ag",        11,    5,      47,     107.8682,    1.44,  1.34, 0.0,  1.93),
		Element("Sodium"/*Natrium*/,       "Na",         1,    3,      11,      22.989768,  1.54,  0.0,  2.31, 0.93),
		Element("Strontium",               "Sr",         2,    5,      38,      87.62,      2.15,  1.92, 0.0,  0.95),
		Element("Sulphur"/*Sulfur*/,       "S",         16,    3,      16,      32.066,     1.04,  1.04, 1.85, 2.58),
		Element("Tantalum",                "Ta",         5,    6,      73,     180.9479,    1.43,  1.34, 0.0,  2.36),
		Element("Technetium",              "Tc",         7,    5,      43,      98.9072,    1.36,  0.0,  0.0,  1.9), 
		Element("Tellurium",               "Te",        16,    5,      52,     127.6,       1.43,  1.37, 2.2,  2.1), 
		Element("Terbium",                 "Tb",         0,    6,      65,     158.92534,   1.78,  1.59, 0.0,  1.22),
		Element("Thallium",                "Tl",        13,    6,      81,     204.3833,    1.7,   1.55, 0.0,  2.33),
		Element("Thorium",                 "Th",         0,    7,      90,     232.0381,    1.80,  0.0,  0.0,  0.0),
		Element("Thulium",                 "Tm",         0,    6,      69,     168.93421,   1.75,  1.56, 0.0,  0.96),
		Element("Tin"/*Stannum*/,          "Sn",        14,    5,      50,     118.71,      1.41,  1.4,  2.0,  1.96), 
		Element("Titanium",                "Ti",         4,    4,      22,      47.867,     1.45,  1.32, 0.0,  1.54),
		Element("Tungsten"/*Wolfram*/,     "W",          6,    6,      74,     183.84,      1.37,  1.3,  0.0,  1.9),
		Element("Ununbium",                "Uub",       12,    7,     112,       0.0,       0.0,   0.0,  0.0,  0.0),
		Element("Ununnilium",              "Uun",       10,    7,     110,       0.0,       0.0,   0.0,  0.0,  0.0),
		Element("Ununninium",              "Uuu",       11,    7,     111,       0.0,       0.0,   0.0,  0.0,  0.0),
		Element("Uranium",                 "U",          0,    7,      92,     238.0289,    1.54,  0.0,  0.0,  1.26),
		Element("Vanadium",                "V",          5,    4,      23,      50.9415,    1.32,  0.0,  0.0,  1.63),
		Element("Xenon",                   "Xe",        18,    5,      54,     131.29,      2.18,  2.09, 2.16, 2.6),
		Element("Ytterbium",               "Yb",         0,    6,      70,     173.04,      1.94,  1.7,  0.0,  1.27),
		Element("Yttrium",                 "Y",          3,    5,      39,      88.90585,   1.81,  1.62, 0.0,  1.22),
		Element("Zinc"/*Zincum*/,          "Zn",        12,    4,      30,      65.39,      1.33,  1.25, 0.0,  1.65),
		Element("Zirconium",               "Zr",         4,    5,      40,      91.224,     1.6,   1.45, 0.0,  1.3)
	};

	PTE_::SymbolToElement PTE_::symbol_to_element_[] = 
	{
		{"AC",  &PTE_::element_[Element::ACTINIUM]},
		{"AG",  &PTE_::element_[Element::SILVER]},
		{"AL",  &PTE_::element_[Element::ALUMINUM]},
		{"AM",  &PTE_::element_[Element::AMERICIUM]},
		{"AR",  &PTE_::element_[Element::ARGON]},
		{"AS",  &PTE_::element_[Element::ARSENIC]},
		{"AT",  &PTE_::element_[Element::ASTATINE]},
		{"AU",  &PTE_::element_[Element::GOLD]},
		{"B",   &PTE_::element_[Element::BORON]},
		{"BA",  &PTE_::element_[Element::BARIUM]},
		{"BE",  &PTE_::element_[Element::BERYLLIUM]},
		{"BH",  &PTE_::element_[Element::BOHRIUM]},
		{"BI",  &PTE_::element_[Element::BISMUTH]},
		{"BK",  &PTE_::element_[Element::BERKELIUM]},
		{"BR",  &PTE_::element_[Element::BROMINE]},
		{"C",   &PTE_::element_[Element::CARBON]},
		{"CA",  &PTE_::element_[Element::CALCIUM]},
		{"CD",  &PTE_::element_[Element::CADMIUM]},
		{"CE",  &PTE_::element_[Element::CERIUM]},
		{"CF",  &PTE_::element_[Element::CALIFORNIUM]},
		{"CL",  &PTE_::element_[Element::CHLORINE]},
		{"CM",  &PTE_::element_[Element::CURIUM]},
		{"CO",  &PTE_::element_[Element::COBALT]},
		{"CR",  &PTE_::element_[Element::CHROMIUM]},
		{"CS",  &PTE_::element_[Element::CAESIUM]},
		{"CU",  &PTE_::element_[Element::COPPER]},
		{"DB",  &PTE_::element_[Element::DUBNIUM]},
		{"DY",  &PTE_::element_[Element::DYSPROSIUM]},
		{"ER",  &PTE_::element_[Element::ERBIUM]},
		{"ES",  &PTE_::element_[Element::EINSTEINIUM]},
		{"EU",  &PTE_::element_[Element::EUROPIUM]},
		{"F",   &PTE_::element_[Element::FLUORINE]},
		{"FE",  &PTE_::element_[Element::IRON]},
		{"FM",  &PTE_::element_[Element::FERMIUM]},
		{"FR",  &PTE_::element_[Element::FRANCIUM]},
		{"GA",  &PTE_::element_[Element::GALLIUM]},
		{"GD",  &PTE_::element_[Element::GADOLINIUM]},
		{"GE",  &PTE_::element_[Element::GERMANIUM]},
		{"H",   &PTE_::element_[Element::HYDROGEN]},
		{"HE",  &PTE_::element_[Element::HELIUM]},
		{"HF",  &PTE_::element_[Element::HAFNIUM]},
		{"HG",  &PTE_::element_[Element::MERCURY]},
		{"HN",  &PTE_::element_[Element::HAHNIUM]},
		{"HO",  &PTE_::element_[Element::HOLMIUM]},
		{"I",   &PTE_::element_[Element::IODINE]},
		{"IN",  &PTE_::element_[Element::INDIUM]},
		{"IR",  &PTE_::element_[Element::IRIDIUM]},
		{"JL",  &PTE_::element_[Element::JOLIOTIUM]},
		{"K",   &PTE_::element_[Element::POTASSIUM]},
		{"KR",  &PTE_::element_[Element::KRYPTON]},
		{"LA",  &PTE_::element_[Element::LANTHANUM]},
		{"LI",  &PTE_::element_[Element::LITHIUM]},
		{"LR",  &PTE_::element_[Element::LAWRENCIUM]},
		{"LU",  &PTE_::element_[Element::LUTETIUM]},
		{"MD",  &PTE_::element_[Element::MENDELEVIUM]},
		{"MG",  &PTE_::element_[Element::MAGNESIUM]},
		{"MN",  &PTE_::element_[Element::MANGANESE]},
		{"MO",  &PTE_::element_[Element::MOLYBDENUM]},
		{"MT",  &PTE_::element_[Element::MEITNERIUM]},
		{"N",   &PTE_::element_[Element::NITROGEN]},
		{"NA",  &PTE_::element_[Element::SODIUM]},
		{"NB",  &PTE_::element_[Element::NIOBIUM]},
		{"ND",  &PTE_::element_[Element::NEODYMIUM]},
		{"NE",  &PTE_::element_[Element::NEON]},
		{"NI",  &PTE_::element_[Element::NICKEL]},
		{"NO",  &PTE_::element_[Element::NOBELIUM]},
		{"NP",  &PTE_::element_[Element::NEPTUNIUM]},
		{"O",   &PTE_::element_[Element::OXYGEN]},
		{"OS",  &PTE_::element_[Element::OSMIUM]},
		{"P",   &PTE_::element_[Element::PHOSPHORUS]},
		{"PA",  &PTE_::element_[Element::PROTACTINIUM]},
		{"PB",  &PTE_::element_[Element::LEAD]},
		{"PD",  &PTE_::element_[Element::PALLADIUM]},
		{"PM",  &PTE_::element_[Element::PROMETHIUM]},
		{"PO",  &PTE_::element_[Element::POLONIUM]},
		{"PR",  &PTE_::element_[Element::PRASEODYMIUM]},
		{"PT",  &PTE_::element_[Element::PLATINUM]},
		{"PU",  &PTE_::element_[Element::PLUTONIUM]},
		{"RA",  &PTE_::element_[Element::RADIUM]},
		{"RB",  &PTE_::element_[Element::RUBIDIUM]},
		{"RE",  &PTE_::element_[Element::RHENIUM]},
		{"RF",  &PTE_::element_[Element::RUTHERFORDIUM]},
		{"RH",  &PTE_::element_[Element::RHODIUM]},
		{"RN",  &PTE_::element_[Element::RADON]},
		{"RU",  &PTE_::element_[Element::RUTHENIUM]},
		{"S",   &PTE_::element_[Element::SULPHUR]},
		{"SB",  &PTE_::element_[Element::ANTIMONY]},
		{"SC",  &PTE_::element_[Element::SCANDIUM]},
		{"SE",  &PTE_::element_[Element::SELENIUM]},
		{"SI",  &PTE_::element_[Element::SILICON]},
		{"SM",  &PTE_::element_[Element::SAMARIUM]},
		{"SN",  &PTE_::element_[Element::TIN]},
		{"SR",  &PTE_::element_[Element::STRONTIUM]},
		{"TA",  &PTE_::element_[Element::TANTALUM]},
		{"TB",  &PTE_::element_[Element::TERBIUM]},
		{"TC",  &PTE_::element_[Element::TECHNETIUM]},
		{"TE",  &PTE_::element_[Element::TELLURIUM]},
		{"TH",  &PTE_::element_[Element::THORIUM]},
		{"TI",  &PTE_::element_[Element::TITANIUM]},
		{"TL",  &PTE_::element_[Element::THALLIUM]},
		{"TM",  &PTE_::element_[Element::THULIUM]},
		{"U",   &PTE_::element_[Element::URANIUM]},
		{"UUB", &PTE_::element_[Element::UNUNBIUM]},
		{"UUN", &PTE_::element_[Element::UNUNNILIUM]},
		{"UUU", &PTE_::element_[Element::UNUNNINIUM]},
		{"V",   &PTE_::element_[Element::VANADIUM]},
		{"W",   &PTE_::element_[Element::TUNGSTEN]},
		{"XE",  &PTE_::element_[Element::XENON]},
		{"Y",   &PTE_::element_[Element::YTTRIUM]},
		{"YB",  &PTE_::element_[Element::YTTERBIUM]},
		{"ZN",  &PTE_::element_[Element::ZINC]},
		{"ZR",  &PTE_::element_[Element::ZIRCONIUM]}
	};

	Element *PTE_::atomic_number_to_element_[] = 
	{
		&PTE_::element_[Element::HYDROGEN],
		&PTE_::element_[Element::HELIUM],
		&PTE_::element_[Element::LITHIUM],
		&PTE_::element_[Element::BERYLLIUM],
		&PTE_::element_[Element::BORON],
		&PTE_::element_[Element::CARBON],
		&PTE_::element_[Element::NITROGEN],
		&PTE_::element_[Element::OXYGEN],
		&PTE_::element_[Element::FLUORINE],
		&PTE_::element_[Element::NEON],
		&PTE_::element_[Element::SODIUM],
		&PTE_::element_[Element::MAGNESIUM],
		&PTE_::element_[Element::ALUMINUM],
		&PTE_::element_[Element::SILICON],
		&PTE_::element_[Element::PHOSPHORUS],
		&PTE_::element_[Element::SULPHUR],
		&PTE_::element_[Element::CHLORINE],
		&PTE_::element_[Element::ARGON],
		&PTE_::element_[Element::POTASSIUM],
		&PTE_::element_[Element::CALCIUM],
		&PTE_::element_[Element::SCANDIUM],
		&PTE_::element_[Element::TITANIUM],
		&PTE_::element_[Element::VANADIUM],
		&PTE_::element_[Element::CHROMIUM],
		&PTE_::element_[Element::MANGANESE],
		&PTE_::element_[Element::IRON],
		&PTE_::element_[Element::COBALT],
		&PTE_::element_[Element::NICKEL],
		&PTE_::element_[Element::COPPER],
		&PTE_::element_[Element::ZINC],
		&PTE_::element_[Element::GALLIUM],
		&PTE_::element_[Element::GERMANIUM],
		&PTE_::element_[Element::ARSENIC],
		&PTE_::element_[Element::SELENIUM],
		&PTE_::element_[Element::BROMINE],
		&PTE_::element_[Element::KRYPTON],
		&PTE_::element_[Element::RUBIDIUM],
		&PTE_::element_[Element::STRONTIUM],
		&PTE_::element_[Element::YTTRIUM],
		&PTE_::element_[Element::ZIRCONIUM],
		&PTE_::element_[Element::NIOBIUM],
		&PTE_::element_[Element::MOLYBDENUM],
		&PTE_::element_[Element::TECHNETIUM],
		&PTE_::element_[Element::RUTHENIUM],
		&PTE_::element_[Element::RHODIUM],
		&PTE_::element_[Element::PALLADIUM],
		&PTE_::element_[Element::SILVER],
		&PTE_::element_[Element::CADMIUM],
		&PTE_::element_[Element::INDIUM],
		&PTE_::element_[Element::TIN],
		&PTE_::element_[Element::ANTIMONY],
		&PTE_::element_[Element::TELLURIUM],
		&PTE_::element_[Element::IODINE],
		&PTE_::element_[Element::XENON],
		&PTE_::element_[Element::CAESIUM],
		&PTE_::element_[Element::BARIUM],
		&PTE_::element_[Element::LANTHANUM],
		&PTE_::element_[Element::CERIUM],
		&PTE_::element_[Element::PRASEODYMIUM],
		&PTE_::element_[Element::NEODYMIUM],
		&PTE_::element_[Element::PROMETHIUM],
		&PTE_::element_[Element::SAMARIUM],
		&PTE_::element_[Element::EUROPIUM],
		&PTE_::element_[Element::GADOLINIUM],
		&PTE_::element_[Element::TERBIUM],
		&PTE_::element_[Element::DYSPROSIUM],
		&PTE_::element_[Element::HOLMIUM],
		&PTE_::element_[Element::ERBIUM],
		&PTE_::element_[Element::THULIUM],
		&PTE_::element_[Element::YTTERBIUM],
		&PTE_::element_[Element::LUTETIUM],
		&PTE_::element_[Element::HAFNIUM],
		&PTE_::element_[Element::TANTALUM],
		&PTE_::element_[Element::TUNGSTEN],
		&PTE_::element_[Element::RHENIUM],
		&PTE_::element_[Element::OSMIUM],
		&PTE_::element_[Element::IRIDIUM],
		&PTE_::element_[Element::PLATINUM],
		&PTE_::element_[Element::GOLD],
		&PTE_::element_[Element::MERCURY],
		&PTE_::element_[Element::THALLIUM],
		&PTE_::element_[Element::LEAD],
		&PTE_::element_[Element::BISMUTH],
		&PTE_::element_[Element::POLONIUM],
		&PTE_::element_[Element::ASTATINE],
		&PTE_::element_[Element::RADON],
		&PTE_::element_[Element::FRANCIUM],
		&PTE_::element_[Element::RADIUM],
		&PTE_::element_[Element::ACTINIUM],
		&PTE_::element_[Element::THORIUM],
		&PTE_::element_[Element::PROTACTINIUM],
		&PTE_::element_[Element::URANIUM],
		&PTE_::element_[Element::NEPTUNIUM],
		&PTE_::element_[Element::PLUTONIUM],
		&PTE_::element_[Element::AMERICIUM],
		&PTE_::element_[Element::CURIUM],
		&PTE_::element_[Element::BERKELIUM],
		&PTE_::element_[Element::CALIFORNIUM],
		&PTE_::element_[Element::EINSTEINIUM],
		&PTE_::element_[Element::FERMIUM],
		&PTE_::element_[Element::MENDELEVIUM],
		&PTE_::element_[Element::NOBELIUM],
		&PTE_::element_[Element::LAWRENCIUM],
		&PTE_::element_[Element::DUBNIUM],
		&PTE_::element_[Element::JOLIOTIUM],
		&PTE_::element_[Element::RUTHERFORDIUM],
		&PTE_::element_[Element::BOHRIUM],
		&PTE_::element_[Element::HAHNIUM],
		&PTE_::element_[Element::MEITNERIUM],
		&PTE_::element_[Element::UNUNNILIUM],
		&PTE_::element_[Element::UNUNNINIUM],
		&PTE_::element_[Element::UNUNBIUM]
	};

	Element::Element()
		:	name_(BALL_ELEMENT_NAME_DEFAULT),
			symbol_(BALL_ELEMENT_SYMBOL_DEFAULT),
			group_(BALL_ELEMENT_GROUP_DEFAULT),
			period_(BALL_ELEMENT_PERIOD_DEFAULT),
			atomic_number_(BALL_ELEMENT_ATOMICNUMBER_DEFAULT),
			atomic_weight_(BALL_ELEMENT_ATOMICWEIGHT_DEFAULT),
			atomic_radius_(BALL_ELEMENT_ATOMICRADIUS_DEFAULT),
			covalent_radius_(BALL_ELEMENT_COVALENTRADIUS_DEFAULT),
			van_der_waals_radius_(BALL_ELEMENT_VANDERWAALSRADIUS_DEFAULT),
			electronegativity_(BALL_ELEMENT_ELECTRONEGATIVITY_DEFAULT)
	{
	}

	Element::Element(const Element& element)
			:	PropertyManager(),
				name_(element.name_),
				symbol_(element.symbol_),
				group_(element.group_),
				period_(element.period_),
				atomic_number_(element.atomic_number_),
				atomic_weight_(element.atomic_weight_),
				atomic_radius_(element.atomic_radius_),
				covalent_radius_(element.covalent_radius_),
				van_der_waals_radius_(element.van_der_waals_radius_),
				electronegativity_(element.electronegativity_)
	{
	}


	Element::Element
		(const String& name,
		 const String& symbol,
		 Element::Group group,
		 Element::Period period,
		 Element::AtomicNumber atomic_number,
		 float atomic_weight,
		 float atomic_radius,
		 float covalent_radius,
		 float van_der_waals_radius,
		 float electronegativity)
		:	name_(name),
			symbol_(symbol),
			group_(group),
			period_(period),
			atomic_number_(atomic_number),
			atomic_weight_(atomic_weight),
			atomic_radius_(atomic_radius),
			covalent_radius_(covalent_radius),
			van_der_waals_radius_(van_der_waals_radius),
			electronegativity_(electronegativity)
	{
	}


	Element::~Element()
	{
	}

	ostream &
	operator <<(ostream& s, const Element& element)
	{
		s	<< element.name_ << ' '
			<< element.symbol_ << ' '
			<< element.group_ << ' '
			<< element.period_ << ' '
			<< element.atomic_number_ << ' '
			<< element.atomic_weight_ << ' '
			<< element.atomic_radius_ << ' '
			<< element.covalent_radius_ << ' '
			<< element.van_der_waals_radius_ << ' '
			<< element.electronegativity_ << ' ';

		return s;
	}



	PTE_::PTE_()
	{
	}

	PTE_::PTE_(const PTE_& /* pte */)
		:	PropertyManager(pte)
	{
		// no nonstatic members to copy
	}

	PTE_::~PTE_()
	{
	}

	Element& PTE_::getElement(Position position)
	{
		return ((position < Element::NUMBER_OF_ELEMENTS)
						 ? element_[position]
						 : Element::UNKNOWN);
	}
 
	Element& PTE_::getElement(const String& symbol)
	{
		if (symbol.isEmpty() == true)
		{
			return Element::UNKNOWN;
		}

		char symbol_buffer[] = { '\0', '\0', '\0', '\0' };
		SymbolToElement compare = { symbol_buffer, 0 };
		
		if (symbol.size() == 1)
		{
			symbol_buffer[0] = toupper(symbol[0]);
		} 
		else if (symbol.size() == 2)
		{
			symbol_buffer[0] = toupper(symbol[0]);
			symbol_buffer[1] = toupper(symbol[1]);
		} 
		else 
		{
			symbol_buffer[0] = toupper(symbol[0]);
			symbol_buffer[1] = toupper(symbol[1]);
			symbol_buffer[2] = toupper(symbol[2]);
		};

		SymbolToElement* result = (SymbolToElement*)::bsearch 
			((const void*)&compare, 
			 (const void*)symbol_to_element_, 
			 Element::NUMBER_OF_ELEMENTS, 
			 sizeof(SymbolToElement), 
			 PTEcompare_);
		
		if (result == 0)
		{
			return Element::UNKNOWN;
		}
		
		return *(result->element);
	}

	bool PTE_::apply(UnaryProcessor<Element>& processor)
	{
		if (processor.start() == false)
			return false;

		Processor::Result result;

		for (register Position position = 0; position < Element::NUMBER_OF_ELEMENTS; ++position)
		{
			result = processor(element_[position]);

			if (result <= Processor::BREAK)
				return (result == Processor::BREAK) ? true : false;
		}

		return processor.finish();
	}

	PTE_ PTE; // not conform to naming convention but fakes operator[] and operator() to be static functions.

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/KERNEL/PTE.iC>
#	endif

} // namespace BALL
