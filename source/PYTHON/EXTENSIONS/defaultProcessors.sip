// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: defaultProcessors.sip,v 1.4 2003/10/29 22:54:39 oliver Exp $
//

class ClearChargeProcessor
//	: public UnaryProcessor<Atom> 
  : AtomProcessor 
{
%TypeHeaderCode
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
%TypeHeaderCode
	#include <BALL/STRUCTURE/defaultProcessors.h>
	using namespace BALL;
%End
  public:
  Processor::Result operator () (Atom& /atom/);
%MethodCode
	printf("TEST\n");
	return 0;
%End
};

class AssignRadiusProcessor
//  : public UnaryProcessor<Atom> 
	: AtomProcessor
{
%TypeHeaderCode
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
%TypeHeaderCode
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
 
