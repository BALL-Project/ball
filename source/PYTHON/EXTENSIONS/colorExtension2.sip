// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorExtension2.sip,v 1.1 2003/11/24 23:42:13 amoll Exp $
//

class ColorExtension2 
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorExtension2.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const ColorExtension2& operator = (const ColorExtension2& color_extension) throw();
  ColorExtension2() throw();
  ColorExtension2(const ColorExtension2&) throw();
  ColorRGBA& getColor2() throw();
  const ColorRGBA& getColor2() const throw();
  void clear() throw();
  void getColor2(ColorRGBA&) const throw();
  void getColor2(ColorUnit&, ColorUnit&, ColorUnit&) const throw();
  void getColor2(ColorUnit&, ColorUnit&, ColorUnit&, ColorUnit&) const throw();
  void set(const ColorExtension2&) throw();
  void setColor2(const ColorRGBA&) throw();
  void setColor2(const ColorUnit&, const ColorUnit&, const ColorUnit&, const ColorUnit&);
  void swap(ColorExtension2&) throw();
  ~ColorExtension2() throw();
};
