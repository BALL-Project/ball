// $Id: colorExtension3.h,v 1.2 1999/12/19 17:19:27 oliver Exp $

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION3_H
#define BALL_VIEW_KERNEL_COLOREXTENSION3_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION2_H
#	include <BALL/VIEW/KERNEL/colorExtension2.h>
#endif


namespace BALL
{
	namespace VIEW
	{

		/**	
		*/
		class ColorExtension3
			: public ColorExtension2
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			ColorExtension3();

			ColorExtension3
				(const ColorExtension3& color_extension, bool deep = true);

			virtual ~ColorExtension3();

			virtual void clear();

			virtual void destroy();
			//@}
		
			/**	@name	Assignment
			*/
			//@{

			void set
				(const ColorExtension3& color_extension, bool deep = true);

			ColorExtension3& operator = (const ColorExtension3& color_extension);

			void get
				(ColorExtension3& color_extension, bool deep = true) const;

			void swap(ColorExtension3& color_extension);
			//@}

			
			/**	@name Accessors
			*/
			//@{

			void setColor3(const ColorRGBA& color);
			
			void setColor3
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1);
			
			ColorRGBA& getColor3();

			const ColorRGBA& getColor3() const;

			void getColor3(ColorRGBA& color) const;

			void getColor3
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const;

			void getColor3
				(ColorUnit& red, ColorUnit& green, ColorUnit& blue) const;

			void setColors
				(const ColorRGBA& a, const ColorRGBA& b, const ColorRGBA& c);

			void getColors
				(ColorRGBA& a, ColorRGBA& b, ColorRGBA& c) const;
				
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
		
			/* color 3 */
			ColorRGBA color3_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension3.iC>
#		endif

	} // namespace VIEW
	
} // namespace BALL

#endif // BALL_VIEW_KERNEL_COLOREXTENSION3_H
