class NotInHexFormat
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorUnit.h>
	using NotInHexFormat = ColorUnit::NotInHexFormat;
%End
	public:
	NotInHexFormat(const char*, int, const String&) throw();
};

class ColorUnit
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorUnit.h>
	using NotInHexFormat = ColorUnit::NotInHexFormat;
%End
	public:
	ColorUnit() throw();
	ColorUnit(const ColorUnit&) throw();
	ColorUnit(const String&) throw(InvalidRange, NotInHexFormat);
	ColorUnit(const unsigned int) throw(InvalidRange);
	void clear() throw();
	void set(const unsigned int) throw(InvalidRange);
	void get(unsigned int&) const throw();

	bool operator == (const ColorUnit&) const throw();
	bool operator <= (const ColorUnit&) const throw();
	bool operator > (const ColorUnit&) const throw();
	bool operator >= (const ColorUnit&) const throw();
};
