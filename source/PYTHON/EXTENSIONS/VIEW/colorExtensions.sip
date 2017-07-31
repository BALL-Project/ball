class ColorExtension2
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorExtensions.h>
%End
	public:
	ColorExtension2() throw();
	ColorExtension2(const ColorExtension2&) throw();
	~ColorExtension2() throw();
	virtual void clear() throw();
	void set(const ColorExtension2&) throw();
	void swap(ColorExtension2&) throw();
	void setColor2(const ColorRGBA&) throw();
	ColorRGBA& getColor2() throw();
	void getColor2(ColorRGBA&) const throw();
};

class MultiColorExtension
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorExtensions.h>
%End
	public:
	MultiColorExtension() throw();
	MultiColorExtension(const MultiColorExtension&) throw();
	~MultiColorExtension() throw();
	virtual void clear() throw();
	void set(const MultiColorExtension&) throw();
	void setColors(const vector<ColorRGBA>& colors) throw();
	vector<ColorRGBA>& getColors();
};
