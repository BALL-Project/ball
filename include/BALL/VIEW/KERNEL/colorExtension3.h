// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorExtension3.h,v 1.16 2003/08/26 08:05:06 oliver Exp $
//

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION3_H
#define BALL_VIEW_KERNEL_COLOREXTENSION3_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION2_H
#	include <BALL/VIEW/KERNEL/colorExtension2.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** ColorExtension3 class.
				The class ColorExtension3 is used as a base class for all geometric 
				objects that needs three colors ColorRGBA. It provides the derived
				class with methods for accessing these colors.
		*/
		class ColorExtension3: public ColorExtension2
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new colorExtension3.
					The colors of this colorExtension3 are set to (0.0, 0.0, 0.0, 1.0).
					Calls ColorExtension2.
					\return      ColorExtension3 new constructed colorExtension3
					\see         ColorRGBA
					\see         ColorExtension2
			*/
			ColorExtension3()
				throw();

			/** Copy constructor.
					Construct new colorExtension3 by copying the colorExtension3 
					<b> color_extension</b>. The colors of this colorExtension3 are
					initialized to the colors of <b> color_extension</b>.
					Calls ColorExtension2.
					\param       color_extension the colorExtension3 to be copied
					\return      ColorExtension3 new constructed colorExtension3 copied from <b> color_extension</b>
					\see         ColorRGBA
					\see         ColorExtension2
			*/
			ColorExtension3(const ColorExtension3& color_extension)
				throw();

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of this colorExtension3.
			*/
			virtual ~ColorExtension3()
				throw();

			/** Explicit default initialization.
					Set the colors of this colorExtension3 to the color 
					(0.0, 0.0, 0.0, 1.0).
					Calls ColorExtension2::clear.
					Calls ColorRGBA::clear.		
					\see  ColorExtension2::clear
					\see  ColorRGBA::clear
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the colorExtension3 <b> color_extension</b> to this 
					colorExtension3.
					The colors of this colorExtension3 are initialized to the colors of 
					the colorExtension3 <b> color_extension</b>.
					Calls ColorExtension2::set.
					Calls ColorRGBA::set.
					\param       color_extension the colorExtension3 to be copied
					\see         ColorExtension2::set
					\see         ColorRGBA
					\see         ColorRGBA::set
			*/
			void set(const ColorExtension3& color_extension)
				throw();

			/** Assignment operator.
					Assign the colorExtension3 <b> color_extension</b> to this
					colorExtension3.
					Calls set.
					The colors of this colorExtension3 are initialized to the colors 
					of the colorExtension3 <b> color_extension</b>.\par
					\param       color_extension the colorExtension3 to be copied
					\return      ColorExtension3& constant reference of this colorExtension3
					\see         set
			*/
			const ColorExtension3& operator = (const ColorExtension3& color_extension)
				throw();

			/** Copying.
					Copy this colorExtension3 to the colorExtension3
					<b> color_extension</b>.
					Calls set.
					The colors of the colorExtension3 <b> color_extension</b> 
					are initialized to the colors of this colorExtension3.\par
					\param       color_extension the colorExtension3 to be assigned to
					\see         set
			*/
			void get(ColorExtension3& color_extension) const
				throw();

			/** Swapping of colorExtensions.
					Swap the colors of this colorExtension3 with the colorExtension3
					<b> color_extension</b>.
					Calls ColorExtension2::swap.
					Calls ColorRGBA::swap.
					\param       color_extension the colorExtension3 being swapped with this colorExtension 
					\see         ColorExtension3
					\see         ColorExtension2::swap
					\see         ColorRGBA::swap
			*/
			void swap(ColorExtension3& color_extension)
				throw();
			//@}

			
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the third color of this colorExtension3.
					Change the third color of this colorExtension3 to the color
					represented by the parameter <b> color</b>.
					\param       color the new third color of this colorExtension3
					\see         getColor3
					\see         ColorRGBA
			*/
			void setColor3(const ColorRGBA& color)
				throw();
			
			/** Change the third color of this colorExtension3.
					Change the third color of this colorExtension3 to the color 
					represented	by the parameters <b> red</b>, <b> green</b>, <b> blue</b> and
					<b> alpha</b>.
					\param       red the red component of the new third color of this colorExtension3
					\param       green the green component of the new third color of this colorExtension3
					\param       blue the blue component of the new third color of this colorExtension3
					\param       alpha the alpha component of the new third color of this colorExtension3 (Default=1.0)
					\see         getColor3
					\see         ColorUnit
			*/
			void setColor3
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1)
				throw();
			
			/** Mutable inspection of the third color of this colorExtension3.
					Access the mutual reference of the third color of this
					colorExtension3.
					\return      ColorRGBA& mutable reference to the third color of this colorExtension3
					\see         setColor3
					\see         ColorRGBA
			*/
			ColorRGBA& getColor3()
				throw();

			/** Non-mutable inspection of the third color of this colorExtension3.
					For further information see getColor3.
			*/
			const ColorRGBA& getColor3() const
				throw();

			/** Inspection of the third color of this colorExtension3.
					Access the third color of this colorExtension3 by using ColorRGBA.
					\param       color the color receiving the third color of this colorExtension3
					\see         setColor3
					\see         ColorRGBA
			*/
			void getColor3(ColorRGBA& color) const
				throw();

			/** Inspection of all the components of the third color of this colorExtension3.
					Access all the components of the third color of this colorExtension3
					by using ColorUnit.
					\param       red the red component of the third color of this colorExtension3
					\param       green the green component of the third color of this colorExtension3
					\param       blue the blue component of the third color of this colorExtension3
					\param       alpha the alpha component of the third color of this colorExtension3
					\see         setColor3
					\see         ColorUnit
			*/
			void getColor3
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const
				throw();

			/** Inspection of the components of the third color of this
					colorExtension3.
					Access only the red, green and blue components of the third color 
					of this colorExtension3	by using ColorUnit.
					\param       red the red component of the third color of this colorExtension3
					\param       green the green component of the third color of this colorExtension3
					\param       blue the blue component of the third color of this colorExtension3
					\see         setColor3
					\see         ColorUnit
			*/
			void getColor3(ColorUnit& red, ColorUnit& green, ColorUnit& blue) const
				throw();

			//@}

			/**	@name	debuggers and diagnostics
			*/	
			//@{

			/** Internal value dump.
					Dump the current state of this colorExtension3 to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this colorExtension3
					\param   depth the dumping depth
					\see     ColorRGBA::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
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
