// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorExtension5.h,v 1.15 2003/08/26 08:05:06 oliver Exp $
//

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION5_H
#define BALL_VIEW_KERNEL_COLOREXTENSION5_H

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION4_H
#	include <BALL/VIEW/KERNEL/colorExtension4.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** ColorExtension5 class.
				The class ColorExtension5 is used as a base class for all geometric 
				objects that needs five colors ColorRGBA. It provides the derived
				class with methods for accessing these colors.
		*/
		class ColorExtension5: public ColorExtension4
		{
			public:
					
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new colorExtension5.
					The colors of this colorExtension5 are set to (0.0, 0.0, 0.0, 1.0).
					Calls ColorExtension4.
					\return      ColorExtension5 new constructed colorExtension5
					\see         ColorRGBA
					\see         ColorExtension4
			*/
			ColorExtension5()
				throw();

			/** Copy constructor.
					Construct new colorExtension5 by copying the colorExtension5 
					<b> color_extension</b>. The colors of this colorExtension5 are
					initialized to the colors of <b> color_extension</b>.
					Calls ColorExtension4.
					\param       color_extension the colorExtension5 to be copied
					\return      ColorExtension5 new constructed colorExtension5 copied from <b> color_extension</b>
					\see         ColorRGBA
					\see         ColorExtension4
			*/
			ColorExtension5(const ColorExtension5& color_extension)
				throw();

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of this colorExtension5.
			*/
			virtual ~ColorExtension5()
				throw();

			/** Explicit default initialization.
					Set the colors of this colorExtension5 to the color 
					(0.0, 0.0, 0.0, 1.0).
					Calls ColorExtension4::clear.
					Calls ColorRGBA::clear.
					\see  ColorExtension4::clear
					\see  ColorRGBA::clear
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the colorExtension5 <b> color_extension</b> to this 
					colorExtension5.
					The colors of this colorExtension5 are initialized to the colors of 
					the colorExtension5 <b> color_extension</b>.
					Calls ColorExtension4::set.
					Calls ColorRGBA::set.
					\param       color_extension the colorExtension5 to be copied
					\see         ColorExtension4::set
					\see         ColorRGBA
					\see         ColorRGBA::set
			*/
			void set(const ColorExtension5& color_extension)
				throw();

			/** Assignment operator.
					Assign the colorExtension5 <b> color_extension</b> to this
					colorExtension5.
					Calls set.
					The colors of this colorExtension5 are initialized to the colors 
					of the colorExtension5 <b> color_extension</b>.\par
					\param       color_extension the colorExtension5 to be copied
					\return      ColorExtension5& constant reference of this colorExtension5
					\see         set
			*/
			const ColorExtension5& operator = (const ColorExtension5& color_extension)
				throw();

			/** Copying.
					Copy this colorExtension5 to the colorExtension5
					<b> color_extension</b>.
					Calls set.
					The colors of the colorExtension5 <b> color_extension</b> 
					are initialized to the colors of this colorExtension5.\par
					\param       color_extension the colorExtension5 to be assigned to
					\see         set
			*/
			void get(ColorExtension5& color_extension) const
				throw();

			/** Swapping of colorExtensions.
					Swap the colors of this colorExtension5 with the colorExtension5
					<b> color_extension</b>.
					Calls ColorExtension4::swap.
					Calls ColorRGBA::swap.
					\param       color_extension the colorExtension5 being swapped with this colorExtension 
					\see         ColorExtension5
					\see         ColorExtension4::swap
					\see         ColorRGBA::swap
			*/
			void swap(ColorExtension5& color_extension)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the fifth color of this colorExtension5.
					Change the fifth color of this colorExtension5 to the color
					represented by the parameter <b> color</b>.
					\param       color the new fifth color of this colorExtension5
					\see         getColor5
					\see         ColorRGBA
			*/
			void setColor5(const ColorRGBA& color)
				throw();
			
			/** Change the fifth color of this colorExtension5.
					Change the fifth color of this colorExtension5 to the color 
					represented	by the parameters <b> red</b>, <b> green</b>, <b> blue</b> and
					<b> alpha</b>.
					\param       red the red component of the new fifth color of this colorExtension5
					\param       green the green component of the new fifth color of this colorExtension5
					\param       blue the blue component of the new fifth color of this colorExtension5
					\param       alpha the alpha component of the new fifth color of this colorExtension5 (Default=255)
					\see         getColor5
					\see         ColorUnit
			*/
			void setColor5
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = 255)
				throw();
			
			/** Mutable inspection of the fifth color of this colorExtension5.
					Access the mutual reference of the fifth color of this
					colorExtension5.
					\return      ColorRGBA& mutable reference to the fifth color of this colorExtension5
					\see         setColor5
					\see         ColorRGBA
			*/
			ColorRGBA& getColor5()
				throw();

			/** Non-mutable inspection of the fifth color of this colorExtension5.
					For further information see getColor5.
			*/
			const ColorRGBA& getColor5() const
				throw();

			/** Inspection of the fifth color of this colorExtension5.
					Access the fifth color of this colorExtension5 by using
					ColorRGBA.
					\param       color the color receiving the fifth color of this colorExtension5
					\see         setColor5
					\see         ColorRGBA
			*/
			void getColor5(ColorRGBA& color) const
				throw();

			/** Inspection of all the components of the fifth color of this colorExtension5.
					Access all the components of the fifth color of this colorExtension5
					by using ColorUnit.
					\param       red the red component of the fifth color of this colorExtension5
					\param       green the green component of the fifth color of this colorExtension5
					\param       blue the blue component of the fifth color of this colorExtension5
					\param       alpha the alpha component of the fifth color of this colorExtension5
					\see         setColor5
					\see         ColorUnit
			*/
			void getColor5
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const
				throw();

			/** Inspection of the components of the fifth color of this
					colorExtension5.
					Access only the red, green and blue components of the fifth color 
					of this colorExtension5	by using ColorUnit.
					\param       red the red component of the fifth color of this colorExtension5
					\param       green the green component of the fifth color of this colorExtension5
					\param       blue the blue component of the fifth color of this colorExtension5
					\see         setColor5
					\see         ColorUnit
			*/
			void getColor5
				(ColorUnit& red, ColorUnit& green, ColorUnit& blue) const
				throw();

			//@} 

			/**	@name	debuggers and diagnostics */	
			//@{

			/** Internal value dump.
					Dump the current state of this colorExtension5 to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this colorExtension5
					\param   depth the dumping depth
					\see     ColorRGBA::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
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
