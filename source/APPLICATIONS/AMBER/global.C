// $Id: global.C,v 1.1 2000/06/10 11:23:20 oliver Exp $
#include "global.h"

FragmentDB*           frag_db = 0;
FDPB                  fdpb;
AssignChargeProcessor charges("/KM/comp-bio/BALL-data/charges/PARSE.crg");
AssignRadiusProcessor radii("/KM/comp-bio/BALL-data/radii/PARSE.siz");
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

String FF_filename = "Amber/amber91.ini";
