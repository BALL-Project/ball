// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricObject.sip,v 1.2 2004/05/19 13:19:05 amoll Exp $
//

class GeometricObject
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/geometricObject.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

  // BALL_CREATE(GeometricObject);
  // GeometricObject& operator = (const GeometricObject& object) throw();
  GeometricObject() throw();
  GeometricObject(const GeometricObject& /object/) throw();
  String& getName() throw();
  bool hasName() const throw();
  bool isValid() const throw();
  const ColorRGBA& getColor() const throw();
  const Composite* getComposite() const throw();
  const String& getName() const throw();
  void clear() throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  void getColor(ColorUnit& /red/, ColorUnit& /green/, ColorUnit& /blue/, ColorUnit& /alpha/) const throw();
  void set(const GeometricObject& /object/) throw();
  void setColor(const ColorRGBA& /color/) throw();
  void setColor(const ColorUnit& /red/, const ColorUnit& /green/, const ColorUnit& /blue/, const ColorUnit& /alpha/ = 1.0) throw();
  void setComposite(const Composite* /composite/) throw();
  void setName(const String& /name/) throw();
  void swap(GeometricObject& /object/) throw();
  ~GeometricObject() throw();
};
