// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: persistentObject.sip,v 1.2 2003/08/26 09:18:18 oliver Exp $
//

class PersistentObject
{ 
%HeaderCode
#include <BALL/CONCEPT/persistentObject.h>
using namespace BALL;
%End
  public:
  PersistentObject();
	// virtual ~PersistentObject();
  //PersistenceManager& operator >> (PersistenceManager& pm) const;
  //virtual void persistentWrite(PersistenceManager& pm, const char* name = "") const;
  //virtual void persistentRead(PersistenceManager& pm);
  //virtual void finalize()
};
