// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:$
//

%ModuleCode
	#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
	using namespace BALL;
%End

class ConnectedComponentsProcessor 
	: AtomContainerProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
	using namespace std;
	using namespace BALL;
	using namespace BALL::Exception;
%End

	public:
		ConnectedComponentsProcessor();
		virtual ~ConnectedComponentsProcessor();
		
		void clear();
		virtual Processor::Result operator () (AtomContainer&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
		Size getNumberOfConnectedComponents();

		//ComponentVector& getComponents();
		//const ComponentVector& getComponents() const;

		void getAllComponents(vector<Molecule>& results);
		void getMinAtomsComponents(vector<Molecule>& result, const int &min);
		void getLargestComponent(Molecule& result);
};
