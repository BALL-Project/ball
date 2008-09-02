// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:$
//

%ModuleHeaderCode
	#include <BALL/STRUCTURE/assignBondOrderProcessor.h>	
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End

//#ifdef BALL_HAS_LPSOLVE
//		struct _lprec;
//		typedef struct _lprec lprec;
//#endif


class AssignBondOrderProcessor 
	: CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/CONCEPT/processor.h>
	#include <BALL/KERNEL/atomContainer.h>
	#include <BALL/DATATYPE/hashMap.h>
	#include <BALL/DATATYPE/hashSet.h>
	#include <BALL/KERNEL/bond.h>
	#include <BALL/DATATYPE/options.h>
	#include <BALL/COMMON/limits.h>
	#include <BALL/COMMON/exception.h>
	#include <map>
	#include <vector>
	#include <queue>
	using namespace BALL;	
	using namespace BALL::Exception;
%End

	public:
		enum BOND_TYPES
		{
			sb,
			SB,
			db,
			DB,
			TB,
			DL,
			AB
		};

		struct Option
		{
%TypeHeaderCode
	#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
	using namespace BALL;
%End
	 		static const char* OVERWRITE_SINGLE_BOND_ORDERS;	
			static const char* OVERWRITE_DOUBLE_BOND_ORDERS;	
			static const char* OVERWRITE_TRIPLE_BOND_ORDERS;	
			static const char* OVERWRITE_SELECTED_BONDS; 	
			static const char* ADD_HYDROGENS;
			static const char* KEKULIZE_RINGS;
			static const char* ALGORITHM; 
			static const char* INIFile;	
			static const char* MAX_BOND_ORDER;
			static const char* MAX_NUMBER_OF_SOLUTIONS;
			static const char* COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS;
			static const char* BOND_LENGTH_WEIGHTING;
		};

		struct Default
		{
%TypeHeaderCode
	#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
	using namespace BALL;
%End
			static const bool OVERWRITE_SINGLE_BOND_ORDERS;
			static const bool OVERWRITE_DOUBLE_BOND_ORDERS;
			static const bool OVERWRITE_TRIPLE_BOND_ORDERS;
			static const bool OVERWRITE_SELECTED_BONDS;
			static const bool ADD_HYDROGENS;
			static const bool KEKULIZE_RINGS;
			static const String ALGORITHM;
			static const String INIFile;
			static const int MAX_BOND_ORDER;	
			static const int MAX_NUMBER_OF_SOLUTIONS;
			static const bool COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS;
			static const float BOND_LENGTH_WEIGHTING;
		};

		struct Algorithm
		{
%TypeHeaderCode
	#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
	using namespace BALL;
%End

			static const String A_STAR;
			static const String ILP; 
		};
		
	//	BALL_CREATE(AssignBondOrderProcessor);
		
		AssignBondOrderProcessor();
		AssignBondOrderProcessor(const AssignBondOrderProcessor& /abop/);
		virtual ~AssignBondOrderProcessor();
		//virtual bool start();
		void clear();
		virtual Processor::Result operator () (AtomContainer& /ac/);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
		//virtual bool finish();
		Size getNumberOfBondOrdersSet();
		Size getNumberOfAddedHydrogens(Position /i/);
		Size getNumberOfComputedSolutions();
		AtomContainer* getAtomContainer();
		//AtomContainer const* getAtomContainer() const;
		const System& getSolution(Position /i/) throw(IndexOverflow);
		float getTotalCharge(Position /i/);
		//float getTotalCharge(const Solution_& sol);
		float getTotalPenalty(Position /i/);
		//float getTotalPenalty(const Solution_& sol); 
		bool apply(Position /i/); 
		bool computeNextSolution();
		//AssignBondOrderProcessor& operator = (const AssignBondOrderProcessor& abop);
		void setDefaultOptions();
		float evaluatePenalty(AtomContainer* /ac/);
		Options options;

		
};
