// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ReconstructFragmentProcessor 
//  : public UnaryProcessor<Fragment> 
	: FragmentProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/reconstructFragmentProcessor.h>
	#include <BALL/STRUCTURE/fragmentDB.h>
	using namespace BALL;
%End
	public:
	ReconstructFragmentProcessor();
	ReconstructFragmentProcessor(const FragmentDB&);
	~ReconstructFragmentProcessor();
	virtual bool start();
	virtual bool finish();
	Processor::Result operator()(Fragment&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
	Size getNumberOfInsertedAtoms();
	void setFragmentDB(const FragmentDB&);
	const FragmentDB* getFragmentDB() const;
};

