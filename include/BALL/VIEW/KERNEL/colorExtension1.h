// $Id: colorExtension1.h,v 1.2 1999/12/19 17:19:26 oliver Exp $

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION1_H
#define BALL_VIEW_KERNEL_COLOREXTENSION1_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif


namespace BALL
{
	namespace VIEW
	{

		/**	
		*/
		class ColorExtension
		{
			public:
			
			/**	@name	Constructors and Destructors
			*/
			//@{

			ColorExtension();

			ColorExtension
				(const ColorExtension& color_extension, bool deep = true);

			virtual ~ColorExtension();

			virtual void clear();

			virtual void destroy();
			//@}
		
			/**	@name	Assignment
			*/
			//@{

			void set
				(const ColorExtension& color_extension, bool deep = true);

			ColorExtension& operator =
				(const ColorExtension& color_extension);

			void get
				(ColorExtension& color_extension, bool deep = true) const;

			void swap
				(ColorExtension& color_extension);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setColor(const ColorRGBA& color);
			
			void setColor
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1);
			
			ColorRGBA& getColor();

			const ColorRGBA& getColor() const;

			void getColor(ColorRGBA& color) const;

			void getColor
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const;

			void getColor
				(ColorUnit& red,
				 ColorUnit& green,
				 ColorUnit& blue) const;
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
		
			/* color */
			ColorRGBA color_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension1.iC>
#		endif

	} // namespace VIEW

} // namespace BALL


#endif // BALL_VIEW_KERNEL_COLOREXTENSION1_H
