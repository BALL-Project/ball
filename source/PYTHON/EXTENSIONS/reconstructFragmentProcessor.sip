// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: reconstructFragmentProcessor.sip,v 1.3 2003/08/26 09:18:22 oliver Exp $
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
	virtual Processor::Result operator()(Fragment&);
	Size getNumberOfInsertedAtoms();
	void setFragmentDB(const FragmentDB& /fragment_db/);
	const FragmentDB* getFragmentDB() const;
};

