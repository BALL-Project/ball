class ElementColorProcessor
	: ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	ElementColorProcessor() throw();
	void setTransparency(Size) throw();
	virtual void getColor(const Composite&, ColorRGBA&);
};

class ResidueNumberColorProcessor
	: ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	ResidueNumberColorProcessor() throw();
	void getColor(const Composite&, ColorRGBA&);
	void setFirstColor(const ColorRGBA&);
	void setLastColor(const ColorRGBA&);
	void setMiddleColor(const ColorRGBA&);
	const ColorRGBA& getFirstColor() const;
	const ColorRGBA& getMiddleColor() const;
	const ColorRGBA& getLastColor() const;
	virtual bool start() throw();
};

class PositionColorProcessor
	: ColorProcessor
{

%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	PositionColorProcessor();
	virtual void getColor(const Composite&, ColorRGBA&);
	bool start() throw();
};

class ChainColorProcessor
	: PositionColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	ChainColorProcessor();
};

class MoleculeColorProcessor
	: PositionColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	MoleculeColorProcessor();
};

class ResidueNameColorProcessor
	: ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	ResidueNameColorProcessor() throw();
	void setTransparency(Size) throw();
	virtual void getColor(const Composite&, ColorRGBA&);
	StringHashMap<ColorRGBA> & getColorMap();
};

class AtomChargeColorProcessor
	: InterpolateColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	AtomChargeColorProcessor() throw();
	AtomChargeColorProcessor(const AtomChargeColorProcessor&) throw();
	virtual void getColor(const Composite&, ColorRGBA&);
};

class AtomDistanceColorProcessor
	: ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	AtomDistanceColorProcessor() throw();
	AtomDistanceColorProcessor(const AtomDistanceColorProcessor&) throw();
	void setDistance(float) throw();
	float getDistance() const throw();
	void setNullDistanceColor(const ColorRGBA&) throw();
	const ColorRGBA& getNullDistanceColor() const throw();
	void setMaxDistanceColor(const ColorRGBA&) throw();
	const ColorRGBA& getMaxDistanceColor() const throw();
	void calculateDistances() throw();
	virtual void addAtom(const Atom&);
	virtual bool finish() throw();
	virtual void getColor(const Composite&, ColorRGBA&);
	bool showSelected();
	void setShowSelected(bool);
};

class CustomColorProcessor
	: ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	CustomColorProcessor();
};

class TemperatureFactorColorProcessor
	: InterpolateColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	TemperatureFactorColorProcessor();
	virtual void getColor(const Composite&, ColorRGBA&);
};

class OccupancyColorProcessor
	: ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	OccupancyColorProcessor();
	virtual void getColor(const Composite&, ColorRGBA&);
};

class ForceColorProcessor
	: InterpolateColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	ForceColorProcessor();
	virtual void getColor(const Composite&, ColorRGBA&);
};

class SecondaryStructureColorProcessor
	: ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	SecondaryStructureColorProcessor();
	virtual void getColor(const Composite&, ColorRGBA&);
	const ColorRGBA& getCoilColor() const throw();
	const ColorRGBA& getHelixColor() const throw();
	const ColorRGBA& getStrandColor() const throw();
	const ColorRGBA& getTurnColor() const throw();
	void setCoilColor(const ColorRGBA&) throw();
	void setHelixColor(const ColorRGBA&) throw();
	void setStrandColor(const ColorRGBA&) throw();
	void setTurnColor(const ColorRGBA&) throw();
};

class ResidueTypeColorProcessor
	: ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/standardColorProcessor.h>
%End
	public:
	ResidueTypeColorProcessor();
	virtual void getColor(const Composite&, ColorRGBA&);
	void setBasicColor(const ColorRGBA&) throw();
	void setAcidicColor(const ColorRGBA& ) throw();
	void setPolarColor(const ColorRGBA& ) throw();
	void setHydrophobicColor(const ColorRGBA& ) throw();
	void setAromaticColor(const ColorRGBA& ) throw();
	void setOtherColor(const ColorRGBA& ) throw();
	const ColorRGBA& getBasicColor() const throw();
	const ColorRGBA& getAcidicColor() const throw();
	const ColorRGBA& getPolarColor() const throw();
	const ColorRGBA& getHydrophobicColor() const throw();
	const ColorRGBA& getAromaticColor() const throw();
	const ColorRGBA& getOtherColor() const throw();
	void setTransparency(Size ) throw();
};
