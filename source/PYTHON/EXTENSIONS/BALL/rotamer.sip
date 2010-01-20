// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

	class Rotamer
	{
%TypeHeaderCode
	#include <BALL/STRUCTURE/residueRotamerSet.h>
	using namespace BALL;
%End
		public:
		Rotamer();
		Rotamer(const Rotamer& rotamer);
		Rotamer(float new_P, float new_chi1, float new_chi2 = 0.0, 
						float new_chi3 = 0.0, float new_chi4 = 0.0);
		bool operator == (Rotamer&);
		float P;
		float chi1;
		float chi2;
		float chi3;
		float chi4;
	};