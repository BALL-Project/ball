// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/KERNEL/PTE.h>
#include <cctype>			// toupper, tolower
#include <cstdlib>			// bsearch

using std::ostream;
namespace BALL
{

	extern "C" int
#ifdef BALL_COMPILER_MSVC
	__cdecl
#endif
	PTEcompare_(const void* a_ptr, const void* b_ptr)
	{
		return strcmp(((PTE_::SymbolToElement*)a_ptr)->symbol,
									((PTE_::SymbolToElement*)b_ptr)->symbol);
	}


	Element Element::UNKNOWN
						("Unknown",                  "?",         0,    0,       0,       0.0,       0.0,   0.0,  0.0, false, 0.0);


	Element PTE_::element_[] =
	{
	//Element    NAME                   SYMBOL       GROUP  PERIOD NUMBER   WEIGHT       ATOMIC  COV.  VDW       METAL   EN    
		Element("Actinium",                "Ac",         3,    7,      89,     227.0278f,    1.88f,  0.0f,  0.0f,  true,   1.3f),
		Element("Aluminum",                "Al",        13,    3,      13,      26.981539f,  1.43f,  1.25f, 2.05f, true,   1.61f),
		Element("Americium",               "Am",         0,    7,      95,     243.0614f,    1.73f,  0.0f,  0.0f,  true,   1.3f),
		Element("Antimony",                "Sb",        15,    5,      51,     121.76f,      1.82f,  1.41f, 2.2f,  false,  2.05f),
		Element("Argon",                   "Ar",        18,    3,      18,      39.948f,     1.74f,  0.0f,  1.91f, false,  0.0f),
		Element("Arsenic",                 "As",        15,    4,      33,      74.92159f,   1.25f,  1.21f, 2.0f,  false,  2.18f),
		Element("Astatine",                "At",        17,    6,      85,     209.9871f,    0.0f,   0.0f,  0.0f,  false,  1.96f),
		Element("Barium",                  "Ba",         2,    6,      56,     137.327f,     2.17f,  1.98f, 0.0f,  true,   0.89f),
		Element("Berkelium",               "Bk",         0,    7,      97,     247.0703f,    1.70f,  0.0f,  0.0f,  true,   1.3f),
		Element("Beryllium",               "Be",         2,    2,       4,       9.012182f,  1.13f,  0.89f, 0.0f,  true,   1.57f),
		Element("Bismuth",                 "Bi",        15,    6,      83,     208.98037f,   1.55f,  1.52f, 2.4f,  true,   2.0f),
		Element("Bohrium",                 "Bh",         7,    7,     107,     262.12f,      0.0f,   0.0f,  0.0f,  true,   0.0f),
		Element("Boron",                   "B",         13,    2,       5,      10.811f,     0.83f,  0.88f, 2.08f, false,  2.04f),
		Element("Bromine",                 "Br",        17,    4,      35,      79.904f,     0.0f,   1.14f, 1.95f, false,  2.96f),
		Element("Cadmium",                 "Cd",        12,    5,      48,     112.411f,     1.49f,  1.41f, 0.0f,  true,   1.69f),
		Element("Caesium",                 "Cs",         1,    6,      55,     132.90543f,   2.654f, 2.35f, 2.62f, true,   0.79f),
		Element("Calcium",                 "Ca",         2,    4,      20,      40.078f,     1.97f,  1.74f, 0.0f,  true,   1.0f),
		Element("Californium",             "Cf",         0,    7,      98,     251.0796f,    1.69f,  0.0f,  0.0f,  true,   1.3f),
		Element("Carbon",                   "C",        14,    2,       6,      12.011f,     0.77f,  0.77f, 1.85f, false,  2.55f),
		Element("Cerium",                  "Ce",         0,    6,      58,     140.115f,     1.825f, 1.65f, 0.0f,  true,   1.12f),
		Element("Chlorine",                "Cl",        17,    3,      17,      35.4527f,    0.0f,   0.99f, 1.81f, false,  3.16f),
		Element("Chromium",                "Cr",         6,    4,      24,      51.9961f,    1.25f,  0.0f,  0.0f,  true,   1.66f),
		Element("Cobalt",                  "Co",         9,    4,      27,      58.9332f,    1.25f,  1.16f, 0.0f,  true,   1.88f),
		Element("Copper",                   "Cu",        11,    4,      29,      63.546f,     1.28f,  1.17f, 0.0f, true,    1.9f),
		Element("Curium",                  "Cm",         0,    7,      96,     247.0703f,    1.74f,  0.0f,  0.0f,  true,   1.3f),
		Element("Dubnium",                 "Db",         4,    7,     104,     261.11f,      0.0f,   0.0f,  0.0f,  true,   0.0f),
		Element("Dysprosium",              "Dy",         0,    6,      66,     162.5f,       1.77f,  1.59f, 0.0f,  true,   1.23f),
		Element("Einsteinium",             "Es",         0,    7,      99,     252.083f,     2.03f,  0.0f,  0.0f,  true,   1.3f),
		Element("Erbium",                  "Er",         0,    6,      68,     167.26f,      1.76f,  1.57f, 0.0f,  true,   1.25f),
		Element("Europium",                "Eu",         0,    6,      63,     151.965f,     2.04f,  1.85f, 0.0f,  true,   1.2f),
		Element("Fermium",                 "Fm",         0,    7,     100,     257.0951f,    0.0f,   0.0f,  0.0f,  false,  1.3f),
		Element("Fluorine",                "F",         17,    2,       9,      18.9984032f, 0.709f, 0.58f, 1.35f, false,  3.98f),
		Element("Francium",                "Fr",         1,    7,      87,     223.0197f,    2.7f,   0.0f,  0.0f,  true,   0.7f),
		Element("Gadolinium",              "Gd",         0,    6,      64,     157.25f,      1.8f,   1.61f, 0.0f,  true,   0.94f),
		Element("Gallium",                 "Ga",        13,    4,      31,      69.723f,     1.22f,  1.25f, 0.0f,  true,   1.81f),
		Element("Germanium",               "Ge",        14,    4,      32,      72.61f,      1.23f,  1.22f, 0.0f,  false,  2.01f),
		Element("Gold"/*Aurum*/,           "Au",        11,    6,      79,     196.96654f,   1.44f,  1.34f, 0.0f,  true,   2.0f),
		Element("Hafnium",                 "Hf",         4,    6,      72,     178.49f,      1.56f,  1.44f, 0.0f,  true,   1.5f),
		Element("Hahnium",                 "Hn",         8,    7,     108,       0.0f,       0.0f,   0.0f,  0.0f,  true,   0.0f),
		Element("Helium",                  "He",        18,    1,       2,       4.002602f,  1.28f,  0.0f,  1.22f, false,  0.0f),
		Element("Holmium",                 "Ho",         0,    6,      67,     164.93032f,   1.77f,  1.58f, 0.0f,  true,   1.24f),
		Element("Hydrogen"/*Hydrogenium*/, "H",          1,    1,       1,       1.00797f,   0.78f,  0.3f,  1.2f,  false,  2.2f),
		Element("Indium",                  "In",        13,    5,      49,     114.818f,     1.63f,  1.5f,  0.0f,  true,   1.78f),
		Element("Iodine"/*Jod*/,           "I",         17,    5,      53,     126.90447f,   0.0f,   1.33f, 2.15f, false,  2.66f),
		Element("Iridium",                 "Ir",         9,    6,      77,     192.217f,     1.36f,  1.26f, 0.0f,  true,   2.28f),
		Element("Iron"/*Ferrum*/,          "Fe",         8,    4,      26,      55.845f,     1.24f,  1.16f, 0.0f,  true,   1.83f),
		Element("Joliotium",               "Jl"/*262*/,  5,    7,     105,     262.114f,     0.0f,   0.0f,  0.0f,  true,   0.0f),
		Element("Krypton",                 "Kr",        18,    4,      36,      83.80f,      0.0f,   1.89f, 1.98f, false,  0.0f),
		Element("Lanthanum",               "La",         3,    6,      57,     138.9055f,    1.88f,  1.69f, 0.0f,  true,   1.1f),
		Element("Lawrencium",              "Lr"/*262*/,  3,    7,     103,     262.11f,      0.0f,   0.0f,  0.0f,  true,   0.0f),
		Element("Lead"/*Plumbum*/,         "Pb",        14,    6,      82,     207.2f,       1.75f,  1.54f, 0.0f,  true,   2.02f),
		Element("Lithium",                 "Li",         1,    2,       3,       6.941f,     1.52f,  1.23f, 0.0f,  true,   0.98f),
		Element("Lutetium",                "Lu",         3,    6,      71,     174.967f,     1.72f,  1.56f, 0.0f,  true,   1.3f),
		Element("Magnesium",               "Mg",         2,    3,      12,      24.30506f,   1.6f,   1.36f, 0.0f,  true,   1.31f),
		Element("Manganese"/*Mangan*/,     "Mn",         7,    4,      25,      54.93805f,   1.24f,  1.77f, 0.0f,  true,   1.55f),
		Element("Meitnerium",              "Mt"/*266*/,  9,    7,     109,       0.0f,       0.0f,   0.0f,  0.0f,  true,   0.0f),
		Element("Mendelevium",             "Md"/*258*/,  0,    7,     101,     258.1f,       0.0f,   0.0f,  0.0f,  true,   1.3f),
		Element("Mercury"/*Hydrargyrum*/,  "Hg",        12,    6,      80,     200.59f,      1.60f,  1.44f, 0.0f,  true,   1.8f),
		Element("Molybdenum",              "Mo",         6,    5,      42,      95.94f,      1.36f,  1.29f, 0.0f,  true,   2.16f),
		Element("Neodymium",               "Nd",         0,    6,      60,     144.24f,      1.82f,  1.64f, 0.0f,  true,   1.14f),
		Element("Neon",                    "Ne",        18,    2,      10,      20.1797f,    0.0f,   0.0f,  1.6f,  false,  0.0f),
		Element("Neptunium",               "Np"/*237*/,  0,    7,      93,     237.0482f,    1.5f,   0.0f,  0.0f,  true,   1.28f),
		Element("Nickel",                  "Ni",        10,    4,      28,      58.6934f,    1.25f,  1.15f, 0.0f,  true,   1.91f),
		Element("Niobium",                 "Nb",         5,    5,      41,      92.90638f,   1.43f,  1.34f, 0.0f,  true,   1.6f),
		Element("Nitrogen"/*Nitrogenium*/, "N",         15,    2,       7,      14.00674f,   0.71f,  0.7f,  1.54f, false,  3.04f),
		Element("Nobelium",                "No"/*259*/,  0,    7,     102,     259.1009f,    0.0f,   0.0f,  0.0f,  true,   0.0f),
		Element("Osmium",                  "Os",         8,    6,      76,     190.23f,      1.35f,  1.26f, 0.0f,  true,   2.2f),
		Element("Oxygen"/*Oxygenium*/,     "O",         16,    2,       8,      15.9994f,    0.6f,   0.66f, 1.4f,  false,  3.44f),
		Element("Palladium",               "Pd",        10,    5,      46,     106.42f,      1.38f,  1.28f, 0.0f,  true,   2.2f),
		Element("Phosphorus",              "P",         15,    3,      15,      30.973762f,  1.15f,  1.10f, 1.9f,  false,  2.19f),
		Element("Platinum",                "Pt",        10,    6,      78,     195.08f,      1.38f,  1.29f, 0.0f,  true,   2.54f),
		Element("Plutonium",               "Pu"/*244*/,  7,    0,      94,     244.0642f,    0.0f,   0.0f,  0.0f,  true,   1.3f),
		Element("Polonium",                "Po"/*209*/, 16,    6,      84,     208.9824f,    1.67f,  1.53f, 0.0f,  false,  2.2f),
		Element("Potassium"/*Kalium*/,     "K",          1,    4,      19,      39.0983f,    2.27f,  2.03f, 2.31f, true,   0.82f),
		Element("Praseodymium",            "Pr",         0,    6,      59,     140.90765f,   1.83f,  1.65f, 0.0f,  true,   1.13f),
		Element("Promethium",              "Pm"/*145*/,  0,    6,      61,     144.9127f,    1.81f,  0.0f,  0.0f,  true,   0.94f),
		Element("Protactinium",            "Pa",         0,    7,      91,     231.03588f,   1.61f,  0.0f,  0.0f,  true,   1.38f),
		Element("Radium",                  "Ra"/*226*/,  2,    7,      88,     226.0254f,    2.23f,  0.0f,  0.0f,  true,   0.89f),
		Element("Radon",                   "Rn"/*222*/, 18,    6,      86,     222.0176f,    0.0f,   0.0f,  0.0f,  false,  0.7f),
		Element("Rhenium",                 "Re",         7,    6,      75,     186.207f,     1.37f,  1.28f, 0.0f,  true,   2.2f),
		Element("Rhodium",                 "Rh",         9,    5,      45,     102.9055f,    1.34f,  1.25f, 0.0f,  true,   2.28f),
		Element("Rubidium",                "Rb",         1,    5,      37,      85.4678f,    1.475f, 0.0f,  2.44f, true,   0.82f),
		Element("Ruthenium",               "Ru",         8,    5,      44,     101.07f,      1.34f,  1.24f, 0.0f,  true,   2.2f),
		Element("Rutherfordium",           "Rf"/*263*/,  6,    7,     106,     263.118f,     0.0f,   0.0f,  0.0f,  true,   0.0f),
		Element("Samarium",                "Sm",         0,    6,      62,     150.36f,      1.8f,   1.66f, 0.0f,  true,   1.17f),
		Element("Scandium",                "Sc",         3,    4,      21,      44.95591f,   1.61f,  1.44f, 0.0f,  true,   1.36f),
		Element("Selenium",                "Se",        16,    4,      34,      78.96f,      2.15f,  1.17f, 2.0f,  true,   2.55f),
		Element("Silicon"/*Silicium*/,     "Si",        14,    3,      14,      28.0855f,    1.17f,  1.17f, 2.0f,  false,  1.9f),
		Element("Silver"/*Argentum*/,      "Ag",        11,    5,      47,     107.8682f,    1.44f,  1.34f, 0.0f,  true,   1.93f),
		Element("Sodium"/*Natrium*/,       "Na",         1,    3,      11,      22.989768f,  1.54f,  0.0f,  2.31f, true,   0.93f),
		Element("Strontium",               "Sr",         2,    5,      38,      87.62f,      2.15f,  1.92f, 0.0f,  true,   0.95f),
		Element("Sulphur"/*Sulfur*/,       "S",         16,    3,      16,      32.066f,     1.04f,  1.04f, 1.85f, false,  2.58f),
		Element("Tantalum",                "Ta",         5,    6,      73,     180.9479f,    1.43f,  1.34f, 0.0f,  true,   2.36f),
		Element("Technetium",              "Tc",         7,    5,      43,      98.9072f,    1.36f,  0.0f,  0.0f,  true,   1.9f),
		Element("Tellurium",               "Te",        16,    5,      52,     127.6f,       1.43f,  1.37f, 2.2f,  false,  2.1f),
		Element("Terbium",                 "Tb",         0,    6,      65,     158.92534f,   1.78f,  1.59f, 0.0f,  true,   1.22f),
		Element("Thallium",                "Tl",        13,    6,      81,     204.3833f,    1.7f,   1.55f, 0.0f,  true,   2.33f),
		Element("Thorium",                 "Th",         0,    7,      90,     232.0381f,    1.80f,  0.0f,  0.0f,  true,   0.0f),
		Element("Thulium",                 "Tm",         0,    6,      69,     168.93421f,   1.75f,  1.56f, 0.0f,  true,   0.96f),
		Element("Tin"/*Stannum*/,          "Sn",        14,    5,      50,     118.71f,      1.41f,  1.4f,  2.0f,  true,   1.96f),
		Element("Titanium",                "Ti",         4,    4,      22,      47.867f,     1.45f,  1.32f, 0.0f,  true,   1.54f),
		Element("Tungsten"/*Wolfram*/,     "W",          6,    6,      74,     183.84f,      1.37f,  1.3f,  0.0f,  true,   1.9f),
		Element("Ununbium",                "Uub",       12,    7,     112,       0.0f,       0.0f,   0.0f,  0.0f,  true,   0.0f),
		Element("Ununnilium",              "Uun",       10,    7,     110,       0.0f,       0.0f,   0.0f,  0.0f,  true,   0.0f),
		Element("Ununninium",              "Uuu",       11,    7,     111,       0.0f,       0.0f,   0.0f,  0.0f,  true,   0.0f),
		Element("Uranium",                 "U",          0,    7,      92,     238.0289f,    1.54f,  0.0f,  0.0f,  true,   1.26f),
		Element("Vanadium",                "V",          5,    4,      23,      50.9415f,    1.32f,  0.0f,  0.0f,  true,   1.63f),
		Element("Xenon",                   "Xe",        18,    5,      54,     131.29f,      2.18f,  2.09f, 2.16f, false,  2.6f),
		Element("Ytterbium",               "Yb",         0,    6,      70,     173.04f,      1.94f,  1.7f,  0.0f,  true,   1.27f),
		Element("Yttrium",                 "Y",          3,    5,      39,      88.90585f,   1.81f,  1.62f, 0.0f,  true,   1.22f),
		Element("Zinc"/*Zincum*/,          "Zn",        12,    4,      30,      65.39f,      1.33f,  1.25f, 0.0f,  true,   1.65f),
		Element("Zirconium",               "Zr",         4,    5,      40,      91.224f,     1.6f,   1.45f, 0.0f,  true,   1.3f)
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

	Element* PTE_::atomic_number_to_element_[] = 
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
			is_metal_(BALL_ELEMENT_IS_METAL_DEFAULT),
			electronegativity_(BALL_ELEMENT_ELECTRONEGATIVITY_DEFAULT)
	{
	}

	Element::Element(const Element& element)
				:	name_(element.name_),
					symbol_(element.symbol_),
					group_(element.group_),
					period_(element.period_),
					atomic_number_(element.atomic_number_),
					atomic_weight_(element.atomic_weight_),
					atomic_radius_(element.atomic_radius_),
					covalent_radius_(element.covalent_radius_),
					van_der_waals_radius_(element.van_der_waals_radius_),
					is_metal_(element.is_metal_),
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
		 bool is_metal,
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
			is_metal_(is_metal),
			electronegativity_(electronegativity)
	{
	}

	Element::~Element()
	{
	}

	ostream& operator << (ostream& s, const Element& element)
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
			<< element.is_metal_ << ' '
			<< element.electronegativity_ << ' ';

		return s;
	}

	bool Element::isMetal() const
	{
		return is_metal_;
	}

	Element& Element::operator = (const Element& element)
	{
		name_ = element.name_;
		symbol_ = element.symbol_;
		group_ = element.group_;
		period_ = element.period_;
		atomic_number_ = element.atomic_number_;
		atomic_weight_ = element.atomic_weight_;
		atomic_radius_ = element.atomic_radius_;
		covalent_radius_ = element.covalent_radius_;
		van_der_waals_radius_ = element.van_der_waals_radius_;
		is_metal_ = element.is_metal_;
		electronegativity_ = element.electronegativity_;

		return *this;
	}

	PTE_::PTE_()
	{
	}

	PTE_::PTE_(const PTE_& /*pte*/)
	{
		// no nonstatic members to copy
	}

	PTE_::~PTE_()
	{
	}

	Element& PTE_::getElement(Position atomic_number)
	{
		// atomic number 1 should point us to array-index 0 ..
		atomic_number--;

		if (atomic_number < Element::NUMBER_OF_ELEMENTS)
		{
			return *atomic_number_to_element_[atomic_number];
		}
		else
		{
			return Element::UNKNOWN;
		}
	}
 
	Element& PTE_::getElement(const String& symbol)
	{
		if (symbol.isEmpty())
		{
			return Element::UNKNOWN;
		}

		char symbol_buffer[] = { '\0', '\0', '\0', '\0' };
		SymbolToElement compare = { symbol_buffer, 0 };
		
		if (symbol.size() == 1)
		{
			symbol_buffer[0] = toupper(symbol[0]);
		} 
		else 
		{
			if (symbol.size() == 2)
			{
				symbol_buffer[0] = toupper(symbol[0]);
				symbol_buffer[1] = toupper(symbol[1]);
			} 
			else 
			{
				symbol_buffer[0] = toupper(symbol[0]);
				symbol_buffer[1] = toupper(symbol[1]);
				symbol_buffer[2] = toupper(symbol[2]);
			}
		}

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
		if (!processor.start())
		{
			return false;
		}

		Processor::Result result;

		for (Position position = 0; position < Element::NUMBER_OF_ELEMENTS; ++position)
		{
			result = processor(element_[position]);

			if (result <= Processor::BREAK)
			{
				return result == Processor::BREAK;
			}
		}

		return processor.finish();
	}

	PTE_ PTE; // not conform to naming convention but fakes operator[] and operator() to be static functions.

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/KERNEL/PTE.iC>
#	endif

} // namespace BALL
