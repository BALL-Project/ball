// $Id: colorExtension2.h,v 1.2 1999/12/19 17:19:27 oliver Exp $

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION2_H
#define BALL_VIEW_KERNEL_COLOREXTENSION2_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#include <BALL/VIEW/DATATYPE/color.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/**	
		*/
		class ColorExtension2
		{
			public:
			
			/**	@name Constructors and Destructors
			*/	
			//@{

			ColorExtension2();

			ColorExtension2
				(const ColorExtension2& color_extension, bool deep = true);

			virtual ~ColorExtension2();

			virtual void clear();

			virtual void destroy();
			//@}
		
			/**	@name	Assignment
			*/
			//@{

			void set
				(const ColorExtension2& color_extension, bool deep = true);

			ColorExtension2& operator = (const ColorExtension2& color_extension);

			void get
				(ColorExtension2& color_extension, bool deep = true) const;

			void swap(ColorExtension2& color_extension);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setColor1(const ColorRGBA& color);
			
			void setColor1
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1);
			
			ColorRGBA& getColor1();

			const ColorRGBA& getColor1() const;

			void getColor1
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const;
			
			void getColor1
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue) const;
			
			void getColor1(ColorRGBA& color) const;

			void setColor2
				(const ColorRGBA& color);
			
			void setColor2
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1);
			
			void getColor2
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const;
			
			void getColor2
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue) const;
			
			ColorRGBA& getColor2();

			const ColorRGBA& getColor2() const;

			void getColor2(ColorRGBA& color) const;

			void setColors(const ColorRGBA& a, const ColorRGBA& b);
				
			void getColors
				(ColorRGBA& a, ColorRGBA& b) const;

			//@}

			/**	@name	Debugging and Diagnostics 
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}

			private:
		
			/* color 1 */
			ColorRGBA color1_;

			/* color 2 */
			ColorRGBA color2_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension2.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_COLOREXTENSION2_H
