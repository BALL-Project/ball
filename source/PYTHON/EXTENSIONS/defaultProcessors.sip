// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: defaultProcessors.sip,v 1.3 2003/09/09 18:57:06 oliver Exp $
//

class ClearChargeProcessor
//	: public UnaryProcessor<Atom> 
  : AtomProcessor 
{
%HeaderCode
	#include <BALL/STRUCTURE/defaultProcessors.h>
	using namespace BALL;
%End
  public:
  Processor::Result operator () (Atom& /atom/);
};

class ClearRadiusProcessor
//  : public UnaryProcessor<Atom>
	: AtomProcessor
{
%HeaderCode
	#include <BALL/STRUCTURE/defaultProcessors.h>
	using namespace BALL;
%End
  public:
  Processor::Result operator () (Atom& /atom/);
%MemberCode
	printf("TEST\n");
	return 0;
%End
};

class AssignRadiusProcessor
//  : public UnaryProcessor<Atom> 
	: AtomProcessor
{
%HeaderCode
	#include <BALL/STRUCTURE/defaultProcessors.h>
	using namespace BALL;
%End
  public:
  AssignRadiusProcessor();
  AssignRadiusProcessor(const String& /filename/);
  virtual bool start();
  virtual bool finish();
  Processor::Result operator() (Atom& /atom/);
  void setFilename(const String& /filename/);
  String& getFilename();
  Size getNumberOfAssignments();
  Size getNumberOfErrors();
};

class AssignChargeProcessor
  : AssignRadiusProcessor 
{
%HeaderCode
	#include <BALL/STRUCTURE/defaultProcessors.h>
	using namespace BALL;
%End
  public:
  AssignChargeProcessor();
  AssignChargeProcessor(const String& /filename/);
  virtual bool start();
  Processor::Result operator () (Atom& /atom/);
  float getTotalCharge();
};
 
