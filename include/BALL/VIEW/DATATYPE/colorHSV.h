// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorHSV.h,v 1.16 2004/02/23 15:20:02 anhi Exp $
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
#include <stdio.h>

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
		class ColorHSV
		{
			/** @name Class friends
						- class ColorRGBA
					\par
			*/
			friend class ColorRGBA;

			public:

			/**	@name	Constructors
			*/
			//@{

			/** Default Constructor.
					Constructs new colorHSV. Initialized with color black (hue=0, saturation=0, value=0).
					\return      ColorHSV new constructed colorHSV
			*/
			ColorHSV()
				throw();

			/** Copy constructor.
					Constructs new colorHSV by copying the colorHSV <b> color</b>.
					\param       color the colorHSV to be copied
					\return      ColorHSV new constructed colorHSV copied from <b> color</b>
			*/
			ColorHSV(const ColorHSV& color)
				throw();

			/** Constructor with ColorRGBA.
					Constructs new colorHSV by converting the colorRGBA <b> color</b>.
					\param       color the colorRGBA to be converted. See ColorRGBA
					\return      ColorHSV new constructed colorHSV converted from <b> color</b>
			*/
			ColorHSV(const ColorRGBA& color)
				throw();

			/** Constructor with Parameter char*.
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
			ColorHSV(const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Constructor with Parameter string.
					Constructs new colorHSV by initialising its value with  
					a string representing a color value in hex format. The length of the string can
					either be 6 or 7 characters depending whether the hue component is 2 or 3 bytes
					long. See ColorUnitHue and ColorUnit for information concerning
					to string format.
					\param       value a string representing a color
					\exception   InvalidRange if <tt> string length != 6 && string length != 7</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
					\return      ColorHSV new constructed colorHSV
					\see         ColorUnit
					\see         ColorUnitHue
			*/
			ColorHSV(const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Detailed component initializing constructor.
					Constructs new colorHSV using the given component parameters.
					\param   hue the hue component. See ColorUnitHue
					\param   saturation the staturation component. See ColorUnit
					\param   value the value component. See ColorUnit
					\return      ColorHSV new constructed colorHSV
					\see         ColorUnit
					\see         ColorUnitHue
			*/
			ColorHSV
				(const ColorUnitHue& hue,
				 const ColorUnit& saturation,
				 const ColorUnit& value)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~ColorHSV()
				throw();

			/** Explicit default initialization.
					Sets the color of this colorHSV to the default color (black).
					The value of this colorHSV is:
						- hue component is zero (0)
						- staturation component is zero (0)
						- value component is zero (0)
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Converters
			*/
			//@{

			/** Conversion to string.
					This operator casts an instance of colorHSV to the string type.
					\return      a string with hex format. See constructor for information concerning the string format
					\see         ColorHSV
			*/
			operator String () const
				throw();

			//@}
			/**	@name	Assignment	
			*/
			//@{

			/** Assignment.
			*/
			void set(const ColorHSV& color)
				throw();

			/** Assignment operator.
					Calls set.
			*/
			const ColorHSV& operator = (const ColorHSV& color)
				throw();

			/** Assignment with ColorRGBA.
					Assigns the colorRGBA <b> color</b> to this colorHSV.
					The color of this is initialized to the color of the colorRGBA <b> color</b>.\par
					\param       color the colorRGBA to be copied 
			*/
			void set(const ColorRGBA& color)
				throw();

			/** Assignment operator with ColorRGBA.
					Assigns the colorRGBA <b> color</b> to this colorHSV.
					Calls set.
					The color of this is initialized to the color of the colorRGBA <b> color</b>.\par
					\param       color the colorRGBA to be copied
					\return      ColorHSV& constant reference of this colorHSV
					\see         set
			*/
			const ColorHSV& operator = (const ColorRGBA& color)
				throw();

			/** Copying to ColorRGBA.
					Copies this colorHSV to the colorRGBA <b> color</b>.
					Calls set.
					The color of the colorRGBA <b> color</b> is initialized to the color of this colorHSV.\par
					\param       color the colorRGBA to be assigned to
					\see         set
			*/
			void get(ColorRGBA& color) const
				throw();

			/** Assignment with char*.
					Assigns the color represented by the string <b> value</b> to this colorHSV.
					The color of this is initialized to the color represented by the string <b> value</b>.
					See constructor for format description.
					\param       value the color represented by a string 
					\exception   InvalidRange if <tt> string length != 6 && string length != 7</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
					\see         ColorHSV
			*/
			void set(const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Assignment operator with char*.
					Assigns the color represented by the string <b> value</b> to this colorHSV.
					Calls set.
					The color of this is initialized to the color represented by the string <b> value</b>.
					See constructor for format description.
					\param       value the color represented by a string
					\exception   InvalidRange if <tt> string length != 6 && string length != 7</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
					\return      ColorHSV& constant reference of this colorHSV
					\see         set
					\see         ColorHSV
			*/
			const ColorHSV& operator = (const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Copying to char*.
					Copies the color of this colorHSV to the string represented by the pointer <b> value</b>.
					Calls set.
					The string <b> value</b> is initialized to the color of this colorHSV.
					At least 8 bytes must be allocated for the string. The format of the string is
					the same as the one given to the constructor.
					\param       value the pointer to string (at least 8 bytes long)
					\see         set
					\see         ColorHSV
			*/
			void get(char* value) const
				throw();

			/** Assignment with string.
					Assigns the color represented by the string <b> value</b> to this colorHSV.
					The color of this is initialized to the color represented by the string <b> value</b>.
					See constructor for format description.
					\param       value the color represented by a string 
					\exception   InvalidRange if <tt> string length != 6 && string length != 7</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
					\see         ColorHSV
			*/
			void set(const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Assignment operator with string.
					Assigns the color represented by the string <b> value</b> to this colorHSV.
					Calls set.
					The color of this is initialized to the color represented by the string <b> value</b>.
					See constructor for format description.
					\param       value the color represented by a string
					\exception   InvalidRange if <tt> string length != 6 && string length != 7</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
					\return      ColorHSV& constant reference of this colorHSV
					\see         set
					\see         ColorHSV
			*/
			const ColorHSV& operator = (const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Copying to string.
					Copies the color of this colorHSV to the string represented by the pointer <b> value</b>.
					Calls set.
					The string <b> value</b> is initialized to the color of this colorHSV.
					The format of the string is	the same as the one given to the constructor.
					\param       value the string receiving the color in hex format
					\see         set
					\see         ColorHSV
			*/
			void get(String& value) const
				throw();

			/** Swapping of colorHSVs.
					Swaps the color of this colorHSV with the colorHSV <b> color</b>.
					\param       color the colorHSV being swapped with this colorHSV
					\see         ColorHSV
			*/
			void swap(ColorHSV& color)
				throw();

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
			void setHue(const ColorUnitHue& hue)
				throw();

			/** Mutable inspection of the hue component.
					Accesses the mutual reference of the hue component of this colorHSV.
					See ColorUnitHue for further information concerning type conversion.
					\return      ColorUnitHue& mutable reference to the hue component of this colorHSV
					\see         setHue
					\see         ColorUnitHue
			*/
			ColorUnitHue& getHue()
				throw();

			/** Non-mutable inspection of the hue component.
					For further information see <b> ColorUnitHue& getHue()</b>.
			*/
			const ColorUnitHue& getHue() const
				throw();

			/** Changes the value of the saturation component.
					Changes the value of of the saturation component of this colorHSV to the value
					represented by the parameter <b> saturation</b>. See ColorUnit for further
					information concerning the format.
					\param       saturation the new saturation component of this colorHSV
					\see         getSaturation
					\see         ColorUnit
			*/
			void setSaturation(const ColorUnit& saturation)
				throw();

			/** Mutable inspection of the saturation component.
					Accesses the mutual reference of the saturation component of this colorHSV.
					See ColorUnit for further information concerning type conversion.
					\return      ColorUnit& mutable reference to the saturation component of this colorHSV
					\see         setSaturation
					\see         ColorUnit
			*/
			ColorUnit& getSaturation()
				throw();

			/** Non-mutable inspection of the saturation component.
			                For further information see <b> ColorUnit& getSaturation()</b>.
			*/
			const ColorUnit& getSaturation() const
				throw();

			/** Changes the value of the value component.
					Changes the value of of the value component of this colorHSV to the value
					represented by the parameter <b> value</b>. See ColorUnit for further
					information concerning the format.
					\param       value the new value component of this colorHSV
					\see         getValue
					\see         ColorUnit
			*/
			void setValue(const ColorUnit& value)
				throw();

			/** Mutable inspection of the value component.
					Accesses the mutual reference of the value component of this colorHSV.
					See ColorUnit for further information concerning type conversion.
					\return      ColorUnit& mutable reference to the value component of this colorHSV
					\see         setValue
					\see         ColorUnit
			*/
			ColorUnit& getValue()
				throw();

			/** Non-mutable inspection of the value component.
			                For further information see <b> ColorUnit& getValue()</b>.
			*/
			const ColorUnit& getValue() const
				throw();

			/** Changes the values of the hue, saturation and value component.
					Changes the values of of the hue, saturation and value component of this
					colorHSV to the values represented by the parameters. See ColorUnit
					and ColorUnitHue for further information concerning type conversions.
					\param       hue the new hue component of this colorHSV
					\param       saturation the new saturation component of this colorHSV
					\param       value the new value component of this colorHSV
					\see         get
					\see         ColorUnit
					\see         ColorUnitHue
			*/
			void set
				(const ColorUnitHue& hue,
				 const ColorUnit& saturation,
				 const ColorUnit& value)
				throw();

			/** Inspection of the colorHSV's components.
					Accesses all the components of this colorHSV by using ColorUnit
					and ColorUnitHue.
					\param      hue the colorUnitHue receiving the hue component of this colorHSV
					\param      saturation the colorUnit receiving the saturation component of this colorHSV
					\param      value the colorUnit receiving the value component of this colorHSV
					\see        set
					\see        ColorUnit
					\see        ColorUnitHue
			*/
			void get
				(ColorUnitHue& hue,
				 ColorUnit& saturation,
				 ColorUnit& value) const
				throw();

			//@}
			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
					Tests if <b> color</b> is equal to this colorHSV.
					Equality means that all components are equal. See ColorUnit and
					ColorUnitHue for further information concerning equality tests.
					\param   color the colorHSV to be compared with
					\return  bool -	<tt> true</tt> if the color of this is 
									 equal to <b> color</b>, <tt> false</tt> otherwise
					\see     ColorUnit
					\see     ColorUnitHue

			*/
			bool operator == (const ColorHSV& color) const
				throw();

			/** Unequality test.
					Tests if <b> color</b> is not equal to this colorHSV.
					Unequality means that all components are unequal. See ColorUnit and
					ColorUnitHue for further information concerning unequality tests.
					\param   color the colorHSV to be compared with
					\return  bool -	<tt> true</tt> if the color of this is not 
									 equal to <b> color</b>, <tt> false</tt> otherwise
					\see     ColorUnit
					\see     ColorUnitHue
			*/
			bool operator != (const ColorHSV& color) const
				throw();

			/** Smaller test.
					Tests if this is smaller than <b> color</b> colorHSV.
					Smaller means that all components of this are smaller than all the
					components of <b> color</b> colorHSV. See ColorUnit and
					ColorUnitHue for further information concerning unequality tests.
					\param   color the colorHSV to be compared with
					\return  bool -	<tt> true</tt> if the color of this is 
									 smaller than <b> color</b>, <tt> false</tt> otherwise
					\see     ColorUnit
					\see     ColorUnitHue
			*/
			bool operator < (const ColorHSV& color) const
				throw();

			/** Smaller or equal test.
					Tests if this is smaller or equal than <b> color</b> colorHSV.
					Smaller or equal means that all components of this are smaller or
					equal than all the components of <b> color</b> colorHSV. See ColorUnit and
					ColorUnitHue for further information concerning unequality tests.
					\param   color the colorHSV to be compared with
					\return  bool -	<tt> true</tt> if the color of this is smaller or 
									 equal than <b> color</b>, <tt> false</tt> otherwise
					\see     ColorUnit
					\see     ColorUnitHue
			*/
			bool operator <= (const ColorHSV& color) const
				throw();

			/** Greater test.
					Tests if this is greater than <b> color</b> colorHSV.
					Greater means that all components of this are greater than all the
					components of <b> color</b> colorHSV. See ColorUnit and
					ColorUnitHue for further information concerning greater tests.
					\param   color the colorHSV to be compared with
					\return  bool -	<tt> true</tt> if the color of this is 
									 greater than <b> color</b>, <tt> false</tt> otherwise
					\see     ColorUnit
					\see     ColorUnitHue
			*/
			bool operator > (const ColorHSV& color) const
				throw();

			/** Greater or equal test.
					Tests if this is greater or equal than <b> color</b> colorHSV.
					Greater or equal means that all components of this are greater or
					equal than all the components of <b> color</b> colorHSV. See ColorUnit and
					ColorUnitHue for further information concerning greater or equal tests.
					\param   color the colorHSV to be compared with
					\return  bool -	<tt> true</tt> if the color of this is greater 
									 or equal than <b> color</b>, <tt> false</tt> otherwise
					\see     ColorUnit
					\see     ColorUnitHue
			*/
			bool operator >= (const ColorHSV& color) const
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{
	
			/** Internal value dump.
					Dumps the current color of this colorHSV to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the color of this colorHSV
					\param   depth the dumping depth
					\see     ColorUnit::dump
					\see     ColorUnitHue::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

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

			void calculateHSV_
				(const ColorUnit red,
				 const ColorUnit green,
				 const ColorUnit blue)
				throw();

			void calculateRGB_
				(ColorUnit& red,
				 ColorUnit& green,
				 ColorUnit& blue) const
				throw();

			void stringToHSV_
				(const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			ColorUnitHue hue_;
			ColorUnit saturation_;
			ColorUnit value_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorHSV.iC>
#		endif
  		
	} // namespace VIEW

} // namespace BALL

#endif // <BALL/VIEW_DATATYPE_COLORHSV_H
