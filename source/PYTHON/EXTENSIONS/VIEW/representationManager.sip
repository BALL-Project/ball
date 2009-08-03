// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class RepresentationManager
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representationManager.h>
	using namespace BALL;
	using namespace VIEW;
%End
  public:
  // BALL_CREATE(RepresentationManager);
  // RepresentationList changedComposite(const Composite& /composite/) throw();
  
	RepresentationManager(MainControl* = 0) throw();
  RepresentationManager(const RepresentationManager& /pm/) throw();
  //virtual ~RepresentationManager() throw();
  
  //const RepresentationManager& operator = (const RepresentationManager& /pm/) throw();
  bool operator == (const RepresentationManager& /pm/) const throw();
  void clear() throw();
  void remove(Representation& /representation/, bool = true) throw();
  void insert(Representation& /representation/, bool = true) throw();
  const RepresentationList& getRepresentations() const throw();
  Size getNumberOfRepresentations() const throw();
  Representation* createRepresentation() throw();
  bool has(const Representation& /representation/) const throw();
	
  // void dump(std::ostream& s, Size depth) const throw();
  // RepresentationsConstIterator begin() const throw();
  // RepresentationsConstIterator end() const throw();
  // RepresentationsIterator begin() throw();
  // RepresentationsIterator end() throw();
	RepresentationList removedComposite(const Composite&, bool = true) throw();
  RepresentationList getRepresentationsOf(const Composite&) throw();
	void rebuildAllRepresentations() throw();
	//const vector<ClippingPlane*>& getClippingPlanes() const;
	bool removeClippingPlane(ClippingPlane*);
	void insertClippingPlane(ClippingPlane*);
	void storeRepresentations(INIFile&);
  //void restoreRepresentations(const INIFile&, const vector<const Composite*>&);
	void focusRepresentation(const Representation&);
	bool willBeUpdated(const Representation&) const throw();
	bool updateRunning() const throw();
	bool isBeeingRendered(const Representation*) const;
	bool startRendering(Representation*);
	void finishedRendering(Representation*);
};
