// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DATATYPE_COLORHSV_H
#define BALL_VIEW_DATATYPE_COLORHSV_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORUNIT_H
#	include <BALL/VIEW/DATATYPE/colorUnit.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORUNITHUE_H
#	include <BALL/VIEW/DATATYPE/colorUnitHue.h>
#endif

#include <iostream>
#include <cstdio>

namespace BALL 
{
	namespace VIEW 
	{

		class ColorRGBA;

		/** ColorHSV class.
				An instance of ColorHSV represents a color consisting of the components
				hue, saturation and value. The components staturation and values are 
				 \link ColorUnit ColorUnit \endlink 's. The component hue is of class type  \link ColorUnitHue ColorUnitHue \endlink .
				See description of these classes for further informations concerning type
				conversions.  \par
			\ingroup ViewDatatypeColors	
		*/
		class BALL_VIEW_EXPORT ColorHSV
		{
			/** @name Class friends
						- class ColorRGBA
					\par
			*/
			friend class ColorRGBA;

			public:

			BALL_CREATE(ColorHSV)

			/**	@name	Constructors
			*/
			//@{

			/** Default Constructor.
					Constructs new colorHSV. Initialized with color black (hue=0, saturation=0, value=0).
					\return      ColorHSV new constructed colorHSV
			*/
			ColorHSV();

			/** Copy constructor.
			*/
			ColorHSV(const ColorHSV& color);

			/** Constructor with ColorRGBA.
			*/
			ColorHSV(const ColorRGBA& color);

			/** Constructor with parameter char*.
					Constructs new colorHSV by initialising its value with the given pointer to
					a string representing a color value in hex format. The length of the string can
					either be 6 or 7 characters depending whether the hue component is 2 or 3 bytes
					long. See ColorUnitHue and ColorUnit for information concerning
					to string format.
					\param       value the pointer to a string representing a color
					\exception   InvalidRange if <tt> string length != 6 && string length != 7</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
					\return      ColorHSV new constructed colorHSV
					\see         ColorUnit
					\see         ColorUnitHue
			*/
			ColorHSV(const char* value);

			/** Constructor with Parameter string.
					Constructs new ColorHSV by initialising its value with  
					a string representing a color value in hex format. The length of the string can
					either be 6 or 7 characters depending whether the hue component is 2 or 3 bytes
					long. See ColorUnitHue and ColorUnit for information concerning
					to string format.
					\param       value a string representing a color
					\exception   InvalidRange if <tt> string length != 6 && string length != 7</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
			*/
			ColorHSV(const String& value);

			/** Detailed component initializing constructor.
					\param   hue the hue component. See ColorUnitHue
					\param   saturation the staturation component. See ColorUnit
					\param   value the value component. See ColorUnit
					\return      ColorHSV new constructed colorHSV
			*/
			ColorHSV
				(const ColorUnitHue& hue,
				 const ColorUnit& saturation,
				 const ColorUnit& value);

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~ColorHSV();

			/** Explicit default initialization.
					Sets the color of this colorHSV to the default color (black).
					The value of this colorHSV is:
						- hue component is zero (0)
						- staturation component is zero (0)
						- value component is zero (0)
			*/
			virtual void clear();

			//@}
			/**	@name	Converters
			*/
			//@{

			/** Conversion to string.
					\return      a string with hex format. See constructor for information concerning the string format
			*/
			operator String () const;

			//@}
			/**	@name	Assignment	
			*/
			//@{

			/** Assignment.
			*/
			void set(const ColorHSV& color);

			/** Assignment operator.
					Calls set.
			*/
			const ColorHSV& operator = (const ColorHSV& color);

			/** Assignment with ColorRGBA.
					The color of this is initialized to the color of the colorRGBA <b> color</b>.\par
			*/
			void set(const ColorRGBA& color);

			/** Assignment operator with ColorRGBA.
					Assigns the colorRGBA <b> color</b> to this colorHSV.
					Calls set.
					The color of this is initialized to the color of the colorRGBA <b> color</b>.\par
					\param       color the colorRGBA to be copied
					\return      ColorHSV& constant reference of this colorHSV
					\see         set
			*/
			const ColorHSV& operator = (const ColorRGBA& color);

			/** Copying to ColorRGBA.
					Calls set.
					The color of the colorRGBA <b> color</b> is initialized to the color of this colorHSV.\par
					\param       color the colorRGBA to be assigned to
			*/
			void get(ColorRGBA& color) const;

			/** Assignment with char*.
					The color of this is initialized to the color represented by the string <b> value</b>.
					See constructor for format description.
					\param       value the color represented by a string 
					\exception   InvalidRange if <tt> string length != 6 && string length != 7</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
			*/
			void set(const char* value);

			/** Assignment operator with char*.
					Calls set.
					The color of this is initialized to the color represented by the string <b> value</b>.
					See constructor for format description.
					\param       value the color represented by a string
					\exception   InvalidRange if <tt> string length != 6 && string length != 7</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
			*/
			const ColorHSV& operator = (const char* value);

			/** Copying to char*.
					Calls set.
					The string <b> value</b> is initialized to the color of this colorHSV.
					At least 8 bytes must be allocated for the string. The format of the string is
					the same as the one given to the constructor.
					\param       value the pointer to string (at least 8 bytes long)
					\see         set
			*/
			void get(char* value) const;

			/** Assignment with string.
					Assigns the color represented by the string <b> value</b> to this colorHSV.
					The color of this is initialized to the color represented by the string <b> value</b>.
					See constructor for format description.
					\param       value the color represented by a string 
					\exception   InvalidRange if <tt> string length != 6 && string length != 7</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
			*/
			void set(const String& value);

			/** Assignment operator with string.
					Assigns the color represented by the string <b> value</b> to this colorHSV.
					Calls set.
					See constructor for format description.
					\param       value the color represented by a string
					\exception   InvalidRange if <tt> string length != 6 && string length != 7</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
			*/
			const ColorHSV& operator = (const String& value);

			/** Copying to string.
					Calls set.
					The string <b> value</b> is initialized to the color of this colorHSV.
					The format of the string is	the same as the one given to the constructor.
					\param       value the string receiving the color in hex format
			*/
			void get(String& value) const;

			/** Swapping of colorHSVs.
			*/
			void swap(ColorHSV& color);

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Changes the value of the hue component.
					Changes the value of of the hue component of this colorHSV to the value
					represented by the parameter <b> hue</b>. See ColorUnitHue for further
					information concerning the format.
					\param       hue the new hue component of this colorHSV
					\see         getHue
					\see         ColorUnitHue
			*/
			void setHue(const ColorUnitHue& hue);

			/** Mutable inspection of the hue component.
					Accesses the mutual reference of the hue component of this colorHSV.
					See ColorUnitHue for further information concerning type conversion.
					\return      ColorUnitHue& mutable reference to the hue component of this colorHSV
					\see         setHue
					\see         ColorUnitHue
			*/
			ColorUnitHue& getHue();

			/** Non-mutable inspection of the hue component.
					For further information see <b> ColorUnitHue& getHue()</b>.
			*/
			const ColorUnitHue& getHue() const;

			/** Changes the value of the saturation component.
					\see         getSaturation
			*/
			void setSaturation(const ColorUnit& saturation);

			/** Mutable inspection of the saturation component.
					\see         setSaturation
			*/
			ColorUnit& getSaturation();

			/** Non-mutable inspection of the saturation component.
			*/
			const ColorUnit& getSaturation() const;

			/** Changes the value of the value component.
			*/
			void setValue(const ColorUnit& value);

			/** Mutable inspection of the value component.
			*/
			ColorUnit& getValue();

			/** Non-mutable inspection of the value component.
			                For further information see <b> ColorUnit& getValue()</b>.
			*/
			const ColorUnit& getValue() const;

			/** Changes the values of the hue, saturation and value component.
			*/
			void set (const ColorUnitHue& hue, const ColorUnit& saturation, const ColorUnit& value);

			/** Inspection of the colorHSV's components.
			*/
			void get (ColorUnitHue& hue, ColorUnit& saturation, ColorUnit& value) const;

			//@}
			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
			*/
			bool operator == (const ColorHSV& color) const;

			/** Unequality test.
			*/
			bool operator != (const ColorHSV& color) const;

			/** Smaller test.
					Smaller means that all components of this are smaller than all the
					components of <b> color</b> colorHSV.
			*/
			bool operator < (const ColorHSV& color) const;

			/** Smaller or equal test.
					Smaller or equal means that all components of this are smaller or
					equal than all the components of <b> color</b> colorHSV.
			*/
			bool operator <= (const ColorHSV& color) const;

			/** Greater test.
					Greater means that all components of this are greater than all the
					components of <b> color</b> colorHSV.
			*/
			bool operator > (const ColorHSV& color) const;

			/** Greater or equal test.
					Greater or equal means that all components of this are greater or
					equal than all the components of <b> color</b> colorHSV.
			*/
			bool operator >= (const ColorHSV& color) const;

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{
	
			/** Internal value dump.
					Dumps the current color of this colorHSV to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the color of this colorHSV
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			//@}
			/**	@name	Storers
			*/
			//@{

			/** Friendly stream input.
					Reads colorHSV data from the input stream <b> s</b> and restore the color of this.
					The hue, saturation and value component are read as integers from the input stream.
					The method expects an vector as written as the one by the output stream operator.
					This method is provided for convienience.
					\param  s input stream from where to restore the color of this colorHSV
					\param  color the colorHSV to which the color will be restored
			*/
			friend std::istream& operator >> (std::istream& s, ColorHSV& color);

			/** Friendly stream output.
					Writes colorHSV data to the output stream <b> s</b> and store the color of this.
					The color will be stored as an vector in integer format. The output will look
					like '(0-360, 0-255, 0-255)'.
					This method is provided for convienience.
					\param  s output stream to where the color of this colorHSV will be stored
					\param  color the colorHSV from which the color will be stored
			*/
			friend std::ostream& operator << (std::ostream& s, const ColorHSV& color);

			//@}

			private:

			void calculateHSV_ (const ColorUnit red, const ColorUnit green, const ColorUnit blue);

			void calculateRGB_ (ColorUnit& red, ColorUnit& green, ColorUnit& blue) const;

			// @throw Exception::InvalidRange 
			// @throw Exception::NotInHexFormat
			void stringToHSV_ (const String& value);

			ColorUnitHue 	hue_;
			ColorUnit 		saturation_;
			ColorUnit 		value_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorHSV.iC>
#		endif
  		
	} // namespace VIEW
} // namespace BALL

#endif // <BALL/VIEW_DATATYPE_COLORHSV_H
