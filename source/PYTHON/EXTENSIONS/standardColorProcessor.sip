// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: standardColorProcessor.sip,v 1.2 2005/01/14 00:26:22 amoll Exp $
//

class AtomChargeColorProcessor
  : ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
  AtomChargeColorProcessor() throw();
  AtomChargeColorProcessor(const AtomChargeColorProcessor&) throw();
	void getColor(const Composite&, ColorRGBA&);
  const ColorRGBA& getNegativeColor() const throw();
  const ColorRGBA& getPositiveColor() const throw();
  const ColorRGBA& getNeutralColor() const throw();
  void setNegativeColor(const ColorRGBA&) throw();
  void setNeutralColor(const ColorRGBA&) throw();
  void setPositiveColor(const ColorRGBA&) throw();
};


class AtomDistanceColorProcessor
  : ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:

  AtomDistanceColorProcessor() throw();
  AtomDistanceColorProcessor(const AtomDistanceColorProcessor&) throw();
	void getColor(const Composite&, ColorRGBA&);
  void setDistance(float) throw();
  float getDistance() const throw();
  void setMaxDistanceColor(const ColorRGBA&) throw();
  void setNullDistanceColor(const ColorRGBA&) throw();
  const ColorRGBA& getMaxDistanceColor() const throw();
  const ColorRGBA& getNullDistanceColor() const throw();
  void addAtom(const Atom&);
  void calculateDistances() throw();
  bool finish() throw();
	//Processor::Result operator() (GeometricObject*&) throw();
};

  
class CustomColorProcessor
  : ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:

	CustomColorProcessor();
};


class ElementColorProcessor
  : ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:

	ElementColorProcessor() throw();
	void getColor(const Composite&, ColorRGBA&);
  //HashMap<Position, ColorRGBA> & getColorMap();
  //constHashMap<Position, ColorRGBA> & getColorMap() const;
};


class OccupancyColorProcessor
  : ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:

  OccupancyColorProcessor();
	void getColor(const Composite&, ColorRGBA&);
};

  
class ResidueNameColorProcessor
  : ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:

  ResidueNameColorProcessor() throw();
	void getColor(const Composite&, ColorRGBA&);
  //StringHashMap<ColorRGBA> & getColorMap();
  //constStringHashMap<ColorRGBA> & getColorMap() const;
};


class ResidueNumberColorProcessor
  : ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:

  ResidueNumberColorProcessor() throw();
	void getColor(const Composite&, ColorRGBA&);
  const ColorRGBA& getFirstColor() const;
  const ColorRGBA& getMiddleColor() const;
  const ColorRGBA& getLastColor() const;
  void setFirstColor(const ColorRGBA&);
  void setLastColor(const ColorRGBA&);
  void setMiddleColor(const ColorRGBA&);
  bool start() throw();
};


class TemperatureFactorColorProcessor
  : ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

  TemperatureFactorColorProcessor();
	void getColor(const Composite&, ColorRGBA&);
};


class SecondaryStructureColorProcessor
  : ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	SecondaryStructureColorProcessor();
  const ColorRGBA& getCoilColor() const throw();
  const ColorRGBA& getHelixColor() const throw();
  const ColorRGBA& getStrandColor() const throw();
  const ColorRGBA& getTurnColor() const throw();
  void setCoilColor(const ColorRGBA&) throw();
  void setHelixColor(const ColorRGBA&) throw();
  void setStrandColor(const ColorRGBA&) throw();
  void setTurnColor(const ColorRGBA&) throw();
};
