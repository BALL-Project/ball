// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: residueChecker.sip,v 1.3 2003/08/26 09:18:22 oliver Exp $
//

class ResidueChecker
//  : public UnaryProcessor<Residue>
	: ResidueProcessor
{
%HeaderCode
	#include <BALL/STRUCTURE/residueChecker.h>
	using namespace BALL;
%End
	
  public:
	enum TestType
	{
		MISSING_ATOMS = 0,
		EXTRA_ATOMS,
		ELEMENTS,
		SUSPECT_BOND_LENGTHS,
		NON_INTEGRAL_NET_CHARGE,
		LARGE_CHARGES,
		LARGE_NET_CHARGE,
		NAN_POSITIONS,
		OVERLAPPING_ATOMS,
		DUPLICATE_ATOM_NAMES,
		UNKNOWN_RESIDUES,
		NUMBER_OF_TESTS
	};

  // BALL_CREATE(ResidueChecker)
  ResidueChecker();
  ResidueChecker(FragmentDB& /fragment_db/);
  ResidueChecker(const ResidueChecker& /residue_checker/);
  ~ResidueChecker();


  void enable(TestType /t/) throw();
  void disable(TestType /t/) throw();
	bool isEnabled(TestType /t/) throw();

  bool getStatus() const;
  Processor::Result operator () (Residue& /residue/);
  bool start();
  bool finish();
}; 
