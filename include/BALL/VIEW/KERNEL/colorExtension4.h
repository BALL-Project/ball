// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorExtension4.h,v 1.15 2003/08/26 08:05:06 oliver Exp $
//

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION4_H
#define BALL_VIEW_KERNEL_COLOREXTENSION4_H

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION3_H
#	include <BALL/VIEW/KERNEL/colorExtension3.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** ColorExtension4 class.
				The class ColorExtension4 is used as a base class for all geometric 
				objects that needs four colors ColorRGBA. It provides the derived
				class with methods for accessing these colors.
		*/
		class ColorExtension4: public ColorExtension3
		{
			public:
		
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new colorExtension4.
					The colors of this colorExtension4 are set to (0.0, 0.0, 0.0, 1.0).
					Calls ColorExtension3.
					\return      ColorExtension4 new constructed colorExtension4
					\see         ColorRGBA
					\see         ColorExtension3
			*/
			ColorExtension4()
				throw();

			/** Copy constructor.
					Construct new colorExtension4 by copying the colorExtension4 
					<b> color_extension</b>. The colors of this colorExtension4 are
					initialized to the colors of <b> color_extension</b>.
					Calls ColorExtension3.
					\param       color_extension the colorExtension4 to be copied
					\return      ColorExtension4 new constructed colorExtension4 copied from <b> color_extension</b>
					\see         ColorRGBA
					\see         ColorExtension3
			*/
			ColorExtension4(const ColorExtension4& color_extension)
				throw();

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of this colorExtension4.
			*/
			virtual ~ColorExtension4()
				throw();

			/** Explicit default initialization.
					Set the colors of this colorExtension4 to the color 
					(0.0, 0.0, 0.0, 1.0).
					Calls ColorExtension3::clear.
					Calls ColorRGBA::clear.
					\see  ColorExtension3::clear
					\see  ColorRGBA::clear
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the colorExtension4 <b> color_extension</b> to this 
					colorExtension4.
					The colors of this colorExtension4 are initialized to the colors of 
					the colorExtension4 <b> color_extension</b>.
					Calls ColorExtension3::set.
					Calls ColorRGBA::set.
					\param       color_extension the colorExtension4 to be copied
					\see         ColorExtension3::set
					\see         ColorRGBA
					\see         ColorRGBA::set
			*/
			void set(const ColorExtension4& color_extension)
				throw();

			/** Assignment operator.
					Assign the colorExtension4 <b> color_extension</b> to this
					colorExtension4.
					Calls set.
					The colors of this colorExtension4 are initialized to the colors 
					of the colorExtension4 <b> color_extension</b>.\par
					\param       color_extension the colorExtension4 to be copied
					\return      ColorExtension4& constant reference of this colorExtension4
					\see         set
			*/
			const ColorExtension4& operator = (const ColorExtension4& color_extension)
				throw();

			/** Copying.
					Copy this colorExtension4 to the colorExtension4
					<b> color_extension</b>.
					Calls set.
					The colors of the colorExtension4 <b> color_extension</b> 
					are initialized to the colors of this colorExtension4.\par
					\param       color_extension the colorExtension4 to be assigned to
					\see         set
			*/
			void get(ColorExtension4& color_extension) const
				throw();

			/** Swapping of colorExtensions.
					Swap the colors of this colorExtension4 with the colorExtension4
					<b> color_extension</b>.
					Calls ColorExtension3::swap.
					Calls ColorRGBA::swap.
					\param       color_extension the colorExtension4 being swapped with this colorExtension 
					\see         ColorExtension4
					\see         ColorExtension3::swap
					\see         ColorRGBA::swap
			*/
			void swap(ColorExtension4& color_extension)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the fourth color of this colorExtension4.
					Change the fourth color of this colorExtension4 to the color
					represented by the parameter <b> color</b>.
					\param       color the new fourth color of this colorExtension4
					\see         getColor4
					\see         ColorRGBA
			*/
			void setColor4(const ColorRGBA& color)
				throw();
			
			/** Change the fourth color of this colorExtension4.
					Change the fourth color of this colorExtension4 to the color 
					represented	by the parameters <b> red</b>, <b> green</b>, <b> blue</b> and
					<b> alpha</b>.
					\param       red the red component of the new fourth color of this colorExtension4
					\param       green the green component of the new fourth color of this colorExtension4
					\param       blue the blue component of the new fourth color of this colorExtension4
					\param       alpha the alpha component of the new fourth color of this colorExtension4 (Default=255)
					\see         getColor4
					\see         ColorUnit
			*/
			void setColor4
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = 255)
				throw();
			
			/** Mutable inspection of the fourth color of this colorExtension4.
					Access the mutual reference of the fourth color of this
					colorExtension4.
					\return      ColorRGBA& mutable reference to the fourth color of this colorExtension4
					\see         setColor4
					\see         ColorRGBA
			*/
			ColorRGBA& getColor4()
				throw();

			/** Non-mutable inspection of the fourth color of this colorExtension4.
				  For further information see getColor4.
			*/
			const ColorRGBA& getColor4() const
				throw();

			/** Inspection of the fourth color of this colorExtension4.
					Access the fourth color of this colorExtension4 by using
					ColorRGBA.
					\param       color the color receiving the fourth color of this colorExtension4
					\see         setColor4
					\see         ColorRGBA
			*/
			void getColor4(ColorRGBA& color) const
				throw();

			/** Inspection of all the components of the fourth color of this colorExtension4.
					Access all the components of the fourth color of this colorExtension4
					by using ColorUnit.
					\param       red the red component of the fourth color of this colorExtension4
					\param       green the green component of the fourth color of this colorExtension4
					\param       blue the blue component of the fourth color of this colorExtension4
					\param       alpha the alpha component of the fourth color of this colorExtension4
					\see         setColor4
					\see         ColorUnit
			*/
			void getColor4
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const
				throw();

			/** Inspection of the components of the fourth color of this
					colorExtension4.
					Access only the red, green and blue components of the fourth color 
					of this colorExtension4	by using ColorUnit.
					\param       red the red component of the fourth color of this colorExtension4
					\param       green the green component of the fourth color of this colorExtension4
					\param       blue the blue component of the fourth color of this colorExtension4
					\see         setColor4
					\see         ColorUnit
			*/
			void getColor4
				(ColorUnit& red, ColorUnit& green, ColorUnit& blue) const
				throw();

			//@} 
			
			/**	@name	debuggers and diagnostics */	
			//@{

			/** Internal value dump.
					Dump the current state of this colorExtension4 to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this colorExtension4
					\param   depth the dumping depth
					\see     ColorRGBA::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
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
