// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: global.C,v 1.5 2003/12/01 07:35:10 oliver Exp $
//

#include "global.h"

FragmentDB*           frag_db = 0;
FDPB                  fdpb;
AssignChargeProcessor charges("charges/PARSE.crg");
AssignRadiusProcessor radii("radii/PARSE.siz");
ClearChargeProcessor	clear_charge_proc;

ChargeRuleProcessor   charge_rules;
RadiusRuleProcessor   radius_rules;

Options								options;
System								S;

String dump_file;

// flags:

// the main options
bool energy_calculation = false;
bool energy_minimization = false;
bool check_structures = false;

// further options

bool verbose = false;
bool normalize_names = false;
bool build_bonds = false;

bool use_selection = false;
String selection;
double max_gradient = 0.1; // kJ/(mol A)
Size max_iterations = 400;

String FF_filename = "Amber/amber96.ini";
String out_filename = "";
bool sd_minimizer = false;
double energy_limit = 500.0; // kJ/mol
