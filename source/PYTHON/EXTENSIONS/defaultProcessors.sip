// $Id: defaultProcessors.sip,v 1.1 2001/09/03 16:53:07 oliver Exp $

class ClearChargeProcessor
//	: public UnaryProcessor<Atom> 
  : AtomProcessor 
{
%HeaderCode
	#include <BALL/STRUCTURE/defaultProcessors.h>
	using namespace BALL;
%End
  public:
  virtual Processor::Result operator () (Atom& /atom/);
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
  virtual Processor::Result operator () (Atom& /atom/);
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
  virtual Processor::Result operator() (Atom& /atom/);
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
  virtual Processor::Result operator () (Atom& /atom/);
  float getTotalCharge();
};
 
