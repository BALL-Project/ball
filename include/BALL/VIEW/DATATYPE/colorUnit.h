// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DATATYPE_COLORUNIT_H
#define BALL_VIEW_DATATYPE_COLORUNIT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#include <iostream>
#include <cstdio>

namespace BALL
{
	namespace VIEW
	{
		/** ColorUnit class.
				An instance of ColorUnit can represent a value for the red, green, blue
				and alpha component of the class ColorRGBA. Furthermore this class is
				used for the value and saturation component of the class ColorHSV.
				The class handles the many possible variations a color value can be set
				(e.g. integer, float, string values can be given as parameter). The integer
				values are of range 0-255, the float values are of range 0-1.0 and the strings
				given are in hex format "00"-"FF".
				ColorUnit provides certain conversion functions as well, that will convert
				its value to the needed type.
				This class is used by the class ColorRGBA and ColorHSV. \par
				\ingroup ViewDatatypeFormat
		*/
		class BALL_VIEW_EXPORT ColorUnit
		{
			public:

			//BALL_CREATE(ColorUnit)
			
			/**	@name	Exceptions
			*/
			//@{
			
			/** NotInHexFormat Exception class.
					This exeption will be thrown if a given string for the colorUnit class
					is not in the format "00" - "FF" or "00" - "ff".

					\see         Exception::GeneralException			
			*/
			class BALL_VIEW_EXPORT NotInHexFormat:	public Exception::GeneralException
			{
				public:

				NotInHexFormat(const char* file, int line, const string& data);
			};

			//@}
			/**	@name Constructors
			*/
			//@{

			/** Default Constructor.
					Its value will be set to zero (0).
			*/
			ColorUnit();
		
			/** Copy constructor.
			*/
			ColorUnit(const ColorUnit& color_unit);

			/** Constructor with Parameter char*,
					representing a value in hex format (00 - FF, or 00 - ff).
					\param       value the pointer to a string (00 - FF, or 00 - ff)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
			*/
			ColorUnit(const char* value);

			/** Constructor with Parameter String.
					\param       value a string (00 - FF, or 00 - ff)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
			*/
			ColorUnit(const String& value);

			/** Constructor with Parameter unsigned char.
					\param       value a char (0 - 255)
			*/
			ColorUnit(const unsigned char value);

			/** Constructor with Parameter short.
					\param       value a short (0 - 255)
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
			*/
			ColorUnit(const short value);

			/** Constructor with Parameter unsigned short.
					\param       value a unsigned short (0 - 255)
					\exception   InvalidRange if <tt> value > 255</tt>
			*/
			ColorUnit(const unsigned short value);

			/** Constructor with Parameter int.
					\param       value an int (0 - 255)
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
			*/
			ColorUnit(const int value);
		
			/** Constructor with Parameter unsigned int.
					\param       value an unsigned int (0 - 255)
					\exception   InvalidRange if <tt> value > 255</tt>
			*/
			ColorUnit(const unsigned int value);

			/** Constructor with Parameter long.
					\param       value an long (0 - 255)
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
			*/
			ColorUnit(const long value);

			/** Constructor with Parameter unsigned long.
					\param       value an unsigned long (0 - 255)
					\exception   InvalidRange if <tt> value > 255</tt>
			*/
			ColorUnit(const unsigned long value);
		
			/** Constructor with Parameter float.
					\param       value a float (0 - 1.0)
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			ColorUnit(const float value);
		
			/** Constructor with Parameter double.
					\param       value a double (0 - 1.0)
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			ColorUnit(const double value);

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
			*/
			~ColorUnit();

			/** Explicit default initialization.
					Sets the value of this colorUnit to the default value.
					The value of this colorUnit is:
						- value is zero (0)
					\par
			*/
			void clear();

			//@}
			/**	@name	Converters
			*/
			//@{

			/** Conversion to string.
					\return      string with format <tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>
			*/
			operator String () const;

			/** Conversion to char.
					\return      char with a value of range <tt> 0 - 255</tt>
			*/
			operator char () const;

			/** Conversion to unsigned char.
					\return      unsigned char with a value of range <tt> 0 - 255</tt>
			*/
			operator unsigned char () const;

			/** Conversion to short.
					\return      short with a value of range <tt> 0 - 255</tt>
			*/
			operator short () const;
			
			/** Conversion to unsigned short.
					\return      unsigned short with a value of range <tt> 0 - 255</tt>
			*/
			operator unsigned short	() const;
			
			/** Conversion to int.
					\return      int with a value of range <tt> 0 - 255</tt>
			*/
			operator int () const;

			/** Conversion to unsigned int.
					\return      unsigned int with a value of range <tt> 0 - 255</tt>
			*/
			operator unsigned int () const;

			/** Conversion to long.
					\return      long with a value of range <tt> 0 - 255</tt>
			*/
			operator long () const;

			/** Conversion to unsigned long.
					\return      unsigned long with a value of range <tt> 0 - 255</tt>
			*/
			operator unsigned long () const;

			/** Conversion to float.
					\return      float with a value of range <tt> 0 - 1.0</tt>
			*/
			operator float () const;

			/** Conversion to double.
					\return      double with a value of range <tt> 0 - 1.0</tt>
			*/
			operator double () const;
			
			//@}
			/**	@name	Assignment methods
			*/
			//@{
			
			/** Assignment.
			*/
			void set(const ColorUnit& color_unit);

			/** Assignment operator.
					\see         set
			*/
			const ColorUnit& operator = (const ColorUnit& color_unit);
		
			/** Swapping of colorUnits.
			*/
			void swap(ColorUnit& color_unit);

			//@}
			/** @name Accessors: inspectors and mutators 
			*/
			//@{ 

			/** Changes the value of the colorUnit.
					\param       value the new value of this colorUnit (a string in the format <tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
			*/
			void set(const char* value);

			/** Assignment operator with char*.
					Calls set.
					\param       value the pointer to a string containing a value in hex format (<tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
			*/
			const ColorUnit& operator = (const char* value);
		
			/** Inspection of the colorUnit's value.
					Memory must be allocated for this parameter. The memory must be at least
					the size of 3 bytes.
					\param   value a char* receiving the value of this colorUnit in hex format (<tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
			*/
			void get(char* value) const;

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					a string <b> value</b>.
					\param       value a string (in the format <tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
			*/
			void set(const String& value);

			/** Assignment operator with string.
					Calls set.
					\param       value a string containing a value in hex format (<tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
			*/
			const ColorUnit& operator = (const String& value);
		
			/** Inspection of the colorUnit's value.
					\param   value a string receiving the value of this colorUnit in hex format (<tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
			*/
			void get(String& value) const;

			/** Changes the value of the colorUnit.
					\param       value an unsigned char of range <tt> 0-255</tt>
			*/
			void set(const unsigned char value);

			/** Assignment operator with unsigned char.
					Calls set.
					\param       value an unsigned char containing a value of range <tt> 0-255</tt>
			*/
			const ColorUnit& operator = (const unsigned char value);

			/** Inspection of the colorUnit's value.
					\param   value an unsigned char receiving the value of this colorUnit (range: <tt> 0-255</tt>)
			*/
			void get(unsigned char& value) const;

			/** Changes the value of the colorUnit.
					\param       value a short of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
			*/
			void set(const short value);

			/** Assignment operator with short.
					Calls set.
					\param       value a short containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
			*/
			const ColorUnit& operator = (const short value);

			/** Inspection of the colorUnit's value.
					\param   value a short receiving the value of this colorUnit (range: <tt> 0-255</tt>)
			*/
			void get(short& value) const;

			/** Changes the value of the colorUnit.
					\param       value an unsigned short of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
			*/
			void set(const unsigned short value);

			/** Assignment operator with unsigned short.
					Calls set.
					\param       value an unsigned short containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
			*/
			const ColorUnit& operator = (const unsigned short value);

			/** Inspection of the colorUnit's value.
					\param   value an unsigned short receiving the value of this colorUnit (range: <tt> 0-255</tt>)
			*/
			void get(unsigned short& value) const;

			/** Changes the value of the colorUnit.
					\param       value an int of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
			*/
			void set(const int value);

			/** Assignment operator with int.
					Calls set.
					\param       value an int containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
			*/
			const ColorUnit& operator = (const int value);

			/** Inspection of the colorUnit's value.
					\param   value an int receiving the value of this colorUnit (range: <tt> 0-255</tt>)
			*/
			void get(int& value) const;

			/** Changes the value of the colorUnit.
					\param       value an unsigned int of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
			*/
			void set(const unsigned int value);

			/** Assignment operator with unsigned int.
					Calls set.
					\param       value an unsigned int containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
			*/
			const ColorUnit& operator = (const unsigned int value);

			/** Inspection of the colorUnit's value.
					\param   value an unsigned int receiving the value of this colorUnit (range: <tt> 0-255</tt>)
			*/
			void get(unsigned int& value) const;

			/** Changes the value of the colorUnit.
					\param       value a long of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
			*/
			void set(const long value);

			/** Assignment operator with long.
					Calls set.
					\param       value a long containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
			*/
			const ColorUnit& operator = (const long value);

			/** Inspection of the colorUnit's value.
					\param   value a long receiving the value of this colorUnit (range: <tt> 0-255</tt>)
			*/
			void get(long& value) const;

			/** Changes the value of the colorUnit.
					\param       value an unsigned long of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
			*/
			void set(const unsigned long value);

			/** Assignment operator with unsigned long.
					Calls set.
					\param       value an unsigned long containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
			*/
			const ColorUnit& operator = (const unsigned long value);

			/** Inspection of the colorUnit's value.
					\param   value an unsigned long receiving the value of this colorUnit (range: <tt> 0-255</tt>)
			*/
			void get(unsigned long& value) const;

			/** Changes the value of the colorUnit.
					\param       value a float of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			void set(const float value);

			/** Assignment operator with float.
					Calls set.
					\param       value a float containing a value of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			const ColorUnit& operator = (const float value);

			/** Inspection of the colorUnit's value.
					\param   value a float receiving the value of this colorUnit (range: 0-1.0)
			*/
			void get(float& value) const;

			/** Changes the value of the colorUnit.
					\param       value a double of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			void set(const double value);

			/** Assignment operator with double.
					Calls set.
					\param       value a double containing a value of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
			*/
			const ColorUnit& operator = (const double value);

			/** Inspection of the colorUnit's value.
					\param   value a double receiving the value of this colorUnit (range: 0-1.0)
			*/
			void get(double& value) const;

			//@}
			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
					Equality means that the internals value (floats) of the colorUnit's
					are equal.
			*/
			bool operator == (const ColorUnit& color_unit) const;

			/** Unequality test.
					Unequality means that the internals value (floats) of the colorUnit's
					are unequal.
			*/
			bool operator != (const ColorUnit& color_unit) const;
			
			/** Smaller test.
					Smaller means that the internal value (float) this is
					smaller than the internal value (float) of <b> color_unit</b> colorUnit.
			*/
			bool operator < (const ColorUnit& color_unit) const;
			
			/** Smaller or equal test.
					Smaller or equal means that the internal value (float) this is
					smaller or equal than the internal value (float) of <b> color_unit</b> colorUnit.
			*/
			bool operator <= (const ColorUnit& color_unit) const;
			
			/** Greater test.
					Greater means that the internal value (float) this is
					greater than the internal value (float) of <b> color_unit</b> colorUnit.
			*/
			bool operator > (const ColorUnit& color_unit) const;

			/** Greater or equal test.
					Greater or equal means that the internal value (float) this is
					greater or equal than the internal value (float) of <b> color_unit</b> colorUnit.
			*/ 
			bool operator >= (const ColorUnit& color_unit) const;
			
			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dumps the current value of this colorUnit to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					The value will be outputed as an integer of range <tt> 0-255</tt>.
					\param   s output stream where to output the value of this colorUnit
					\param   depth the dumping depth (depth is irrelevant here)
			*/
			void dump(std::ostream& s = std::cout, Size depth = 0) const;

			//@}
			/**	@name	Storers
			*/
			//@{

			/** Friendly stream input.
					Reads colorUnit data from the input stream <b> s</b> and restore the value of this.
					The value will be needed in integer format of range <tt> 0-255</tt>.
					This method is provided for convienience.
					\param  s input stream from where to restore the value of this colorUnit
					\param  color_unit the colorUnit to which the value will be restored
			*/
			friend std::istream& operator >> (std::istream& s, ColorUnit& color_unit);

			/** Friendly stream output.
					Writes colorUnit data to the output stream <b> s</b> and store the value of this.
					The value will be stored in integer format of range <tt> 0-255</tt>.
					\param  s output stream to where the value of this colorUnit will be stored
					\param  color_unit the colorUnit from which the value will be stored
			*/
			friend std::ostream& operator << (std::ostream& s, const ColorUnit& color_unit);
			//@}

			private:
		
			// @throws Exception::InvalidRange
			// @throws Exception::NotInHexFormat
			float hexToFloat_(const char* value);
			
			float value_;
		
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorUnit.iC>
#		endif
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DATATYPE_COLORUNIT_H
