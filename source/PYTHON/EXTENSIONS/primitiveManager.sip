// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: primitiveManager.sip,v 1.2 2003/12/07 22:34:38 oliver Exp $
//

class PrimitiveManager
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/primitiveManager.h>
	using namespace BALL;
	using namespace VIEW;
%End
  public:
  // BALL_CREATE(PrimitiveManager);
  // PyRepresentationList changedComposite(const Composite& /composite/) throw();
  PyRepresentationList removedComposite(const Composite& /composite/) throw();
  PrimitiveManager() throw();
  PrimitiveManager(const PrimitiveManager& /pm/) throw();
  Representation* createRepresentation() throw();
  // RepresentationsConstIterator begin() const throw();
  // RepresentationsConstIterator end() const throw();
  // RepresentationsIterator begin() throw();
  // RepresentationsIterator end() throw();
  Size getNumberOfRepresentations() const throw();
  bool has(const Representation& /representation/) const throw();
  bool operator == (const PrimitiveManager& /pm/) const throw();
  // const PrimitiveManager& operator = (const PrimitiveManager& /pm/) throw();
  const PyRepresentationList& getRepresentations() const throw();
  void clear() throw();
  // void dump(std::ostream& s, Size depth) const throw();
  void insert(Representation& /representation/) throw();
  void remove(Representation& /representation/) throw();
  ~PrimitiveManager() throw();
};
