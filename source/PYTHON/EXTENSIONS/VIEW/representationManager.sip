// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class RepresentationManager
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/representationManager.h>
	#include <list>
	using namespace BALL;
	using namespace VIEW;
	using namespace std;
%End
  public:
  // BALL_CREATE(RepresentationManager);
  // list<Representation*> changedComposite(const Composite&) throw();
  
	RepresentationManager(MainControl* = 0) throw();
  RepresentationManager(const RepresentationManager&) throw();
  //virtual ~RepresentationManager() throw();
  
  //const RepresentationManager& operator = (const RepresentationManager&) throw();
  bool operator == (const RepresentationManager&) const throw();
  void clear() throw();
  void remove(Representation&, bool = true) throw();
  void insert(Representation&, bool = true) throw();
  const list<Representation*>& getRepresentations() const throw();
  Size getNumberOfRepresentations() const throw();
  Representation* createRepresentation() throw();
  bool has(const Representation&) const throw();
	
  // void dump(std::ostream& s, Size depth) const throw();
  // RepresentationsConstIterator begin() const throw();
  // RepresentationsConstIterator end() const throw();
  // RepresentationsIterator begin() throw();
  // RepresentationsIterator end() throw();
	list<Representation*> removedComposite(const Composite&, bool = true) throw();
  list<Representation*> getRepresentationsOf(const Composite&) throw();
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
