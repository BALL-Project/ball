// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: compositeManager.sip,v 1.1 2003/11/15 10:59:05 oliver Exp $
//
class CompositeManager
{
%TypeHeaderCode
	#include <BALL/MATHS/sphere3.h>
	using namespace BALL;
%End
	public:

 	// CompositeConstIterator begin() const throw();
  // CompositeConstIterator end() const throw();
  // CompositeIterator begin() throw();
  // CompositeIterator end() throw();
  CompositeManager() throw();
  CompositeManager(const CompositeManager& /cm/) throw();
  Size getNumberOfComposite() const throw();
  bool has(const Composite& /composite/) const throw();
  bool insert(Composite& /composite/) throw();
  bool remove(Composite& /composite/) throw();
  void clear() throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  ~CompositeManager() throw();
};
