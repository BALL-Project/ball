// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorUnit.h,v 1.23 2003/11/27 17:30:51 oliver Exp $
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
#include <stdio.h>

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
		*/
		class ColorUnit
		{
			public:
			
			/**	@name	Exceptions
			*/
			//@{
			
			/** NotInHexFormat Exception class.
					This exeption will be thrown if a given string for the colorUnit class
					is not in the format "00" - "FF" or "00" - "ff".

					\see         Exception::GeneralException			
			*/
			class NotInHexFormat:	public Exception::GeneralException
			{
				public:

				NotInHexFormat(const char* file, int line, const string& data)
					throw();
			};
			//@}

			/**	@name Constructors
			*/
			//@{

			/** Default Constructor.
					Constructs new colorUnit. Its value will be set to zero (0).
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit()
				throw();
		
			/** Copy constructor.
					Constructs new colorUnit by copying the colorUnit <b> color_unit</b>.
					\param       color_unit the colorUnit to be copied
					\return      ColorUnit new constructed colorUnit copied from <b> color_unit</b>
			*/
			ColorUnit(const ColorUnit& color_unit)
				throw();

			/** Constructor with Parameter char*.
					Constructs new colorUnit by initialising its value with the given pointer to
					a string representing a value in hex format (00 - FF, or 00 - ff).
					\param       value the pointer to a string (00 - FF, or 00 - ff)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit(const char* value)
				throw(Exception::InvalidRange, NotInHexFormat);

			/** Constructor with Parameter String.
					Constructs new colorUnit by initialising its value with the given 
					string representing a value in hex format (00 - FF, or 00 - ff).
					\param       value a string (00 - FF, or 00 - ff)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit(const String& value)
				throw(Exception::InvalidRange, NotInHexFormat);

			/** Constructor with Parameter unsigned char.
					Constructs new colorUnit by initialising its value with the given 
					unsigned char representing a value in integer format (0 - 255).
					\param       value a char (0 - 255)
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit(const unsigned char value)
				throw();

			/** Constructor with Parameter short.
					Constructs new colorUnit by initialising its value with the given 
					short representing a value in integer format (0 - 255).
					\param       value a short (0 - 255)
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit(const short value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter unsigned short.
					Constructs new colorUnit by initialising its value with the given 
					unsigned short representing a value in integer format (0 - 255).
					\param       value a unsigned short (0 - 255)
					\exception   InvalidRange if <tt> value > 255</tt>
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit(const unsigned short value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter int.
					Constructs new colorUnit by initialising its value with the given 
					int representing a value in integer format (0 - 255).
					\param       value an int (0 - 255)
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit(const int value)
				throw(Exception::InvalidRange);
		
			/** Constructor with Parameter unsigned int.
					Constructs new colorUnit by initialising its value with the given 
					unsigned int representing a value in integer format (0 - 255).
					\param       value an unsigned int (0 - 255)
					\exception   InvalidRange if <tt> value > 255</tt>
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit(const unsigned int value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter long.
					Constructs new colorUnit by initialising its value with the given 
					long representing a value in integer format (0 - 255).
					\param       value an long (0 - 255)
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit(const long value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter unsigned long.
					Constructs new colorUnit by initialising its value with the given 
					unsigned long representing a value in integer format (0 - 255).
					\param       value an unsigned long (0 - 255)
					\exception   InvalidRange if <tt> value > 255</tt>
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit(const unsigned long value)
				throw(Exception::InvalidRange);
		
			/** Constructor with Parameter float.
					Constructs new colorUnit by initialising its value with the given 
					float representing a value of range (0 - 1.0).
					\param       value a float (0 - 1.0)
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit(const float value)
				throw(Exception::InvalidRange);
		
			/** Constructor with Parameter double.
					Constructs new colorUnit by initialising its value with the given 
					double representing a value of range (0 - 1.0).
					\param       value a double (0 - 1.0)
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
					\return      ColorUnit new constructed colorUnit
			*/
			ColorUnit(const double value)
				throw(Exception::InvalidRange);

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
			*/
			~ColorUnit()
				throw();

			/** Explicit default initialization.
					Sets the value of this colorUnit to the default value.
					The value of this colorUnit is:
						- value is zero (0)
					\par
			*/
			void clear()
				throw();

			//@}
			/**	@name	Converters
			*/
			//@{

			/** Conversion to string.
					This operator casts an instance of colorUnit to the string type.
					\return      string with format <tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>
			*/
			operator String () const
				throw();

			/** Conversion to char.
					This operator casts an instance of colorUnit to the char type.
					\return      char with a value of range <tt> 0 - 255</tt>
			*/
			operator char () const
				throw();

			/** Conversion to unsigned char.
					This operator casts an instance of colorUnit to the unsigned char type.
					\return      unsigned char with a value of range <tt> 0 - 255</tt>
			*/
			operator unsigned char () const
				throw();

			/** Conversion to short.
					This operator casts an instance of colorUnit to the short type.
					\return      short with a value of range <tt> 0 - 255</tt>
			*/
			operator short () const
				throw();
			
			/** Conversion to unsigned short.
					This operator casts an instance of colorUnit to the unsigned short type.
					\return      unsigned short with a value of range <tt> 0 - 255</tt>
			*/
			operator unsigned short	() const
				throw();
			
			/** Conversion to int.
					This operator casts an instance of colorUnit to the int type.
					\return      int with a value of range <tt> 0 - 255</tt>
			*/
			operator int () const
				throw();

			/** Conversion to unsigned int.
					This operator casts an instance of colorUnit to the unsigned int type.
					\return      unsigned int with a value of range <tt> 0 - 255</tt>
			*/
			operator unsigned int () const
				throw();

			/** Conversion to long.
					This operator casts an instance of colorUnit to the long type.
					\return      long with a value of range <tt> 0 - 255</tt>
			*/
			operator long () const
				throw();

			/** Conversion to unsigned long.
					This operator casts an instance of colorUnit to the unsigned long type.
					\return      unsigned long with a value of range <tt> 0 - 255</tt>
			*/
			operator unsigned long () const
				throw();

			/** Conversion to float.
					This operator casts an instance of colorUnit to the float type.
					\return      float with a value of range <tt> 0 - 1.0</tt>
			*/
			operator float () const
				throw();

			/** Conversion to double.
					This operator casts an instance of colorUnit to the double type.
					\return      double with a value of range <tt> 0 - 1.0</tt>
			*/
			operator double () const
				throw();
			
			//@}

			/**	@name	Assignment methods
			*/
			//@{
			
			/** Assignment.
			*/
			void set(const ColorUnit& color_unit)
				throw();

			/** Assignment operator.
					\see         set

			*/
			const ColorUnit& operator = (const ColorUnit& color_unit)
				throw();
		
			/** Swapping of colorUnits.
					Swaps the values of this colorUnit with the colorUnit <b> color_unit</b>.
					\param       color_unit the colorUnit being swapped with this colorUnit 
					\see         ColorUnit
			*/
			void swap(ColorUnit& color_unit)
				throw();

			//@}

			/** @name Accessors: inspectors and mutators 
			*/
			//@{ 

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					a pointer <b> value</b> to a string.
					\param       value the new value of this colorUnit (a string in the format <tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
					\see         get
			*/
			void set(const char* value)
				throw(Exception::InvalidRange, NotInHexFormat);

			/** Assignment operator with char*.
					Assigns the contents of <b> value</b> to this colorUnit.
					Calls set.
					\param       value the pointer to a string containing a value in hex format (<tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
					\return      ColorUnit& constant reference of this colorUnit
					\see         set
			*/
			const ColorUnit& operator = (const char* value)
				throw(Exception::InvalidRange, NotInHexFormat);
		
			/** Inspection of the colorUnit's value.
					Accesses the value of this colorUnit by using a char pointer.
					Memory must be allocated for this parameter. The memory must be at least
					the size of 3 bytes.
					\param   value a char* receiving the value of this colorUnit in hex format (<tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
					\see     set
			*/
			void get(char* value) const
				throw();

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					a string <b> value</b>.
					\param       value a string (in the format <tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
					\see         get
			*/
			void set(const String& value)
				throw(Exception::InvalidRange, NotInHexFormat);

			/** Assignment operator with string.
					Assigns the contents of <b> value</b> to this colorUnit.
					Calls set.
					\param       value a string containing a value in hex format (<tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
					\exception   InvalidRange if <tt> string length != 2</tt>
					\exception   NotInHexFormat if string contains characters g-z or G-Z
					\return      ColorUnit& constant reference of this colorUnit
					\see         set
			*/
			const ColorUnit& operator = (const String& value)
				throw(Exception::InvalidRange, NotInHexFormat);
		
			/** Inspection of the colorUnit's value.
					Accesses the value of this colorUnit by using a string.
					\param   value a string receiving the value of this colorUnit in hex format (<tt> "00"-"FF"</tt> or <tt> "00"-"ff"</tt>)
					\see     set
			*/
			void get(String& value) const
				throw();

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					a unsigned char <b> value</b>.
					\param       value an unsigned char of range <tt> 0-255</tt>
					\see         get
			*/
			void set(const unsigned char value)
				throw();

			/** Assignment operator with unsigned char.
					Assigns the contents of <b> value</b> to this colorUnit.
					Calls set.
					\param       value an unsigned char containing a value of range <tt> 0-255</tt>
					\return      ColorUnit& constant reference of this colorUnit
					\see         set
			*/
			const ColorUnit& operator = (const unsigned char value)
				throw();

			/** Inspection of the colorUnit's value.
					Accesses the value of this colorUnit by using an unsigned char.
					\param   value an unsigned char receiving the value of this colorUnit (range: <tt> 0-255</tt>)
					\see     set
			*/
			void get(unsigned char& value) const
				throw();

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					a short <b> value</b>.
					\param       value a short of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
					\see         get
			*/
			void set(const short value)
				throw(Exception::InvalidRange);

			/** Assignment operator with short.
					Assigns the contents of <b> value</b> to this colorUnit.
					Calls set.
					\param       value a short containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
					\return      ColorUnit& constant reference of this colorUnit
					\see         set
			*/
			const ColorUnit& operator = (const short value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnit's value.
					Accesses the value of this colorUnit by using a short.
					\param   value a short receiving the value of this colorUnit (range: <tt> 0-255</tt>)
					\see     set
			*/
			void get(short& value) const
				throw();

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					an unsigned short <b> value</b>.
					\param       value an unsigned short of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
					\see         get
			*/
			void set(const unsigned short value)
				throw(Exception::InvalidRange);

			/** Assignment operator with unsigned short.
					Assigns the contents of <b> value</b> to this colorUnit.
					Calls set.
					\param       value an unsigned short containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
					\return      ColorUnit& constant reference of this colorUnit
					\see         set
			*/
			const ColorUnit& operator = (const unsigned short value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnit's value.
					Accesses the value of this colorUnit by using an unsigned short.
					\param   value an unsigned short receiving the value of this colorUnit (range: <tt> 0-255</tt>)
					\see     set
			*/
			void get(unsigned short& value) const
				throw();

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					an int <b> value</b>.
					\param       value an int of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
					\see         get
			*/
			void set(const int value)
				throw(Exception::InvalidRange);

			/** Assignment operator with int.
					Assigns the contents of <b> value</b> to this colorUnit.
					Calls set.
					\param       value an int containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
					\return      ColorUnit& constant reference of this colorUnit
					\see         set
			*/
			const ColorUnit& operator = (const int value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnit's value.
					Accesses the value of this colorUnit by using an int.
					\param   value an int receiving the value of this colorUnit (range: <tt> 0-255</tt>)
					\see     set
			*/
			void get(int& value) const
				throw();

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					an unsigned int <b> value</b>.
					\param       value an unsigned int of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
					\see         get
			*/
			void set(const unsigned int value)
				throw(Exception::InvalidRange);

			/** Assignment operator with unsigned int.
					Assigns the contents of <b> value</b> to this colorUnit.
					Calls set.
					\param       value an unsigned int containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
					\return      ColorUnit& constant reference of this colorUnit
					\see         set
			*/
			const ColorUnit& operator = (const unsigned int value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnit's value.
					Accesses the value of this colorUnit by using an unsigned int.
					\param   value an unsigned int receiving the value of this colorUnit (range: <tt> 0-255</tt>)
					\see     set
			*/
			void get(unsigned int& value) const
				throw();

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					a long <b> value</b>.
					\param       value a long of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
					\see         get
			*/
			void set(const long value)
				throw(Exception::InvalidRange);

			/** Assignment operator with long.
					Assigns the contents of <b> value</b> to this colorUnit.
					Calls set.
					\param       value a long containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value < 0 || value > 255</tt>
					\return      ColorUnit& constant reference of this colorUnit
					\see         set
			*/
			const ColorUnit& operator = (const long value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnit's value.
					Accesses the value of this colorUnit by using a long.
					\param   value a long receiving the value of this colorUnit (range: <tt> 0-255</tt>)
					\see     set
			*/
			void get(long& value) const
				throw();

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					an unsigned long <b> value</b>.
					\param       value an unsigned long of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
					\see         get
			*/
			void set(const unsigned long value)
				throw(Exception::InvalidRange);

			/** Assignment operator with unsigned long.
					Assigns the contents of <b> value</b> to this colorUnit.
					Calls set.
					\param       value an unsigned long containing a value of range <tt> 0-255</tt>
					\exception   InvalidRange if <tt> value > 255</tt>
					\return      ColorUnit& constant reference of this colorUnit
					\see         set
			*/
			const ColorUnit& operator = (const unsigned long value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnit's value.
					Accesses the value of this colorUnit by using an unsigned long.
					\param   value an unsigned long receiving the value of this colorUnit (range: <tt> 0-255</tt>)
					\see     set
			*/
			void get(unsigned long& value) const
				throw();

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					a float <b> value</b>.
					\param       value a float of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
					\see         get
			*/
			void set(const float value)
				throw(Exception::InvalidRange);

			/** Assignment operator with float.
					Assigns the contents of <b> value</b> to this colorUnit.
					Calls set.
					\param       value a float containing a value of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
					\return      ColorUnit& constant reference of this colorUnit
					\see         set
			*/
			const ColorUnit& operator = (const float value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnit's value.
					Accesses the value of this colorUnit by using a float.
					\param   value a float receiving the value of this colorUnit (range: 0-1.0)
					\see     set
			*/
			void get(float& value) const
				throw();

			/** Changes the value of the colorUnit.
					Changes the value of this colorUnit to the value represented by 
					a double <b> value</b>.
					\param       value a double of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
					\see         get
			*/
			void set(const double value)
				throw(Exception::InvalidRange);

			/** Assignment operator with double.
					Assigns the contents of <b> value</b> to this colorUnit.
					Calls set.
					\param       value a double containing a value of range 0-1.0
					\exception   InvalidRange if <tt> value < 0.0 || value > 1.0</tt>
					\return      ColorUnit& constant reference of this colorUnit
					\see         set
			*/
			const ColorUnit& operator = (const double value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnit's value.
					Accesses the value of this colorUnit by using a double.
					\param   value a double receiving the value of this colorUnit (range: 0-1.0)
					\see     set
			*/
			void get(double& value) const
				throw();

			//@}

			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
					Tests if <b> color_unit</b> is equal to this colorUnit.
					Equality means that the internals value (floats) of the colorUnit's
					are equal.
					\param   color_unit the colorUnit to be compared with
					\return  bool <tt> true</b> if the value of this is equal to <b> color_unit</b>, <tt> false</b> otherwise
			*/
			bool operator == (const ColorUnit& color_unit) const
				throw();

			/** Unequality test.
					Tests if <b> color_unit</b> is not equal to this colorUnit.
					Unequality means that the internals value (floats) of the colorUnit's
					are unequal.
					\param   color_unit the colorUnit to be compared with
					\return  bool	<tt> true</b> if the value of this is not equal to <b> color_unit</b>, <tt> false</b> otherwise
			*/
			bool operator != (const ColorUnit& color_unit) const
				throw();
			
			/** Smaller test.
					Tests if this is smaller than <b> color_unit</b> colorUnit.
					Smaller means that the internal value (float) this is
					smaller than the internal value (float) of <b> color_unit</b> colorUnit.
					\param   color_unit the colorUnit to be compared with
					\return  bool	<tt> true</b> if the value of this is smaller than <b> color_unit</b>, <tt> false</b> otherwise
			*/
			bool operator < (const ColorUnit& color_unit) const
				throw();
			
			/** Smaller or equal test.
					Tests if this is smaller or equal than <b> color_unit</b> colorUnit.
					Smaller or equal means that the internal value (float) this is
					smaller or equal than the internal value (float) of <b> color_unit</b> colorUnit.
					\param   color_unit the colorUnit to be compared with
					\return  bool	<tt> true</b> if the value of this is smaller or equal than <b> color_unit</b>, <tt> false</b> otherwise
			*/
			bool operator <= (const ColorUnit& color_unit) const
				throw();
			
			/** Greater test.
					Tests if this is greater than <b> color_unit</b> colorUnit.
					Greater means that the internal value (float) this is
					greater than the internal value (float) of <b> color_unit</b> colorUnit.
					\param   color_unit the colorUnit to be compared with
					\return  bool	<tt> true</b> if the value of this is greater than <b> color_unit</b>, <tt> false</b> otherwise
			*/
			bool operator > (const ColorUnit& color_unit) const
				throw();

			/** Greater or equal test.
					Tests if this is greater or equal than <b> color_unit</b> colorUnit.
					Greater or equal means that the internal value (float) this is
					greater or equal than the internal value (float) of <b> color_unit</b> colorUnit.
					\param   color_unit the colorUnit to be compared with
					\return  bool	<tt> true</b> if the value of this is greater or equal than <b> color_unit</b>, <tt> false</b> otherwise
			*/ 
			bool operator >= (const ColorUnit& color_unit) const
				throw();
			
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
			void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

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
		
			float hexToFloat_(const char* value)
				throw(Exception::InvalidRange, NotInHexFormat);
			
			float value_;
		
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorUnit.iC>
#		endif
  
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_DATATYPE_COLORUNIT_H
