// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorExtension2.h,v 1.16 2003/08/26 08:05:06 oliver Exp $
//

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION2_H
#define BALL_VIEW_KERNEL_COLOREXTENSION2_H

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** ColorExtension2 class.
				The class ColorExtension2 is used as a base class for all geometric 
				objects that needs two colors ColorRGBA. It provides the derived
				class with methods for accessing these colors.
		*/
		class ColorExtension2
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new colorExtension2.
					The colors of this colorExtension2 are set to (0.0, 0.0, 0.0, 1.0).
					\return      ColorExtension2 new constructed colorExtension2
					\see         ColorRGBA
			*/
			ColorExtension2()
				throw();

			/** Copy constructor.
					Construct new colorExtension2 by copying the colorExtension2 
					<b> color_extension</b>. The colors of this colorExtension2 are
					initialized to the colors of <b> color_extension</b>.
					\param       color_extension the colorExtension2 to be copied
					\return      ColorExtension2 new constructed colorExtension2 copied from <b> color_extension</b>
					\see         ColorRGBA
			*/
			ColorExtension2(const ColorExtension2& color_extension)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of this colorExtension2.
			*/
			virtual ~ColorExtension2()
				throw();

			/** Explicit default initialization.
					Set the colors of this colorExtension2 to the color 
					(0.0, 0.0, 0.0, 1.0).
					Calls ColorRGBA::clear.
					\see ColorRGBA::clear
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the colorExtension2 <b> color_extension</b> to this 
					colorExtension2.
					The colors of this colorExtension2 are initialized to the colors of 
					the colorExtension2 <b> color_extension</b>.
					Calls ColorRGBA::set.
					\param       color_extension the colorExtension2 to be copied
					\see         ColorRGBA
					\see         ColorRGBA::set
			*/
			void set(const ColorExtension2& color_extension)
				throw();

			/** Assignment operator.
					Assign the colorExtension2 <b> color_extension</b> to this
					colorExtension2.
					Calls set.
					The colors of this colorExtension2 are initialized to the colors 
					of the colorExtension2 <b> color_extension</b>.\par
					\param       color_extension the colorExtension2 to be copied
					\return      ColorExtension2& const reference of this colorExtension2
					\see         set
			*/
			const ColorExtension2& operator = (const ColorExtension2& color_extension)
				throw();

			/** Copying.
					Copy this colorExtension2 to the colorExtension2
					<b> color_extension</b>.
					Calls set.
					The colors of the colorExtension2 <b> color_extension</b> 
					are initialized to the colors of this colorExtension2.\par
					\param       color_extension the colorExtension2 to be assigned to
					\see         set
			*/
			void get(ColorExtension2& color_extension) const
				throw();

			/** Swapping of colorExtensions.
					Swap the colors of this colorExtension2 with the colorExtension2
					<b> color_extension</b>.
					Calls ColorRGBA::swap.
					\param       color_extension the colorExtension2 being swapped with this colorExtension 
					\see         ColorExtension2
					\see         ColorRGBA::swap
			*/
			void swap(ColorExtension2& color_extension)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the second color of this colorExtension2.
					Change the second color of this colorExtension2 to the color
					represented by the parameter <b> color</b>.
					\param       color the new second color of this colorExtension2
					\see         getColor2
					\see         ColorRGBA
			*/
			void setColor2(const ColorRGBA& color)
				throw();
			
			/** Change the second color of this colorExtension2.
					Change the second color of this colorExtension2 to the color 
					represented	by the parameters <b> red</b>, <b> green</b>, <b> blue</b> and
					<b> alpha</b>.
					\param       red the red component of the new second color of this colorExtension2
					\param       green the green component of the new second color of this colorExtension2
					\param       blue the blue component of the new second color of this colorExtension2
					\param       alpha the alpha component of the new second color of this colorExtension2 (Default=1.0)
					\see         getColor2
					\see         ColorUnit
			*/
			void setColor2
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1)
				throw();
			
			/** Inspection of all the components of the second color of this
					colorExtension2.
					Access all the components of the second color of this colorExtension2
					by using ColorUnit.
					\param       red the red component of the second color of this colorExtension2
					\param       green the green component of the second color of this colorExtension2
					\param       blue the blue component of the second color of this colorExtension2
					\param       alpha the alpha component of the second color of this colorExtension2
					\see         setColor2
					\see         ColorUnit
			*/
			void getColor2
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const
				throw();
			
			/** Inspection of the components of the second color of this
					colorExtension2.
					Access only the red, green and blue components of the second color 
					of this colorExtension2	by using ColorUnit.
					\param       red the red component of the second color of this colorExtension2
					\param       green the green component of the second color of this colorExtension2
					\param       blue the blue component of the second color of this colorExtension2
					\see         setColor2
					\see         ColorUnit
			*/
			void getColor2
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue) const
				throw();
			
			/** Mutable inspection of the second color of this colorExtension2.
					Access the mutual reference of the second color of this
					colorExtension2.
					\return      ColorRGBA& mutable reference to the second color of this colorExtension2
					\see         setColor2
					\see         ColorRGBA
			*/
			ColorRGBA& getColor2()
				throw();

			/** Non-mutable inspection of the second color of this colorExtension2.
					For further information see getColor2.
			*/
			const ColorRGBA& getColor2() const
				throw();

			/** Inspection of the second color of this colorExtension2.
					Access the second color of this colorExtension2 by using
					ColorRGBA.
					\param       color the color receiving the second color of this colorExtension2
					\see         setColor2
					\see         ColorRGBA
			*/
			void getColor2(ColorRGBA& color) const
				throw();

			//@} 

			/**	@name	debuggers and diagnostics */	
			//@{

			/** Internal value dump.
					Dump the current state of this colorExtension2 to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this colorExtension2
					\param   depth the dumping depth
					\see     ColorRGBA::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			private:
		
			/* color 2 */
			ColorRGBA color2_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension2.iC>
#		endif
  
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_COLOREXTENSION2_H
