class ColorMap
// :ColorRGBAVector
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorMap.h>
  #include <BALL/PYTHON/pyColorRGBAVector.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

		//BALL_CREATE(ColorMap)
		ColorMap() throw();
		ColorMap(Size) throw();
		ColorMap(const ColorMap&) throw();
		ColorMap(Size, const ColorRGBA& , bool =false) throw();
		ColorMap(const ColorMap&, Index, Index, bool =false) throw();
		ColorMap(const ColorRGBA*, Size, bool =false) throw();
		//virtual ~ColorMap() throw();
	  void setBaseColors(const PyColorRGBAVector&) throw();
%MethodCode
		sipCpp->clear();
		for (Position p = 0; p < (*a0).size(); p++)
		{
			sipCpp->push_back((*a0)[p]);
		}
%End
	//  void setBaseColors(const ColorRGBA*, Size) throw();
		void setNumberOfColors(const Size) throw();
		Size getNumberOfColors() const throw();
		void setAlphaBlending(bool) throw();
		bool getAlphaBlending() const throw();
		Size createMap() throw();
		void setMinMaxColors(ColorRGBA, ColorRGBA) throw();
		void setRange(float, float) throw();
		ColorRGBA& map(float) throw();
		// const ColorRGBA& map(float) const throw();
		//virtual void dump(std::ostream&  = std::cout, Size = 0) const;
};
