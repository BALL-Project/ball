// $Id: colorUnit.h,v 1.6 2001/02/04 15:58:18 hekl Exp $

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

				{\bf Framework:} BALL/VIEW/DATATYPE\\
				{\bf Definition:} \URL{BALL/VIEW/DATATYPE/colorUnit.h}
				\\
				
				An instance of ColorUnit can represent a value for the red, green, blue
				and alpha component of the class \Ref{ColorRGBA}. Furthermore this class is
				used for the value and saturation component of the class \Ref{ColorHSV}.
				The class handles the many possible variations a color value can be set
				(e.g. integer, float, string values can be given as parameter). The integer
				values are of range 0-255, the float values are of range 0-1.0 and the strings
				given are in hex format "00"-"FF".
				ColorUnit provides certain conversion functions as well that will convert
				its value to the needed type.
				This class is used by the class \Ref{ColorRGBA} and \Ref{ColorHSV}.

				@memo    ColorUnit class (BALL VIEW datatype framework)
				@author  $Author: hekl $
				@version $Revision: 1.6 $
				@date    $Date: 2001/02/04 15:58:18 $
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

					@see         Exception::GeneralException			
			*/
			class NotInHexFormat:	public Exception::GeneralException
			{
				public:

				NotInHexFormat(const char* file, int line, const string& data);
			};
			//@}

			/**	@name Constructors
			*/
			//@{

			/** Default Constructor.
					Construct new colorUnit. Its value will be set to zero (0).

					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit();
		
			/** Copy constructor.
					Construct new colorUnit by copying the colorUnit {\em color_unit}.

					@param       color_unit the colorUnit to be copied
					@return      ColorUnit - new constructed colorUnit copied from {\em color_unit}
			*/
			ColorUnit(const ColorUnit& color_unit);

			/** Constructor with Parameter char*
					Construct new colorUnit by initialising its value with the given pointer to
					a string representing a value in hex format (00 - FF, or 00 - ff).

					@param       value the pointer to a string (00 - FF, or 00 - ff)
					@exception   InvalidRange - if {\tt string length != 2}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const char* value);

			/** Constructor with Parameter String
					Construct new colorUnit by initialising its value with the given 
					string representing a value in hex format (00 - FF, or 00 - ff).

					@param       value a string (00 - FF, or 00 - ff)
					@exception   InvalidRange - if {\tt string length != 2}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const String& value);

			/** Constructor with Parameter unsigned char
					Construct new colorUnit by initialising its value with the given 
					unsigned char representing a value in integer format (0 - 255).

					@param       value a char value (0 - 255)
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const unsigned char value);

			/** Constructor with Parameter short
					Construct new colorUnit by initialising its value with the given 
					short representing a value in integer format (0 - 255).

					@param       value a short value (0 - 255)
					@exception   InvalidRange - if {\tt value < 0 || value > 255}
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const short value);

			/** Constructor with Parameter unsigned short
					Construct new colorUnit by initialising its value with the given 
					unsigned short representing a value in integer format (0 - 255).

					@param       value a unsigned short value (0 - 255)
					@exception   InvalidRange - if {\tt value > 255}
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const unsigned short value);

			/** Constructor with Parameter int
					Construct new colorUnit by initialising its value with the given 
					int representing a value in integer format (0 - 255).

					@param       value an int value (0 - 255)
					@exception   InvalidRange - if {\tt value < 0 || value > 255}
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const int value);
		
			/** Constructor with Parameter unsigned int
					Construct new colorUnit by initialising its value with the given 
					unsigned int representing a value in integer format (0 - 255).

					@param       value an unsigned int value (0 - 255)
					@exception   InvalidRange - if {\tt value > 255}
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const unsigned int value);

			/** Constructor with Parameter long
					Construct new colorUnit by initialising its value with the given 
					long representing a value in integer format (0 - 255).

					@param       value an long value (0 - 255)
					@exception   InvalidRange - if {\tt value < 0 || value > 255}
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const long value);

			/** Constructor with Parameter unsigned long
					Construct new colorUnit by initialising its value with the given 
					unsigned long representing a value in integer format (0 - 255).

					@param       value an unsigned long value (0 - 255)
					@exception   InvalidRange - if {\tt value > 255}
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const unsigned long value);
		
			/** Constructor with Parameter float
					Construct new colorUnit by initialising its value with the given 
					float representing a value of range (0 - 1.0).

					@param       value a float value (0 - 1.0)
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const float value);
		
			/** Constructor with Parameter double
					Construct new colorUnit by initialising its value with the given 
					double representing a value of range (0 - 1.0).

					@param       value a double value (0 - 1.0)
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const double value);

			/** Constructor with Parameter long double
					Construct new colorUnit by initialising its value with the given 
					long double representing a value of range (0 - 1.0).

					@param       value a long double value (0 - 1.0)
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnit(const long double value);

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} colorUnit.

					Calls \Ref{ColorUnit::destroy}.
					@see         ColorUnit::destroy
			*/
			virtual ~ColorUnit();

			/** Explicit default initialization.
					Set the value of {\em *this} colorUnit to the default value.
					The value of {\em *this} colorUnit is:
					\begin{itemize}
						\item value is zero (0)
					\end{itemize}
			*/
			virtual void clear();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy();
				
			//@}

			/**	@name	Converters
			*/
			//@{

			/** Conversion to string.
					This operator casts an instance of colorUnit to the string type.
					
					@return      a string with format "00"-"FF" or "00"-"ff"
			*/
			operator String () const;

			/** Conversion to char.
					This operator casts an instance of colorUnit to the char type.

					@return      a char with a value of range 0 - 255
			*/
			operator char () const;

			/** Conversion to unsigned char.
					This operator casts an instance of colorUnit to the unsigned char type.

					@return      a unsigned char with a value of range 0 - 255
			*/
			operator unsigned char () const;

			/** Conversion to short.
					This operator casts an instance of colorUnit to the short type.

					@return      a short with a value of range 0 - 255
			*/
			operator short () const;
			
			/** Conversion to unsigned short.
					This operator casts an instance of colorUnit to the unsigned short type.

					@return      a unsigned short with a value of range 0 - 255
			*/
			operator unsigned short	() const;
			
			/** Conversion to int.
					This operator casts an instance of colorUnit to the int type.

					@return      a int with a value of range 0 - 255
			*/
			operator int () const;

			/** Conversion to unsigned int.
					This operator casts an instance of colorUnit to the unsigned int type.

					@return      a unsigned int with a value of range 0 - 255
			*/
			operator unsigned int () const;

			/** Conversion to long.
					This operator casts an instance of colorUnit to the long type.

					@return      a long with a value of range 0 - 255
			*/
			operator long () const;

			/** Conversion to unsigned long.
					This operator casts an instance of colorUnit to the unsigned long type.

					@return      a unsigned long with a value of range 0 - 255
			*/
			operator unsigned long () const;

			/** Conversion to float.
					This operator casts an instance of colorUnit to the float type.

					@return      a float with a value of range 0 - 1.0
			*/
			operator float () const;

			/** Conversion to double.
					This operator casts an instance of colorUnit to the double type.

					@return      a double with a value of range 0 - 1.0
			*/
			operator double () const;
			
			/** Conversion to long double.
					This operator casts an instance of colorUnit to the long double type.

					@return      a long double with a value of range 0 - 1.0
			*/
			operator long double () const;

			//@}

			/**	@name	Assignment methods
			*/
			//@{
			
			/** Assignment.
					Assign the colorUnit {\em color_unit} to {\em *this} colorUnit.
					The value of {\em *this} is initialized to the value of the colorUnit {\em color_unit}.\\

					@param       color_unit the colorUnit to be copied 
			*/
			void set(const ColorUnit& color_unit);

			/** Assignment operator.
					Assign the colorUnit {\em color_unit} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.
					The value of {\em *this} is initialized to the value of the colorUnit {\em color_unit}.\\

					@param       color_unit the colorUnit to be copied
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const ColorUnit& color_unit);
		
			/** Copying.
					Copy {\em *this} colorUnit to the colorUnit {\em color_unit}.
					Calls \Ref{ColorUnit::set}.
					The value of the colorUnit {\em color_unit} is initialized to the value of {\em *this} colorUnit.\\

					@param       color_unit the colorUnit to be assigned to
					@see         ColorUnit::set
			*/
			void get(ColorUnit& color_unit) const;

			/** Swapping of colorUnits.
					Swap the values of {\em *this} colorUnit with the colorUnit {\em color_unit}.
					@param       color_unit the colorUnit being swapped with {\em *this} colorUnit 
					@see         ColorUnit::ColorUnit
			*/
			void swap(ColorUnit& color_unit);

			//@}

			/** @name Accessors: inspectors and mutators 
			*/
			//@{ 

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					a pointer {\em value} to a string.

					@param       value the new value of {\em *this} colorUnit (a string in the format "00"-"FF" or "00"-"ff")

					@exception   InvalidRange - if {\tt string length != 2}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z
					@see         ColorUnit::getValue
			*/
			void set(const char* value);

			/** Assignment operator with char*
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value the pointer to a string containing a value in hex format ("00"-"FF" or "00"-"ff")
					@exception   InvalidRange - if {\tt string length != 2}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const char* value);
		
			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using a char pointer.
					Memory must be allocated for this parameter. The memory must be at least
					the size of 3 bytes.
					
					@param   value a char* receiving the value of {\em *this} colorUnit in hex format ("00"-"FF" or "00"-"ff")
					@see     ColorUnit::setValue
			*/
			void get(char* value) const;

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					a string {\em value}.

					@param       value a string (in the format "00"-"FF" or "00"-"ff")
					@exception   InvalidRange - if {\tt string length != 2}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z
					@see         ColorUnit::getValue
			*/
			void set(const String& value);

			/** Assignment operator with string
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value a string containing a value in hex format ("00"-"FF" or "00"-"ff")
					@exception   InvalidRange - if {\tt string length != 2}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const String& value);
		
			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using a string.
					
					@param   value a string receiving the value of {\em *this} colorUnit in hex format ("00"-"FF" or "00"-"ff")
					@see     ColorUnit::setValue
			*/
			void get(String& value) const;

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					a unsigned char {\em value}.

					@param       value an unsigned char of range 0-255
					@see         ColorUnit::getValue
			*/
			void set(const unsigned char value);

			/** Assignment operator with unsigned char
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value an unsigned char containing a value of range 0-255
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const unsigned char value);

			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using an unsigned char.
					
					@param   value an unsigned char receiving the value of {\em *this} colorUnit (range: 0-255)
					@see     ColorUnit::setValue
			*/
			void get(unsigned char& value) const;

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					a short {\em value}.

					@param       value a short of range 0-255
					@exception   InvalidRange - if {\tt value < 0 || value > 255}
					@see         ColorUnit::getValue
			*/
			void set(const short value);

			/** Assignment operator with short
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value a short containing a value of range 0-255
					@exception   InvalidRange - if {\tt value < 0 || value > 255}
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const short value);

			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using a short.
					
					@param   value a short receiving the value of {\em *this} colorUnit (range: 0-255)
					@see     ColorUnit::setValue
			*/
			void get(short& value) const;

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					an unsigned short {\em value}.

					@param       value an unsigned short of range 0-255
					@exception   InvalidRange - if {\tt value > 255}
					@see         ColorUnit::getValue
			*/
			void set(const unsigned short value);

			/** Assignment operator with unsigned short
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value an unsigned short containing a value of range 0-255
					@exception   InvalidRange - if {\tt value > 255}
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const unsigned short value);

			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using an unsigned short.
					
					@param   value an unsigned short receiving the value of {\em *this} colorUnit (range: 0-255)
					@see     ColorUnit::setValue
			*/
			void get(unsigned short& value) const;

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					an int {\em value}.

					@param       value an int of range 0-255
					@exception   InvalidRange - if {\tt value < 0 || value > 255}
					@see         ColorUnit::getValue
			*/
			void set(const int value);

			/** Assignment operator with int
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value an int containing a value of range 0-255
					@exception   InvalidRange - if {\tt value < 0 || value > 255}
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const int value);

			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using an int.
					
					@param   value an int receiving the value of {\em *this} colorUnit (range: 0-255)
					@see     ColorUnit::setValue
			*/
			void get(int& value) const;

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					an unsigned int {\em value}.

					@param       value an unsigned int of range 0-255
					@exception   InvalidRange - if {\tt value > 255}
					@see         ColorUnit::getValue
			*/
			void set(const unsigned int value);

			/** Assignment operator with unsigned int
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value an unsigned int containing a value of range 0-255
					@exception   InvalidRange - if {\tt value > 255}
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const unsigned int value);

			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using an unsigned int.
					
					@param   value an unsigned int receiving the value of {\em *this} colorUnit (range: 0-255)
					@see     ColorUnit::setValue
			*/
			void get(unsigned int& value) const;

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					a long {\em value}.

					@param       value a long of range 0-255
					@exception   InvalidRange - if {\tt value < 0 || value > 255}
					@see         ColorUnit::getValue
			*/
			void set(const long value);

			/** Assignment operator with long
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value a long containing a value of range 0-255
					@exception   InvalidRange - if {\tt value < 0 || value > 255}
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const long value);

			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using a long.
					
					@param   value a long receiving the value of {\em *this} colorUnit (range: 0-255)
					@see     ColorUnit::setValue
			*/
			void get(long& value) const;

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					an unsigned long {\em value}.

					@param       value an unsigned long of range 0-255
					@exception   InvalidRange - if {\tt value > 255}
					@see         ColorUnit::getValue
			*/
			void set(const unsigned long value);

			/** Assignment operator with unsigned long
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value an unsigned long containing a value of range 0-255
					@exception   InvalidRange - if {\tt value > 255}
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const unsigned long value);

			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using an unsigned long.
					
					@param   value an unsigned long receiving the value of {\em *this} colorUnit (range: 0-255)
					@see     ColorUnit::setValue
			*/
			void get(unsigned long& value) const;

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					a float {\em value}.

					@param       value a float of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@see         ColorUnit::getValue
			*/
			void set(const float value);

			/** Assignment operator with float
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value a float containing a value of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const float value);

			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using a float.
					
					@param   value a float receiving the value of {\em *this} colorUnit (range: 0-1.0)
					@see     ColorUnit::setValue
			*/
			void get(float& value) const;

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					a double {\em value}.

					@param       value a double of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@see         ColorUnit::getValue
			*/
			void set(const double value);

			/** Assignment operator with double
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value a double containing a value of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const double value);

			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using a double.
					
					@param   value a double receiving the value of {\em *this} colorUnit (range: 0-1.0)
					@see     ColorUnit::setValue
			*/
			void get(double& value) const;

			/** Change the value of the colorUnit.
					Change the value of {\em *this} colorUnit to the value represented by 
					a long double {\em value}.

					@param       value a long double of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@see         ColorUnit::getValue
			*/
			void set(const long double value);

			/** Assignment operator with long double
					Assign the contents of {\em value} to {\em *this} colorUnit.
					Calls \Ref{ColorUnit::set}.

					@param       value a long double containing a value of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnit& - {\em *this} colorUnit
					@see         ColorUnit::set
			*/
			ColorUnit& operator = (const long double value);

			/** Inspection of the colorUnit's value.
					Access the value of {\em *this} colorUnit by using a long double.
					
					@param   value a long double receiving the value of {\em *this} colorUnit (range: 0-1.0)
					@see     ColorUnit::setValue
			*/
			void get(long double& value) const;
			
			//@}

			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
					Test if {\em color_unit} is equal to {\em *this} colorUnit.
					Equality means that the internals value (floats) of the colorUnit's
					are equal.

					@param   color_unit the colorUnit to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is equal to {\em color_unit}, {\tt false} otherwise
			*/
			bool operator == (const ColorUnit& color_unit) const;

			/** Unequality test.
					Test if {\em color_unit} is not equal to {\em *this} colorUnit.
					Unequality means that the internals value (floats) of the colorUnit's
					are unequal.

					@param   color_unit the colorUnit to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is not equal to {\em color_unit}, {\tt false} otherwise
			*/
			bool operator != (const ColorUnit& color_unit) const;
			
			/** Smaller test.
					Test if {\em *this} is smaller than {\em color_unit} colorUnit.
					Smaller means that the internal value (float) {\em *this} is
					smaller than the internal value (float) of {\em color_unit} colorUnit.

					@param   color_unit the colorUnit to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is smaller than {\em color_unit}, {\tt false} otherwise
			*/
			bool operator < (const ColorUnit& color_unit) const;
			
			/** Smaller or equal test.
					Test if {\em *this} is smaller or equal than {\em color_unit} colorUnit.
					Smaller or equal means that the internal value (float) {\em *this} is
					smaller or equal than the internal value (float) of {\em color_unit} colorUnit.

					@param   color_unit the colorUnit to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is smaller or equal than {\em color_unit}, {\tt false} otherwise
			*/
			bool operator <= (const ColorUnit& color_unit) const;
			
			/** Greater test.
					Test if {\em *this} is greater than {\em color_unit} colorUnit.
					Greater means that the internal value (float) {\em *this} is
					greater than the internal value (float) of {\em color_unit} colorUnit.

					@param   color_unit the colorUnit to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is greater than {\em color_unit}, {\tt false} otherwise
			*/
			bool operator > (const ColorUnit& color_unit) const;

			/** Greater or equal test.
					Test if {\em *this} is greater or equal than {\em color_unit} colorUnit.
					Greater or equal means that the internal value (float) {\em *this} is
					greater or equal than the internal value (float) of {\em color_unit} colorUnit.

					@param   color_unit the colorUnit to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is greater or equal than {\em color_unit}, {\tt false} otherwise
			*/ 
			bool operator >= (const ColorUnit& color_unit) const;
			//@}
			
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dump the current value of {\em *this} colorUnit to 
					the output ostream {\em s} with dumping depth {\em depth}.
					The value will be outputed as an integer of range 0-255.

					@param   s output stream where to output the value of {\em *this} colorUnit
					@param   depth the dumping depth (depth is irrelevant here)
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream input and value restorage.
					Read persistent colorUnit data from the input stream {\em s} and restore the value of {\em *this}.
					\\
					{\bf Note:} Not yet implemented.
					
					@param  s input stream from where to restore the value of {\em *this} colorUnit
					@exception   NotImplemented - always
			*/
			virtual void read(std::istream& s);

			/** Persistent stream output and value storage.
					Write persistent colorUnit data to the output stream {\em s} and store the value of {\em *this}.
					\\
					{\bf Note:} Not yet implemented.
					
					@param  s output stream to where the value of {\em *this} colorUnit will be stored
					@exception   NotImplemented - always
			*/
			virtual void write(std::ostream& s) const;

			/** Friendly stream input.
					Read colorUnit data from the input stream {\em s} and restore the value of {\em *this}.
					The value will be needed in integer format of range 0-255.
					This method is provided for convienience.

					@param  s input stream from where to restore the value of {\em *this} colorUnit
					@param  color_unit the colorUnit to which the value will be restored
			*/
			friend std::istream& operator >> (std::istream& s, ColorUnit& color_unit);

			/** Friendly stream output.
					Write colorUnit data to the output stream {\em s} and store the value of {\em *this}.
					The value will be stored in integer format of range 0-255.
					This method is provided for convienience.

					@param  s output stream to where the value of {\em *this} colorUnit will be stored
					@param  color_unit the colorUnit from which the value will be stored
			*/
			friend std::ostream& operator << (std::ostream& s, const ColorUnit& color_unit);

			//@}

			private:
		
			float hexToFloat_(const char* value);
			
			float value_;
		
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorUnit.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_DATATYPE_COLORUNIT_H
