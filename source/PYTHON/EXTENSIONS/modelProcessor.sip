// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: modelProcessor.sip,v 1.2 2004/02/25 10:47:13 oliver Exp $
//

class ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/modelProcessor.h>
	#include "sipBALLComposite.h"
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

 	ModelProcessor() throw();
  ModelProcessor(const ModelProcessor& /model_processor/) throw();
  Processor::Result operator() (Composite& /composite/);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
  bool finish();
  bool isValid() const throw();
  bool start();
  void clear() throw();
  ~ModelProcessor() throw();
};
