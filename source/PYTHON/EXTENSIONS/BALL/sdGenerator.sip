// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class SDGenerator
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/sdGenerator.h>
	#include <BALL/KERNEL/atom.h>
	#include <BALL/KERNEL/system.h>
	using namespace BALL;
%End

    public:
		
		struct Option
		{	
%TypeHeaderCode	
	#include <BALL/STRUCTURE/sdGenerator.h>
	using namespace BALL;
%End
		 	static const char* SHOW_HYDROGENS;
			static const char* STANDARD_BOND_LENGTH;
		};

		struct Default
		{		
%TypeHeaderCode	
	#include <BALL/STRUCTURE/sdGenerator.h>
	using namespace BALL;
%End
				static const bool SHOW_HYDROGENS;
				static const float STANDARD_BOND_LENGTH;
		};
			
		SDGenerator(bool);
		virtual ~SDGenerator();
			
		Options options;
			
		void setDefaultOptions();
    void generateSD(System&);
		void clear();
}; 
