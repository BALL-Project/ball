// $Id: charmm.sip,v 1.1 2001/09/03 16:53:05 oliver Exp $

class CharmmFF 
  : ForceField
{
%HeaderCode
	#include <BALL/MOLMEC/CHARMM/charmm.h>
	using namespace BALL;
%End
  public:
//struct Option
//{
//   static const char* FILENAME;
//   static const char* NONBONDED_CUTOFF;
//   static const char* VDW_CUTOFF;
//   static const char* VDW_CUTON;
//   static const char* ELECTROSTATIC_CUTOFF;
//   static const char* ELECTROSTATIC_CUTON;
//   static const char* SOLVATION_CUTOFF;
//   static const char* SOLVATION_CUTON;
//   static const char* SCALING_VDW_1_4;
//   static const char* USE_EEF1;
//   static const char* SCALING_ELECTROSTATIC_1_4;
//   static const char* DISTANCE_DEPENDENT_DIELECTRIC; 
//   static const char* ASSIGN_CHARGES;
//   static const char* ASSIGN_TYPENAMES;
//   static const char* ASSIGN_TYPES;
//   static const char* OVERWRITE_CHARGES;
//   static const char* OVERWRITE_TYPENAMES;
//  };
//  struct Default
//  {
// static const char* FILENAME;
//   static const float NONBONDED_CUTOFF;
//   static const float VDW_CUTOFF;
//   static const float VDW_CUTON;
//   static const float ELECTROSTATIC_CUTOFF;
//   static const float ELECTROSTATIC_CUTON;
//   static const float SOLVATION_CUTOFF;
//   static const float SOLVATION_CUTON;
// static const float SCALING_ELECTROSTATIC_1_4; 
//   static const bool USE_EEF1;
// static const float SCALING_VDW_1_4; 
//   static const bool DISTANCE_DEPENDENT_DIELECTRIC; 
//   static const bool ASSIGN_CHARGES;
//   static const bool ASSIGN_TYPENAMES;
//   static const bool ASSIGN_TYPES;
//   static const bool OVERWRITE_CHARGES;
//   static const bool OVERWRITE_TYPENAMES;
//  };
  // BALL_CREATE(CharmmFF)
  CharmmFF();
  CharmmFF(System& /system/);
  CharmmFF(System& /system/, const Options& /options/);
  CharmmFF(const CharmmFF& /force_field/);
  ~CharmmFF();
  virtual bool specificSetup();
  double getStretchEnergy() const;
  double getBendEnergy() const;
  double getTorsionEnergy() const;
  double getImproperTorsionEnergy() const;
  double getProperTorsionEnergy() const;
  double getNonbondedEnergy() const;
  double getVdWEnergy() const;
  double getESEnergy() const;
  double getSolvationEnergy() const;
  bool hasInitializedParameters() const;
};
