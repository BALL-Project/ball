// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: compositeManager.sip,v 1.4 2003/11/17 18:11:24 amoll Exp $
//
class CompositeManager
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/compositeManager.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:

 	// CompositeConstIterator begin() const throw();
  // CompositeConstIterator end() const throw();
  // CompositeIterator begin() throw();
  // CompositeIterator end() throw();
  CompositeManager() throw();
  CompositeManager(const CompositeManager& /cm/) throw();
  Size getNumberOfComposites() const throw();
  bool has(const Composite& /composite/) const throw();
  bool insert(Composite& /composite/) throw();
  bool remove(Composite& /composite/) throw();
  void clear() throw();
	PyCompositeList getComposites() throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  ~CompositeManager() throw();
};
