// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: embeddable.sip,v 1.2 2003/08/26 09:18:15 oliver Exp $
//

class Embeddable
{
%HeaderCode
	#include <BALL/CONCEPT/embeddable.h>
	using namespace BALL;
%End
  public:
  // typedef List<const Embeddable*> EmbeddableList;
  ~Embeddable() throw();

  void unregisterThis() throw();
  virtual void registerThis() throw();
};
