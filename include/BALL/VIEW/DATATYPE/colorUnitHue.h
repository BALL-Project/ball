// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorUnitHue.h,v 1.20 2003/12/15 02:50:55 amoll Exp $
//

#ifndef BALL_VIEW_DATATYPE_COLORUNITHUE_H
#define BALL_VIEW_DATATYPE_COLORUNITHUE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORUNIT_H
#	include <BALL/VIEW/DATATYPE/colorUnit.h>
#endif

#include <iostream>
#include <stdio.h>

namespace BALL 
{
	namespace VIEW
	{
		/** ColorUnitHue class.
				An instance of ColorUnitHue represents a value for the hue
				component of the class ColorHSV.
				The class handles the many possible variations a hue value can be set
				(e.g. integer, float, string values can be given as parameter). The integer values
				are of range <tt> 0-360</tt>, the float values are of range 0-1.0 and the strings
				are given in hex format <tt> "000"-"168"</tt> for the values <tt> 0-360</tt>.
				ColorUnitHue provides certain conversion functions as well that will convert
				its value to the needed type.
				This class is used by the class ColorHSV. \par
				\ingroup ViewDatatypeFormat
		*/
		class ColorUnitHue
		{
			public:
			
			/**	@name	Constructors
			*/
			//@{

			/** Default Constructor.
			*/
			ColorUnitHue()
				throw();
		
			/** Copy constructor.
			*/
			ColorUnitHue(const ColorUnitHue& colcor_h_unit)
				throw();

			/** Constructor with Parameter char*.
					\param       value the pointer to a string (<tt> 00 - 168h</tt>)
					\exception   InvalidRange if <tt> string length > 3</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
			*/
			ColorUnitHue(const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Constructor with Parameter String.
					\exception   InvalidRange if <tt> string length > 3</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
			*/
			ColorUnitHue(const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Constructor with Parameter short.
					\param       value a short value (<tt> 0 - 360</tt>)
					\exception   InvalidRange if <tt> value < 0 || value > 360</tt>
			*/
			ColorUnitHue(const short value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter unsigned short.
					\param       value a unsigned short value (<tt> 0 - 360</tt>)
					\exception   InvalidRange if <tt> value > 360</tt>
			*/
			ColorUnitHue(const unsigned short value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter int.
					\param       value an int value (<tt> 0 - 360</tt>)
					\exception   InvalidRange if <tt> value < 0 || value > 360</tt>
			*/
			ColorUnitHue(const int value)
				throw(Exception::InvalidRange);
		
			/** Constructor with Parameter unsigned int.
					\param       value an unsigned int value (<tt> 0 - 360</tt>)
					\exception   InvalidRange if <tt> value > 360</tt>
			*/
			ColorUnitHue(const unsigned int value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter long.
					\param       value an long value (<tt> 0 - 360</tt>)
					\exception   InvalidRange if <tt> value < 0 || value > 360</tt>
			*/
			ColorUnitHue(const long value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter unsigned long.
					\param       value an unsigned long value (<tt> 0 - 360</tt>)
					\exception   InvalidRange if <tt> value > 360</tt>
			*/
			ColorUnitHue(const unsigned long value)
				throw(Exception::InvalidRange);
		
			/** Constructor with Parameter float.
					\param       value a float value (<tt> 0 - 1.0</tt>)
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			ColorUnitHue(const float value)
				throw(Exception::InvalidRange);
		
			/** Constructor with Parameter double.
					\param       value a double value (<tt> 0 - 1.0</tt>)
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			ColorUnitHue(const double value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter long double.
					\param       value a long double value (<tt> 0 - 1.0</tt>)
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			ColorUnitHue(const long double value)
				throw(Exception::InvalidRange);

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
			*/
			virtual ~ColorUnitHue()
				throw();

			/** Explicit default initialization.
					Sets the value of this colorUnitHue to the default value.
					The value of this colorUnitHue is:
						- value is zero (0)
					\par
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Converters
			*/
			//@{

			/** Conversion to string.
					\return      string with hex format <tt> "00"-"168"</tt>
			*/
			operator String () const
				throw();

			/** Conversion to short.
					\return      short with a value of range <tt> 0 - 360</tt>
			*/
			operator short () const
				throw();
			
			/** Conversion to unsigned short.
					\return      unsigned short with a value of range <tt> 0 - 360</tt>
			*/
			operator unsigned short () const
				throw();
			
			/** Conversion to int.
					\return      int with a value of range <tt> 0 - 360</tt>
			*/
			operator int () const
				throw();

			/** Conversion to unsigned int.
					\return      unsigned int with a value of range <tt> 0 - 360</tt>
			*/
			operator unsigned int () const
				throw();

			/** Conversion to long.
					\return      long with a value of range <tt> 0 - 360</tt>
			*/
			operator long () const
				throw();

			/** Conversion to unsigned long.
					\return      unsigned long with a value of range <tt> 0 - 360</tt>
			*/
			operator unsigned long () const
				throw();

			/** Conversion to float.
					\return      float with a value of range <tt> 0 - 1.0</tt>
			*/
			operator float () const
				throw();

			/** Conversion to double.
					\return      double with a value of range <tt> 0 - 1.0</tt>
			*/
			operator double () const
				throw();
			
			/** Conversion to long double.
					\return      long double with a value of range <tt> 0 - 1.0</tt>
			*/
			operator long double () const
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{
			
			/** Assignment.
			*/
			void set(const ColorUnitHue& color_unit_hue)
				throw();

			/** Assignment operator.
			*/
			const ColorUnitHue& operator = (const ColorUnitHue& color_unit_hue)
				throw();
		
			/** Swapping of colorUnitHues.
			*/
			void swap(ColorUnitHue& color_unit_hue)
				throw();

			/** Changes the value of this colorUnitHue to the value represented by 
					a pointer <b> value</b> to a string.
					\param       value the new value of this colorUnitHue (a string in the format <tt> "00"-"168"</tt>)
					\exception   InvalidRange if <tt> string length > 3</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
			*/
			void set(const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Assignment operator with char*.
					Calls set.
					\param       value the pointer to a string containing a value in hex format (<tt> "00"-"168"</tt>)
					\exception   InvalidRange if <tt> string length > 3</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
			*/
			const ColorUnitHue& operator = (const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);
		
			/** Inspection of the colorUnitHue's value.
					Accesses the value of this colorUnitHue by using a char pointer.
					Memory must be allocated for this parameter. The memory must be at least
					the size of 4 bytes.
					\param   value a char* receiving the value of this colorUnitHue in hex format (<tt> "00"-"168"</tt>)
			*/
			void get(char* value) const
				throw();

			/** Changes the value of the colorUnitHue.
					\param       value a string (in the hex format <tt> "00"-"168"</tt>)
					\exception   InvalidRange if <tt> string length > 3</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
			*/
			void set(const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Assignment operator with string.
					Calls set.
					\param       value a string containing a value in hex format (<tt> "00"-"168"</tt>)
					\exception   InvalidRange if <tt> string length > 3</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
			*/
			const ColorUnitHue& operator = (const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);
		
			/** Inspection of the colorUnitHue's value.
					\param   value a string receiving the value of this colorUnitHue in hex format (<tt> "00"-"168"</tt>)
					\see     set
			*/
			void get(String& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					\param       value a short of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 360</tt>
			*/
			void set(const short value)
				throw(Exception::InvalidRange);

			/** Assignment operator with short.
					Calls set.
					\param       value a short containing a value of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 360</tt>
			*/
			const ColorUnitHue& operator = (const short value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					\param   value a short receiving the value of this colorUnitHue (range: <tt> 0-360</tt>)
			*/
			void get(short& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					\param       value an unsigned short of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value > 360</tt>
			*/
			void set(const unsigned short value)
				throw(Exception::InvalidRange);

			/** Assignment operator with unsigned short.
					Calls set.
					\param       value an unsigned short containing a value of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value > 360</tt>
			*/
			const ColorUnitHue& operator = (const unsigned short value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					\param   value an unsigned short receiving the value of this colorUnitHue (range: <tt> 0-360</tt>)
			*/
			void get(unsigned short& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					\param       value an int of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 360</tt>
			*/
			void set(const int value)
				throw(Exception::InvalidRange);

			/** Assignment operator with int.
					Calls set.
					\param       value an int containing a value of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 360</tt>
			*/
			const ColorUnitHue& operator = (const int value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					\param   value an int receiving the value of this colorUnitHue (range: <tt> 0-360</tt>)
			*/
			void get(int& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					\param       value an unsigned int of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value > 360</tt>
			*/
			void set(const unsigned int value)
				throw(Exception::InvalidRange);

			/** Assignment operator with unsigned int.
					Calls set.
					\param       value an unsigned int containing a value of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value > 360</tt>
			*/
			const ColorUnitHue& operator = (const unsigned int value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					\param   value an unsigned int receiving the value of this colorUnitHue (range: <tt> 0-360</tt>)
			*/
			void get(unsigned int& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					\param       value a long of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 360</tt>
			*/
			void set(const long value)
				throw(Exception::InvalidRange);

			/** Assignment operator with long.
					Calls set.
					\param       value a long containing a value of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 360</tt>
			*/
			const ColorUnitHue& operator = (const long value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					\param   value a long receiving the value of this colorUnitHue (range: <tt> 0-360</tt>)
			*/
			void get(long& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					\param       value an unsigned long of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value > 360</tt>
			*/
			void set(const unsigned long value)
				throw(Exception::InvalidRange);

			/** Assignment operator with unsigned long.
					Calls set.
					\param       value an unsigned long containing a value of range <tt> 0-360</tt>
					\exception   InvalidRange if <tt> value > 360</tt>
			*/
			const ColorUnitHue& operator = (const unsigned long value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					\param   value an unsigned long receiving the value of this colorUnitHue (range: <tt> 0-360</tt>)
			*/
			void get(unsigned long& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					\param       value a float of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			void set(const float value)
				throw(Exception::InvalidRange);

			/** Assignment operator with float.
					Calls set.
					\param       value a float containing a value of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			const ColorUnitHue& operator = (const float init)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					\param   value a float receiving the value of this colorUnitHue (range: 0-1.0)
					\see     set
			*/
			void get(float& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					\param       value a double of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			void set(const double value)
				throw(Exception::InvalidRange);

			/** Assignment operator with double.
					Calls set.
					\param       value a double containing a value of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			const ColorUnitHue& operator = (const double value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					\param   value a double receiving the value of this colorUnitHue (range: 0-1.0)
			*/
			void get(double& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					\param       value a long double of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			void set(const long double value)
				throw(Exception::InvalidRange);

			/** Assignment operator with long double.
					Calls set.
					\param       value a long double containing a value of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			const ColorUnitHue& operator = (const long double value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					\param   value a long double receiving the value of this colorUnitHue (range: 0-1.0)
			*/
			void get(long double& value) const
				throw();

			//@}
			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
			*/
			bool operator == (const ColorUnitHue& color_unit_hue) const
				throw();

			/** Unequality test.
			*/
			bool operator != (const ColorUnitHue& color_unit_hue) const
				throw();
			
			/** Smaller test.
			*/
			bool operator <	(const ColorUnitHue& color_unit_hue) const
				throw();
			
			/** Smaller or equal test.
			*/
			bool operator <= (const ColorUnitHue& color_unit_hue) const
				throw();
			
			/** Greater test.
			*/
			bool operator > (const ColorUnitHue& color_unit_hue) const
				throw();

			/** Greater or equal test.
			*/
			bool operator >= (const ColorUnitHue& color_unit_hue) const
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dumps the current value of this colorUnitHue to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the value of this colorUnitHue
					\param   depth the dumping depth (depth is irrelevant here)
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}
			/**	@name	Storers
			*/	
			//@{

			/** Friend stream input.
					Reads colorUnitHue data from the input stream <b> s</b> and restore the value of this.
					The value will be needed in integer format of range <tt> 0-360</tt>.
					This method is provided for convienience.
					\param  s input stream from where to restore the value of this colorUnitHue
					\param  color_unit_hue the colorUnitHue to which the value will be restored
			*/
			friend std::istream& operator >> (std::istream& s, ColorUnitHue& color_unit_hue);

			/** Friend stream output.
					Writes colorUnitHue data to the output stream <b> s</b> and store the value of this.
					The value will be stored in integer format of range <tt> 0-360</tt>.
					This method is provided for convienience.
					\param  s output stream to where the value of this colorUnitHue will be stored
					\param  color_unit_hue the colorUnitHue from which the value will be stored
			*/
			friend std::ostream& operator << (std::ostream& s, const ColorUnitHue& color_unit_hue);
			//@}

			private:
			
			float hexToFloat_(const char* string)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);
			
			float value_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorUnitHue.iC>
#		endif
  
	} // namspace VIEW
} // namespace BALL

#endif // BALL_VIEW_DATATYPE_COLORUNITHUE_H
