// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: defaultProcessors.sip,v 1.5 2003/11/01 09:27:37 oliver Exp $
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
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
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
  sipRes = sipCpp->operator () (*a0);
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
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
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
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
  float getTotalCharge();
};
 
