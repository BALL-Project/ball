// $Id: global.C,v 1.1 2000/05/05 12:33:37 oliver Exp $
#include "global.h"

FragmentDB*           frag_db = 0;
FDPB                  fdpb;
AssignChargeProcessor charges("/KM/comp-bio/BALL-data/charges/PARSE.crg");
AssignRadiusProcessor radii("/KM/comp-bio/BALL-data/radii/PARSE.siz");
ClearChargeProcessor	clear_charge_proc;
Options								options;
System								S;

// flags:
bool verbose = false;
bool clear_charges = false;
bool normalize_names = false;



