// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: embeddable.sip,v 1.3 2003/10/29 22:08:39 oliver Exp $
//

class Embeddable
{
%TypeHeaderCode
	#include <BALL/CONCEPT/embeddable.h>
	using namespace BALL;
%End
  public:
  // typedef List<const Embeddable*> EmbeddableList;
  ~Embeddable() throw();

  void unregisterThis() throw();
  virtual void registerThis() throw();
};
