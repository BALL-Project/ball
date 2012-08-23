// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
  ColorProcessor(const ColorProcessor&) throw();
  ~ColorProcessor();
  virtual void clear() throw();
  void set(const ColorProcessor&) throw();
	virtual bool start() throw();
  // const ColorProcessor& operator = (const ColorProcessor&) throw();
	bool updateAlwaysNeeded();
  void setDefaultColor(const ColorRGBA&) throw();
  const ColorRGBA& getDefaultColor() const throw();
  //Processor::Result operator() (GeometricObject&);
	void getColor(const Composite&, ColorRGBA&);
  Size getTransparency() const throw();
  void setTransparency(Size) throw();
  // void setComposites(const CompositeSet*) throw();
  // const CompositeSet* getComposites() throw();
  //void clearAtomGrid() throw();
	void setAdditionalGridDistance(float) throw();
	float getAdditionalGridDistance() const throw();
	//AtomGrid& getAtomGrid() throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
	void setModelType(ModelType);
	//virtual void createAtomGrid(const Composite* = 0) throw();
	const Atom* getClosestItem(const Vector3&) const throw();
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
