// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: primitiveManager.sip,v 1.4 2004/11/12 15:04:25 amoll Exp $
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
  void insert(Representation& /representation/) throw();
  void remove(Representation& /representation/) throw();
  ~PrimitiveManager() throw();

	bool willBeUpdated(const Representation& rep) const throw();
	void setMainControl(MainControl& mc) throw();
	/// Return true, if a Representation is currently beeing updated
	bool updateRunning() const throw();
	void willUpdateSoon() throw();
	bool updateStillToBeStarted() const throw();
};
