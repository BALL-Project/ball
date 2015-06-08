// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:$
//

%ModuleCode
	#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
	using namespace BALL;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End



class ConnectedComponentsProcessor 
	: CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
	using namespace std;
	using namespace BALL;
	using namespace BALL::Exception;
	typedef UnaryProcessor<Composite> CompositeProcessor;	
	
	//typedef vector<Atom*>      Component;
	//typedef	vector<Component>	ComponentVector;
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
