// $Id: colorExtension5.h,v 1.1 1999/08/26 07:53:15 oliver Exp $

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION5_H
#define BALL_VIEW_KERNEL_COLOREXTENSION5_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION4_H
#	include <BALL/VIEW/KERNEL/colorExtension4.h>
#endif

namespace BALL
{

	namespace VIEW
	{

		/**
		*/
		class ColorExtension5
			: public ColorExtension4
		{
			public:
					
			/**	@name Constructors and Destructors	
			*/
			//@{

			ColorExtension5();

			ColorExtension5
				(const ColorExtension5& color_extension, bool deep = true);

			virtual ~ColorExtension5();

			virtual void clear();

			virtual void destroy();
		
			//@}
			
			/**	@name	Assignment
			*/
			//@{

			void set
				(const ColorExtension5& color_extension, bool deep = true);

			ColorExtension5& operator = (const ColorExtension5& color_extension);

			void get(ColorExtension5& color_extension, bool deep = true) const;

			void swap(ColorExtension5& color_extension);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setColor5(const ColorRGBA& color);
			
			void setColor5
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = 255);
			
			ColorRGBA& getColor5();

			const ColorRGBA& getColor5() const;

			void getColor5(ColorRGBA& color) const;

			void getColor5
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const;

			void getColor5
				(ColorUnit& red, ColorUnit& green, ColorUnit& blue) const;

			void setColors
				(const ColorRGBA& a,
				 const ColorRGBA& b,
				 const ColorRGBA& c,
				 const ColorRGBA& d,
				 const ColorRGBA& e);
				
			void getColors
				(ColorRGBA& a, ColorRGBA& b,
				 ColorRGBA& c, ColorRGBA& d,
				 ColorRGBA& e) const;

			//@}
			
			/**	@name	Debugging and Diangostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(ostream& s = cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/	
			//@{

			virtual void read(istream& s);

			virtual void write(ostream& s) const;
			//@}

			private:
		
			/* color 5 */
			ColorRGBA color5_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension5.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_COLOREXTENSION5_H 
