// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleBox.sip,v 1.1 2003/11/24 22:46:32 amoll Exp $
//

class SimpleBox 
  : SimpleBox3,
    GeometricObject
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const SimpleBox& operator = (const SimpleBox& box) throw();
  SimpleBox() throw();
  SimpleBox(const SimpleBox&) throw();
  bool isValid() const throw();
  void clear() throw();
  void set(const SimpleBox&) throw();
  ~SimpleBox() throw();
};
