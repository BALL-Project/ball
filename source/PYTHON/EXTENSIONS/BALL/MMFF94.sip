// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class MMFF94
 : ForceField
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MMFF94/MMFF94.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  MMFF94();
  MMFF94(System&);
  MMFF94(System&, Options&);
  MMFF94(MMFF94&);

  bool specificSetup() throw(TooManyErrors);

	/*
		struct Option
		{
			static const char* FOLDER;
			static const char* ASSIGN_CHARGES;
			static const char* ASSIGN_TYPENAMES;
			static const char* ASSIGN_TYPES;
			static const char* OVERWRITE_CHARGES;
			static const char* OVERWRITE_TYPENAMES;
			static const char* NONBONDED_CUTOFF;
			static const char* VDW_CUTOFF;
			static const char* VDW_CUTON;
			static const char* ELECTROSTATIC_CUTOFF;
			static const char* ELECTROSTATIC_CUTON;
			static const char* DISTANCE_DEPENDENT_DIELECTRIC;
		};
		*/

		double getStretchEnergy() const;
		double getBendEnergy() const;
		double getStretchBendEnergy() const;
		double getTorsionEnergy() const;
		double getNonbondedEnergy() const;
		double getESEnergy() const;
		double getVdWEnergy() const;
		double getPlaneEnergy() const;
		bool hasInitializedParameters() const;
		Size getUpdateFrequency() const;
		virtual String getResults() const throw();
		//const vector<Bond*> getBonds() const;
		//const vector<HashSet<Atom*> >& getRings() const;
		//const vector<HashSet<Atom*> >& getAromaticRings() const;
		bool isInOneAromaticRing(const Bond& bond) const;
		//bool assignMMFF94BondType(Bond& bond) const;
		//const vector<MMFF94AtomType>& getAtomTypes() const;
 		//const MMFF94StretchParameters& getStretchParameters() const;
		//const MMFF94AtomTypeEquivalences & getEquivalences() const;
		//bool areInOneRing(vector<Atom*> v, Size ring_size = 0) const;
		//bool areInOneAromaticRing(const vector<Atom*>& v, Size ring_size = 0);
		bool checkAtomType(Atom& atom);
};
