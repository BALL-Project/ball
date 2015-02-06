// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

%ModuleCode
	#include <BALL/MOLMEC/COMMON/assignTypes.h>
	using namespace BALL;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End

class	AssignBaseProcessor
	:	public AtomProcessor
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/assignTypes.h>
	using namespace BALL;
	using namespace BALL::Exception;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End

	public:

		AssignBaseProcessor();

		void setMaximumUnassignedAtoms(Size);
		Size getMaximumUnassignedAtoms() const;
		Size getNumberOfUnassignedAtoms() const;

		virtual Processor::Result operator () (Atom& atom);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

		virtual ~AssignBaseProcessor();

//		HashSet<const Atom*>& getUnassignedAtoms();
};

class	AssignTypeProcessor
	:	public AssignBaseProcessor
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/assignTypes.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End

	public:

		AssignTypeProcessor(const AtomTypes&);
		virtual Processor::Result operator () (Atom&);
%MethodCode
		sipRes = sipCpp->operator () (*a0);
%End
};

class	AssignTypeNameProcessor
	:	public AssignBaseProcessor
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/assignTypes.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End

	public:

		AssignTypeNameProcessor(const String&, bool overwrite);

		virtual Processor::Result operator () (Atom& atom);
%MethodCode
		sipRes = sipCpp->operator () (*a0);
%End
};
