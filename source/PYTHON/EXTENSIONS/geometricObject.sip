// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricObject.sip,v 1.4.4.1 2005/11/11 17:21:22 leonhardt Exp $
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
  GeometricObject() throw();
  GeometricObject(const GeometricObject& /object/) throw();
  ~GeometricObject() throw();
  virtual void clear() throw();
  void set(const GeometricObject& /object/) throw();
  // GeometricObject& operator = (const GeometricObject& object) throw();
  void swap(GeometricObject& /object/) throw();
  void setComposite(const Composite* /composite/) throw();
  const Composite* getComposite() const throw();
  virtual bool isValid() const throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  void setColor(const ColorRGBA& /color/) throw();
  void setColor(const ColorUnit& /red/, const ColorUnit& /green/, const ColorUnit& /blue/, const ColorUnit& /alpha/ = 1.0) throw();
  const ColorRGBA& getColor() const throw();
	ColorRGBA& getColor() throw();
  void getColor(ColorUnit& /red/, ColorUnit& /green/, ColorUnit& /blue/, ColorUnit& /alpha/) const throw();
	//bool operator == (const GeometricObject& object) const throw();
	//bool operator < (const GeometricObject& object) const throw();

};
