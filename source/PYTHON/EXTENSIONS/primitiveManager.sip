// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: primitiveManager.sip,v 1.10 2004/11/15 02:07:01 amoll Exp $
//

class PrimitiveManager
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/primitiveManager.h>
	using namespace BALL;
	using namespace VIEW;
	typedef PrimitiveManager::RepresentationList RepresentationList;
%End
  public:
  // BALL_CREATE(PrimitiveManager);
  // RepresentationList changedComposite(const Composite& /composite/) throw();
  RepresentationList removedComposite(const Composite& /composite/) throw();
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
  const RepresentationList& getRepresentations() const throw();
  void clear() throw();
  // void dump(std::ostream& s, Size depth) const throw();
  void insert(Representation& /representation/, bool) throw();
  void remove(Representation& /representation/, bool) throw();
  ~PrimitiveManager() throw();

	bool willBeUpdated(const Representation&) const throw();
	bool updateRunning() const throw();
	void setMultithreadingMode(bool) throw();
	bool usesMultithreading() throw();
};
