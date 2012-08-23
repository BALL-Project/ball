// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//
class CompositeManager
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/compositeManager.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:

 	CompositeManager() throw();
  CompositeManager(const CompositeManager&) throw();
  ~CompositeManager() throw();
  virtual void clear() throw();
  bool has(const Composite*) const throw();
  bool hasRoot(const Composite*) const throw();
  bool insert(Composite&) throw();
  void remove(Composite&, bool = true) throw();
  Size getNumberOfComposites() const throw();
	
	// CompositeConstIterator begin() const throw();
  // CompositeConstIterator end() const throw();
  // CompositeIterator begin() throw();
  // CompositeIterator end() throw();

	PyCompositeList getComposites() throw();
	//const PyCompositeList& getComposites() const throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
};
