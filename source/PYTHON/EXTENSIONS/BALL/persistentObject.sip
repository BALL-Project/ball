// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class PersistentObject
{ 
%TypeHeaderCode
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
