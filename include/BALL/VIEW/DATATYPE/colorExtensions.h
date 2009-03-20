// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorExtensions.h,v 1.1.4.1 2007/03/25 21:25:40 oliver Exp $
//

#ifndef BALL_VIEW_DATATYPE_COLOREXTENSIONS_H
#define BALL_VIEW_DATATYPE_COLOREXTENSIONS_H

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** Base class for geometric objects that need two colors. 
		  	It provides the derived class with methods for accessing these colors.
				\see         ColorRGBA
				\see         ColorUnit
				\ingroup ViewDatatypeGeometricObject
		*/
		class BALL_VIEW_EXPORT ColorExtension2
		{
			public:

			BALL_CREATE(ColorExtension2)
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The colors are set to (0.0, 0.0, 0.0, 1.0).
			*/
			ColorExtension2();

			/** Copy constructor.
			*/
			ColorExtension2(const ColorExtension2& color_extension);

			/** Destructor.
			*/
			virtual ~ColorExtension2();

			/** Explicit default initialization.
					Set the colors of this colorExtension2 to the color 
					(0.0, 0.0, 0.0, 1.0).
					Calls ColorRGBA::clear.
			*/
			virtual void clear();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
			*/
			void set(const ColorExtension2& color_extension);

			/** Assignment operator.
					Calls set.
			*/
			const ColorExtension2& operator = (const ColorExtension2& color_extension);

			/** Swapping of colorExtensions.
			*/
			void swap(ColorExtension2& color_extension);
			
			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the second color
			*/
			void setColor2(const ColorRGBA& color);
		
			/** Mutable inspection of the second color of this colorExtension2.
			*/
			ColorRGBA& getColor2();

			/** Non-mutable inspection of the second color
			*/
			const ColorRGBA& getColor2() const;

			/** Inspection of the second color of this colorExtension2.
			*/
			void getColor2(ColorRGBA& color) const;

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
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			private:
		
			/* color 2 */
			ColorRGBA color2_;
		};

		/** Base class for geometric objects that need multiple colors. 
		  	It provides the derived class with methods for accessing these colors.
				\see         ColorRGBA
				\see         ColorUnit
				\ingroup ViewDatatypeGeometricObject
		*/
		class BALL_VIEW_EXPORT MultiColorExtension
		{
			public:

			BALL_CREATE(MultiColorExtension)
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
			*/
			MultiColorExtension();

			/** Copy constructor.
			*/
			MultiColorExtension(const MultiColorExtension& color_extension);

			/** Destructor.
			*/
			virtual ~MultiColorExtension();

			/** Explicit default initialization.
					Clears the color vector.
			*/
			virtual void clear();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
			*/
			void set(const MultiColorExtension& color_extension);

			/** Assignment operator.
					Calls set.
			*/
			const MultiColorExtension& operator = (const MultiColorExtension& color_extension);

			/** Swapping of colorExtensions.
			*/
			void swap(MultiColorExtension& color_extension);
			
			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the colors
			*/
			void setColors(const vector<ColorRGBA>& colors);
		
			/** Mutable inspection of the colors
			*/
			vector<ColorRGBA>& getColors();

			/** Non-mutable inspection of the colors
			*/
			const vector<ColorRGBA>& getColors() const;

			/// Set the alpha values for all colors
			void setAlphas(Size alpha);

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
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			vector<ColorRGBA> colors;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorExtensions.iC>
#		endif

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_COLOREXTENSIONS_H
