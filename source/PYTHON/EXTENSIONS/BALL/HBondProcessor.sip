// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:$
//  

%ModuleCode
	#include <BALL/STRUCTURE/HBondProcessor.h>
	using namespace BALL;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End

	class HBondProcessor 
		:	CompositeProcessor
  {
%TypeHeaderCode
	#include <BALL/STRUCTURE/HBondProcessor.h>
	using namespace BALL;
	using namespace std;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End
	
	public:	
		class HBond
		{
%TypeHeaderCode
	#include <BALL/STRUCTURE/HBondProcessor.h>
	using namespace BALL;
%End

			public:	
				HBond();
				HBond(const Atom*, const Atom*, bool); 
					
				const Atom * getAcceptor();
				const Atom * getDonor();
				bool donorIsHydrogen();
				float getLength();
		};

	
		struct Option
		{
%TypeHeaderCode
	#include <BALL/STRUCTURE/HBondProcessor.h>
	using namespace BALL;
%End	
			static const String PREDICTION_METHOD;
			static const String ADD_HBONDS;	
			static const String KABSCH_SANDER_ENERGY_CUTOFF;
		};

		struct Default
		{
%TypeHeaderCode
	#include <BALL/STRUCTURE/HBondProcessor.h>
	using namespace BALL;
%End
			static const String PREDICTION_METHOD;	 
			static const bool ADD_HBONDS;	
			static const float KABSCH_SANDER_ENERGY_CUTOFF;
		};

		struct PredictionMethod
		{
%TypeHeaderCode
	#include <BALL/STRUCTURE/HBondProcessor.h>
	using namespace BALL;
%End
			static const String KABSCH_SANDER;
			static const String WISHART_ET_AL;	
		};

    HBondProcessor();	
		HBondProcessor(Options);
		virtual ~HBondProcessor();
		
    Processor::Result operator() (Composite&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
		
  // vector<HBondProcessor::HBond> getHBonds() const;
	// std::vector< std::vector<Position> > getBackboneHBondPairs();
	
		void setDefaultOptions();

		Options options;
};
