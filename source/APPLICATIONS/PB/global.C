// $Id: global.C,v 1.6 2000/06/06 13:16:10 oliver Exp $
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

// default radius is 1.4 Angstrom (water)
float probe_radius = 1.4;

// flags:

// the main options
// true, if the solvent excluded surface is to be calculated
bool ses_calculation = false;

// true, if the solvent accessible surface is to be calculated
bool sas_calculation = false;

// true, if a FDPB calculation is to ber performed
bool fdpb_calculation = false;

// the results of the SES and SAS area calculations
float total_SAS_area = 0.0;
float total_SES_area = 0.0;

// a hash map containing the atom surfaces of the SAS (if calculated)
HashMap<Atom*, float> surface_map;


// further options

bool verbose = false;
bool clear_charges = false;
bool normalize_names = false;
bool build_bonds = false;
bool use_charge_rules = false;
bool use_radius_rules = false;
bool calculate_solvation_energy = false;
bool calculate_SES = false;
 

