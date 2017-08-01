class AssignBondOrderProcessor
	: AtomContainerProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/EXTENSIONS/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
%End

	public:
	struct Option
	{
		static const char* OVERWRITE_SINGLE_BOND_ORDERS;
		static const char* OVERWRITE_DOUBLE_BOND_ORDERS;
		static const char* OVERWRITE_TRIPLE_BOND_ORDERS;
		static const char* OVERWRITE_SELECTED_BONDS;
		static const char* ADD_HYDROGENS;
		static const char* USE_FINE_PENALTY;
		static const char* KEKULIZE_RINGS;
		static const char* ALGORITHM;
		static const char* INIFile;
		static const char* MAX_BOND_ORDER;
		static const char* MAX_NUMBER_OF_SOLUTIONS;
		static const char* COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS;
		static const char* BOND_LENGTH_WEIGHTING;
		static const char* APPLY_FIRST_SOLUTION;
	};

	struct Default
	{
		static const bool OVERWRITE_SINGLE_BOND_ORDERS;
		static const bool OVERWRITE_DOUBLE_BOND_ORDERS;
		static const bool OVERWRITE_TRIPLE_BOND_ORDERS;
		static const bool OVERWRITE_SELECTED_BONDS;
		static const bool ADD_HYDROGENS;
		static const bool USE_FINE_PENALTY;
		static const bool KEKULIZE_RINGS;
		static const String ALGORITHM;
		static const String INIFile;
		static const int MAX_BOND_ORDER;
		static const int MAX_NUMBER_OF_SOLUTIONS;
		static const bool COMPUTE_ALSO_NON_OPTIMAL_SOLUTIONS;
		static const float BOND_LENGTH_WEIGHTING;
		static const bool APPLY_FIRST_SOLUTION;
	};

	struct Algorithm
	{
		static const String A_STAR;
		static const String ILP;
		static const String K_GREEDY;
		static const String BRANCH_AND_BOUND;
	};

	AssignBondOrderProcessor();
	virtual ~AssignBondOrderProcessor();

	void clear();
	virtual Processor::Result operator () (AtomContainer&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

	Size getNumberOfAddedHydrogens(Position);
	Size getNumberOfComputedSolutions();
	AtomContainer* getAtomContainer();
	const System& getSolution(Position) throw(IndexOverflow);
	float getTotalPenalty(Position);
	int getNumberOfNodeExpansions(Position);
	int getQueueSize(Position);
	bool apply(Position);
	void resetBondOrders();
	bool computeNextSolution();
	void setDefaultOptions();
	float evaluatePenalty(AtomContainer*);
	Options options;

	private:
	AssignBondOrderProcessor(const AssignBondOrderProcessor&);
};
