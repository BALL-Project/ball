// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ColorExtension2 
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorExtensions.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
  ColorExtension2() throw();
  ColorExtension2(const ColorExtension2&) throw();
  ~ColorExtension2() throw();
  virtual void clear() throw();
  void set(const ColorExtension2&) throw();
//  const ColorExtension2& operator = (const ColorExtension2& color_extension) throw();
  void swap(ColorExtension2&) throw();
  void setColor2(const ColorRGBA&) throw();
//void setColor2(const ColorUnit&, const ColorUnit&, const ColorUnit&, const ColorUnit& =1.0);
//void getColor2(ColorUnit&, ColorUnit&, ColorUnit&, ColorUnit&) const throw();
//void getColor2(ColorUnit&, ColorUnit&, ColorUnit&) const throw();
  ColorRGBA& getColor2() throw();
  // const ColorRGBA& getColor2() const throw();
  void getColor2(ColorRGBA&) const throw();
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
};

class MultiColorExtension
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorExtensions.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
  MultiColorExtension() throw();
  MultiColorExtension(const MultiColorExtension&) throw();
  ~MultiColorExtension() throw();
  virtual void clear() throw();
  void set(const MultiColorExtension&) throw();
//  const ColorExtension2& operator = (const ColorExtension2& color_extension) throw();
	void setColors(const vector<ColorRGBA>& colors) throw();
	vector<ColorRGBA>& getColors();
};
