// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorExtension2.h,v 1.5 2004/02/18 18:19:06 anhi Exp $

#ifndef BALL_VIEW_DATATYPE_COLOREXTENSION2_H
#define BALL_VIEW_DATATYPE_COLOREXTENSION2_H

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** Base class for all geometric 
				objects that needs two colors ColorRGBA. It provides the derived
				class with methods for accessing these colors.
				\see         ColorRGBA
				\see         ColorUnit
				\ingroup ViewDatatypeGeometricObject
		*/
		class ColorExtension2
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The colors are set to (0.0, 0.0, 0.0, 1.0).
			*/
			ColorExtension2()
				throw();

			/** Copy constructor.
			*/
			ColorExtension2(const ColorExtension2& color_extension)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~ColorExtension2()
				throw();

			/** Explicit default initialization.
					Set the colors of this colorExtension2 to the color 
					(0.0, 0.0, 0.0, 1.0).
					Calls ColorRGBA::clear.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
			*/
			void set(const ColorExtension2& color_extension)
				throw();

			/** Assignment operator.
					Calls set.
			*/
			const ColorExtension2& operator = (const ColorExtension2& color_extension)
				throw();

			/** Swapping of colorExtensions.
			*/
			void swap(ColorExtension2& color_extension)
				throw();
			
			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the second color
			*/
			void setColor2(const ColorRGBA& color)
				throw();
			
			/** Change the second color of this colorExtension2.
			*/
			void setColor2
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1)
				throw();
			
			/** Inspection of all the components of the second color
			*/
			void getColor2
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const
				throw();
			
			/** Inspection of the components of the second color
			*/
			void getColor2
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue) const
				throw();
			
			/** Mutable inspection of the second color of this colorExtension2.
			*/
			ColorRGBA& getColor2()
				throw();

			/** Non-mutable inspection of the second color
			*/
			const ColorRGBA& getColor2() const
				throw();

			/** Inspection of the second color of this colorExtension2.
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
#			include <BALL/VIEW/DATATYPE/colorExtension2.iC>
#		endif

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_COLOREXTENSION2_H
