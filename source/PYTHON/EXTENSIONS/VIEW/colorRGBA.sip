class ColorRGBA
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorRGBA.h>
	#include <BALL/VIEW/DATATYPE/colorUnit.h>
	using NotInHexFormat = ColorUnit::NotInHexFormat;
%End
	public:

	ColorRGBA() throw();
	ColorRGBA(const ColorRGBA&) throw();
	ColorRGBA(const char*) throw(InvalidRange, NotInHexFormat);
	ColorRGBA(const String&) throw(InvalidRange, NotInHexFormat);
	ColorRGBA(float, float, float, float alpha = 1.0) throw();
	void clear() throw();
	void set(const ColorRGBA&) throw();
	void set(const char*) throw(InvalidRange, NotInHexFormat);
	void get(char*) const throw();
	void set(const String&) throw(InvalidRange, NotInHexFormat);
	void get(String&) const throw();
	void swap(ColorRGBA&) throw();
	void setRed(const ColorUnit&) throw();
	ColorUnit& getRed() throw();
	void setGreen(const ColorUnit&) throw();
	ColorUnit& getGreen() throw();
	void setBlue(const ColorUnit&) throw();
	ColorUnit& getBlue() throw();
	void setAlpha(const ColorUnit&) throw();
	ColorUnit& getAlpha() throw();
	void set(const ColorUnit&, const ColorUnit&, const ColorUnit&,
		const ColorUnit& alpha_value = 1.0) throw();
	void get(ColorUnit&, ColorUnit&, ColorUnit&, ColorUnit&) const throw();
	void get(ColorUnit&, ColorUnit&, ColorUnit&) const throw();
	bool operator == (const ColorRGBA&) const throw();
	bool operator !=  (const ColorRGBA&) const throw();
	bool operator < (const ColorRGBA&) const throw();
	bool operator <= (const ColorRGBA&) const throw();
	bool operator > (const ColorRGBA&) const throw();
	bool operator >= (const ColorRGBA&) const throw();

	SIP_PYOBJECT __str__();
%MethodCode
	String s("ColorRGBA {");
	s += (String) *sipCpp;
	s += " }";
	sipRes = PyString_FromString(s.c_str());
%End

	SIP_PYOBJECT __repr__();
%MethodCode
	String s("ColorRGBA {");
	s += (String) *sipCpp;
	s += " }";
	sipRes = PyString_FromString(s.c_str());
%End
};
