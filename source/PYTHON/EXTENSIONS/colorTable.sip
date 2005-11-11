class ColorTable
// :ColorRGBAVector
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorTable.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

		//BALL_CREATE(ColorTable)
		ColorTable() throw();
		ColorTable(Size) throw();
		ColorTable(const ColorTable&) throw();
		ColorTable(Size, const ColorRGBA& , bool =false) throw();
		ColorTable(const ColorTable&, Index, Index, bool =false) throw();
		ColorTable(const ColorRGBA*, Size, bool =false) throw();
		//virtual ~ColorTable() throw();
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
