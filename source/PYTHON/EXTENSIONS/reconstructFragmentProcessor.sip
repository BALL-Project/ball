// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: reconstructFragmentProcessor.sip,v 1.4 2003/09/09 18:57:08 oliver Exp $
//

class ReconstructFragmentProcessor 
//  : public UnaryProcessor<Fragment> 
	: FragmentProcessor
{
%HeaderCode
	#include <BALL/STRUCTURE/reconstructFragmentProcessor.h>
	#include <BALL/STRUCTURE/fragmentDB.h>
	using namespace BALL;
%End
	public:
	ReconstructFragmentProcessor();
	ReconstructFragmentProcessor(const FragmentDB& /db/);
	~ReconstructFragmentProcessor();
	virtual bool start();
	virtual bool finish();
	Processor::Result operator()(Fragment&);
	Size getNumberOfInsertedAtoms();
	void setFragmentDB(const FragmentDB& /fragment_db/);
	const FragmentDB* getFragmentDB() const;
};

