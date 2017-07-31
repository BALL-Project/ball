class ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/colorProcessor.h>
%End
	public:

	ColorProcessor() throw();
	ColorProcessor(const ColorProcessor&) throw();
	~ColorProcessor();
	virtual void clear() throw();
	void set(const ColorProcessor&) throw();
	virtual bool start() throw();
	bool updateAlwaysNeeded();
	void setDefaultColor(const ColorRGBA&) throw();
	const ColorRGBA& getDefaultColor() const throw();
	void getColor(const Composite&, ColorRGBA&);
	Size getTransparency() const throw();
	void setTransparency(Size) throw();
	void setAdditionalGridDistance(float) throw();
	float getAdditionalGridDistance() const throw();
	void setModelType(ModelType);
	const Atom* getClosestItem(const Vector3&) const throw();
};


class InterpolateColorProcessor
	: ColorProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/colorProcessor.h>
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
