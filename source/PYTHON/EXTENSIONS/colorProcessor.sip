// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.sip,v 1.2 2003/11/15 12:53:57 oliver Exp $
//

class ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/colorProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

  ColorProcessor() throw();
  ColorProcessor(const ColorProcessor& /color_calculator/) throw();
  ColorRGBA getColor(const Composite* /composite/);
  //Processor::Result operator() (GeometricObject& /object/);
//%MethodCode
//	sipRes = sipCpp->operator () (*a0);
//%End

  Size getTransparency() const throw();
  // const ColorProcessor& operator = (const ColorProcessor& /color_calculator/) throw();
  const ColorRGBA& getDefaultColor() const throw();
  // const CompositeSet* getComposites() throw();
  void clear() throw();
  void clearAtomGrid() throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  void set(const ColorProcessor& /color_calculator/) throw();
  // void setComposites(const CompositeSet* /composites/) throw();
  void setDefaultColor(const ColorRGBA& /color/) throw();
  void setTransparency(Size /value/) throw();
  ~ColorProcessor();
};
