// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.sip,v 1.3.4.1 2005/07/27 13:20:23 amoll Exp $
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
	void getColor(const Composite&, ColorRGBA&);
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


class InterpolateColorProcessor
	: ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/colorProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	enum Mode
	{
		USE_OUTSIDE_COLOR = 0,
		DEFAULT_COLOR_FOR_OUTSIDE_COLORS,
		NO_OUTSIDE_COLORS
	};
		
	InterpolateColorProcessor();
	InterpolateColorProcessor(const InterpolateColorProcessor&);
	virtual bool start() throw();
	void setMode(Mode);
	Mode getMode() const;
	//vector<ColorRGBA>& getColors() throw();
	//const vector<ColorRGBA>& getColors() const throw();
	//void setColors(const vector<ColorRGBA>&) throw();
	void setMinColor(const ColorRGBA&) throw();
	void setMaxColor(const ColorRGBA&) throw();
	const ColorRGBA& getMinColor() const throw();
	const ColorRGBA& getMaxColor() const throw();
	void setMaxValue(float) throw();
	float getMaxValue() const throw();
	void setMinValue(float) throw();
	float getMinValue() const throw();
	virtual void interpolateColor(float, ColorRGBA&) throw();
};
