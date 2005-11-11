// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: primitiveManager.sip,v 1.10.4.3 2005/11/11 17:21:24 leonhardt Exp $
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
  
	PrimitiveManager(MainControl* = 0) throw();
  PrimitiveManager(const PrimitiveManager& /pm/) throw();
  //virtual ~PrimitiveManager() throw();
  
  //const PrimitiveManager& operator = (const PrimitiveManager& /pm/) throw();
  bool operator == (const PrimitiveManager& /pm/) const throw();
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
	bool willBeUpdated(const Representation&) const throw();
	bool updateRunning() const throw();
	void setUpdatePending(bool);
	bool updatePending();
	//HashSet<Representation*>& getRepresentationsBeeingUpdated();
	//HashSet<Representation*>& getRepresentationsBeeingDrawn();
	//const vector<ClippingPlane*>& getClippingPlanes() const;
	//bool removeClippingPlane(ClippingPlane*);
	//void insertClippingPlane(ClippingPlane*);
	void storeRepresentations(INIFile&);
  //void restoreRepresentations(const INIFile&, const vector<const Composite*>&);
	void focusRepresentation(const Representation&);
};
