// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: persistentObject.sip,v 1.3 2003/10/29 22:08:39 oliver Exp $
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
