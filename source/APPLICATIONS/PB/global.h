// $Id: global.h,v 1.2 2000/05/25 11:02:41 oliver Exp $

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/SOLVATION/poissonBoltzmann.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>

using namespace BALL;

// a pointer to the fragment data base (a pointer since we do
// not need it every time an want to save the loading time)
extern FragmentDB*           frag_db;

// the Finite Difference Poisson-Boltzmann object
extern FDPB                  fdpb;

// the options for the calculation
extern Options								options;

// charge and radius assignment processors
extern AssignChargeProcessor charges;
extern AssignRadiusProcessor radii;
extern ClearChargeProcessor	clear_charge_proc;

// rule-based assignment processors
extern ChargeRuleProcessor charge_rules;
extern RadiusRuleProcessor radius_rules;

// the system that holds everything
extern System S;

// the name of the dump file
extern String dump_file;

// flags:
//  true, if timing information and final options should be printed
extern bool verbose;

//  true, if the charges should be cleared upon read
extern bool clear_charges;

//  true, if the names should be normalized upon read
extern bool normalize_names;

// true, if the rule-based assignment is to be used
extern bool use_charge_rules;
extern bool use_radius_rules;


