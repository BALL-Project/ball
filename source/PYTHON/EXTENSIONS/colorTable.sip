// $Id: colorTable.sip,v 1.1.2.2 2005/11/16 11:29:45 oliver Exp $

class ColorTable
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorTable.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
	ColorTable() throw();
	ColorTable(Size) throw();
	ColorTable(const ColorTable&) throw();
	ColorTable(Size, const ColorRGBA& , bool =false) throw();
	ColorTable(const ColorTable&, Index, Index, bool =false) throw();
	ColorTable(const ColorRGBA*, Size, bool =false) throw();
	~ColorTable()	throw();
  void setBaseColors(const ColorRGBA*, Size) throw();
	void setNumberOfColors(const Size) throw();
	Size getNumberOfColors() const throw();
	void setAlphaBlending(bool) throw();
	bool getAlphaBlending() const throw();
	Size createTable() throw();
	void setMinMaxColors(ColorRGBA, ColorRGBA) throw();
	void setRange(float, float) throw();
	ColorRGBA& map(float) throw();
	const ColorRGBA& map(float) const throw();
	//virtual void dump(std::ostream&  = std::cout, Size = 0) const;
};
