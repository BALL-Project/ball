// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ResidueChecker
//  : public UnaryProcessor<Residue>
	: ResidueProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/residueChecker.h>
	using namespace BALL;
	typedef UnaryProcessor<Residue> ResidueProcessor;
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
  ResidueChecker(FragmentDB&);
  ResidueChecker(const ResidueChecker&);
  ~ResidueChecker();


  void enable(TestType) throw();
  void disable(TestType) throw();
	bool isEnabled(TestType) throw();

  bool getStatus() const;
  Processor::Result operator () (Residue&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
  bool start();
  bool finish();
}; 
