// $Id: colorExtension4.h,v 1.1 1999/08/26 07:53:14 oliver Exp $

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION4_H
#define BALL_VIEW_KERNEL_COLOREXTENSION4_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION3_H
#	include <BALL/VIEW/KERNEL/colorExtension3.h>
#endif

namespace BALL
{
	
	namespace VIEW
	{

		/**
		*/
		class ColorExtension4
			: public ColorExtension3
		{
			public:
		
			/**	@name Constructors and Destructors
			*/
			//@{

			ColorExtension4();

			ColorExtension4
				(const ColorExtension4& color_extension, bool deep = true);

			virtual ~ColorExtension4();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Assignment
			*/
			//@{

			void set
				(const ColorExtension4& color_extension, bool deep = true);

			ColorExtension4& operator = (const ColorExtension4& color_extension);

			void get
				(ColorExtension4& color_extension, bool deep = true) const;

			void swap(ColorExtension4& color_extension);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setColor4(const ColorRGBA& color);
			
			void setColor4
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = 255);
			
			ColorRGBA& getColor4();

			const ColorRGBA& getColor4() const;

			void getColor4(ColorRGBA& color) const;

			void getColor4
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const;

			void getColor4
				(ColorUnit& red, ColorUnit& green, ColorUnit& blue) const;

			void setColors
				(const ColorRGBA& a, const ColorRGBA& b,
				 const ColorRGBA& c, const ColorRGBA& d);
				
			void getColors
				(ColorRGBA& a, ColorRGBA& b,
				 ColorRGBA& c, ColorRGBA& d) const;

			//@}
			
			/**	@name	Debugging and Diagnostics
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
		
			/* color 4 */
			ColorRGBA color4_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension4.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_COLOREXTENSION4_H 
