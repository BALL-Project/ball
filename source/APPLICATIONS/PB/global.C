// $Id: global.C,v 1.3 2000/05/27 10:38:59 oliver Exp $
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
bool verbose = false;
bool clear_charges = false;
bool normalize_names = false;
bool use_charge_rules = false;
bool use_radius_rules = false;
bool calculate_solvation_energy = false;
