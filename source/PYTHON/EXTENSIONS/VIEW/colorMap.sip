class ColorMap
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorMap.h>
	#include <BALL/PYTHON/EXTENSIONS/pyColorRGBAVector.h>
%End
	public:

	ColorMap() throw();
	ColorMap(Size) throw();
	ColorMap(const ColorMap&) throw();
	ColorMap(Size, const ColorRGBA& , bool =false) throw();
	ColorMap(const ColorMap&, Index, Index, bool =false) throw();
	ColorMap(const ColorRGBA*, Size, bool =false) throw();
	void setBaseColors(const PyColorRGBAVector&) throw();
%MethodCode
	sipCpp->clear();
	for (Position p = 0; p < (*a0).size(); p++)
	{
		sipCpp->push_back((*a0)[p]);
	}
%End

	void setNumberOfColors(const Size) throw();
	Size getNumberOfColors() const throw();
	void setAlphaBlending(bool) throw();
	bool getAlphaBlending() const throw();
	Size createMap() throw();
	void setMinMaxColors(ColorRGBA, ColorRGBA) throw();
	void setRange(float, float) throw();
	ColorRGBA& map(float) throw();
};
