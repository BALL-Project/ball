// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: periodicBoundary.sip,v 1.2 2003/08/26 09:18:18 oliver Exp $
//

class PeriodicBoundary
{
%HeaderCode
#include <BALL/MOLMEC/COMMON/periodicBoundary.h>
using namespace BALL;
%End
 public:
  PeriodicBoundary();
  PeriodicBoundary(const ForceField&);
  PeriodicBoundary(const PeriodicBoundary&);

  // PeriodicBoundary& operator = (const PeriodicBoundary&);
  bool setup();
  int generateMoleculesVector();
  void enable();
  void disable();
  Box3 getBox() const;
  void setBox(const Box3&);
  int addSolvent(const String&) const;
  int removeSolvent() const;
  bool isEnabled() const;
  void updateMolecules();
  Options* options;
};
