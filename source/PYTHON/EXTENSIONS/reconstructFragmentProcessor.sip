// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: reconstructFragmentProcessor.sip,v 1.5 2003/10/29 22:54:45 oliver Exp $
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
	ReconstructFragmentProcessor(const FragmentDB& /db/);
	~ReconstructFragmentProcessor();
	virtual bool start();
	virtual bool finish();
	Processor::Result operator()(Fragment&);
	Size getNumberOfInsertedAtoms();
	void setFragmentDB(const FragmentDB& /fragment_db/);
	const FragmentDB* getFragmentDB() const;
};

