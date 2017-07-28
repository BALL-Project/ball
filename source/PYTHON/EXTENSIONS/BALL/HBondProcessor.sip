class HBondProcessor 
	: CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/HBondProcessor.h>
	using namespace BALL;
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

	void setDefaultOptions();
	Options options;
};
