// $Id: colorUnitHue.h,v 1.6 2001/02/11 13:04:38 hekl Exp $

#ifndef BALL_VIEW_DATATYPE_COLORUNITHUE_H
#define BALL_VIEW_DATATYPE_COLORUNITHUE_H

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

		/** ColorUnitHue class.

				{\bf Framework:} BALL/VIEW/DATATYPE\\
				{\bf Definition:} \URL{BALL/VIEW/DATATYPE/colorUnitHue.h}
				\\
				
				An instance of ColorUnitHue represents a value for the hue
				component of the class \Ref{ColorHSV}.
				The class handles the many possible variations a hue value can be set
				(e.g. integer, float, string values can be given as parameter). The integer values
				are of range 0-360, the float values are of range 0-1.0 and the strings
				are given in hex format "000"-"168" for the values 0-360.
				ColorUnitHue provides certain conversion functions as well that will convert
				its value to the needed type.
				This class is used by the class \Ref{ColorHSV}.

				@memo    ColorUnitHue class (BALL VIEW datatype framework)
				@author  $Author: hekl $
				@version $Revision: 1.6 $
				@date    $Date: 2001/02/11 13:04:38 $
		*/
		class ColorUnitHue
		{
			public:
			
			/**	@name	Constructors
			*/
			//@{

			/** Default Constructor.
					Construct new colorUnitHue. Its value will be set to zero (0).

					@return      ColorUnitHue - new constructed colorUnitHue
			*/
			ColorUnitHue();
		
			/** Copy constructor.
					Construct new colorUnitHue by copying the colorUnitHue {\em color_h_unit}.

					@param       color_h_unit the colorUnitHue to be copied
					@return      ColorUnitHue - new constructed colorUnitHue copied from {\em color_h_unit}
			*/
			ColorUnitHue(const ColorUnitHue& colcor_h_unit);

			/** Constructor with Parameter char*.
					Construct new colorUnitHue by initialising its value with the given pointer to
					a string representing a value in hex format (00 - 168h).

					@param       value the pointer to a string (00 - 168h)
					@exception   InvalidRange - if {\tt string length > 3}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z or the hex number exceeds 168h
					@return      ColorUnitHue - new constructed colorUnitHue
			*/
			ColorUnitHue(const char* value);

			/** Constructor with Parameter String.
					Construct new colorUnitHue by initialising its value with the given 
					string representing a value in hex format (00 - 168h).

					@param       value a string (00 - 168h)
					@exception   InvalidRange - if {\tt string length > 3}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z or the hex number exceeds 168h
					@return      ColorUnitHue - new constructed colorUnitHue
			*/
			ColorUnitHue(const String& value);

			/** Constructor with Parameter short
					Construct new colorUnitHue by initialising its value with the given 
					short representing a value in integer format (0 - 360).

					@param       value a short value (0 - 360)
					@exception   InvalidRange - if {\tt value < 0 || value > 360}
					@return      ColorUnit - new constructed colorUnit
			*/
			ColorUnitHue(const short value);

			/** Constructor with Parameter unsigned short.
					Construct new colorUnitHue by initialising its value with the given 
					unsigned short representing a value in integer format (0 - 360).

					@param       value a unsigned short value (0 - 360)
					@exception   InvalidRange - if {\tt value > 360}
					@return      ColorUnitHue - new constructed colorUnitHue
			*/
			ColorUnitHue(const unsigned short value);

			/** Constructor with Parameter int.
					Construct new colorUnitHue by initialising its value with the given 
					int representing a value in integer format (0 - 360).

					@param       value an int value (0 - 360)
					@exception   InvalidRange - if {\tt value < 0 || value > 360}
					@return      ColorUnitHue - new constructed colorUnitHue
			*/
			ColorUnitHue(const int value);
		
			/** Constructor with Parameter unsigned int.
					Construct new colorUnitHUe by initialising its value with the given 
					unsigned int representing a value in integer format (0 - 360).

					@param       value an unsigned int value (0 - 360)
					@exception   InvalidRange - if {\tt value > 360}
					@return      ColorUnitHue - new constructed colorUnitHue
			*/
			ColorUnitHue(const unsigned int value);

			/** Constructor with Parameter long.
					Construct new colorUnitHue by initialising its value with the given 
					long representing a value in integer format (0 - 360).

					@param       value an long value (0 - 360)
					@exception   InvalidRange - if {\tt value < 0 || value > 360}
					@return      ColorUnitHue - new constructed colorUnitHue
			*/
			ColorUnitHue(const long value);

			/** Constructor with Parameter unsigned long.
					Construct new colorUnitHue by initialising its value with the given 
					unsigned long representing a value in integer format (0 - 360).

					@param       value an unsigned long value (0 - 360)
					@exception   InvalidRange - if {\tt value > 360}
					@return      ColorUnitHue - new constructed colorUnitHue
			*/
			ColorUnitHue(const unsigned long value);
		
			/** Constructor with Parameter float.
					Construct new colorUnitHue by initialising its value with the given 
					float representing a value of range (0 - 1.0).

					@param       value a float value (0 - 1.0)
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue - new constructed colorUnitHue
			*/
			ColorUnitHue(const float value);
		
			/** Constructor with Parameter double.
					Construct new colorUnitHue by initialising its value with the given 
					double representing a value of range (0 - 1.0).

					@param       value a double value (0 - 1.0)
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue - new constructed colorUnitHue
			*/
			ColorUnitHue(const double value);

			/** Constructor with Parameter long double.
					Construct new colorUnitHue by initialising its value with the given 
					long double representing a value of range (0 - 1.0).

					@param       value a long double value (0 - 1.0)
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue - new constructed colorUnitHue
			*/
			ColorUnitHue(const long double value);

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} colorUnitHue.

					Calls \Ref{ColorUnitHue::destroy}.
					@see         ColorUnitHue::destroy
			*/
			virtual ~ColorUnitHue();

			/** Explicit default initialization.
					Set the value of {\em *this} colorUnitHue to the default value.
					The value of {\em *this} colorUnitHue is:
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
					This operator casts an instance of colorUnitHue to the string type.
					
					@return      a string with hex format "00"-"168"
			*/
			operator String () const;

			/** Conversion to short.
					This operator casts an instance of colorUnitHue to the short type.

					@return      a short with a value of range 0 - 360
			*/
			operator short () const;
			
			/** Conversion to unsigned short.
					This operator casts an instance of colorUnitHue to the unsigned short type.

					@return      a unsigned short with a value of range 0 - 360
			*/
			operator unsigned short () const;
			
			/** Conversion to int.
					This operator casts an instance of colorUnitHue to the int type.

					@return      a int with a value of range 0 - 360
			*/
			operator int () const;

			/** Conversion to unsigned int.
					This operator casts an instance of colorUnitHue to the unsigned int type.

					@return      a unsigned int with a value of range 0 - 360
			*/
			operator unsigned int () const;

			/** Conversion to long.
					This operator casts an instance of colorUnitHue to the long type.

					@return      a long with a value of range 0 - 360
			*/
			operator long () const;

			/** Conversion to unsigned long.
					This operator casts an instance of colorUnitHue to the unsigned long type.

					@return      a unsigned long with a value of range 0 - 360
			*/
			operator unsigned long () const;

			/** Conversion to float.
					This operator casts an instance of colorUnitHue to the float type.

					@return      a float with a value of range 0 - 1.0
			*/
			operator float () const;

			/** Conversion to double.
					This operator casts an instance of colorUnitHue to the double type.

					@return      a double with a value of range 0 - 1.0
			*/
			operator double () const;
			
			/** Conversion to long double.
					This operator casts an instance of colorUnitHue to the long double type.

					@return      a long double with a value of range 0 - 1.0
			*/
			operator long double () const;

			//@}
			
			/**	@name	Assignment methods
			*/
			//@{
			
			/** Assignment.
					Assign the colorUnitHue {\em color_unit_hue} to {\em *this} colorUnitHue.
					The value of {\em *this} is initialized to the value of the colorUnitHue {\em color_unit_hue}.\\

					@param       color_unit_hue the colorUnitHue to be copied 
			*/
			void set(const ColorUnitHue& color_unit_hue);

			/** Assignment operator.
					Assign the colorUnitHue {\em color_unit_hue} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.
					The value of {\em *this} is initialized to the value of the colorUnitHue {\em color_unit_hue}.\\

					@param       color_unit_hue the colorUnitHue to be copied
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const ColorUnitHue& color_unit_hue);
		
			/** Copying.
					Copy {\em *this} colorUnitHue to the colorUnitHue {\em color_unit_hue}.
					Calls \Ref{ColorUnitHue::set}.
					The value of the colorUnitHue {\em color_unit_hue} is initialized to the value of {\em *this} colorUnitHue.\\

					@param       color_unit_hue the colorUnitHue to be assigned to
					@see         ColorUnitHue::set
			*/
			void get(ColorUnitHue& color_unit_hue) const;

			/** Swapping of colorUnitHues.
					Swap the values of {\em *this} colorUnitHue with the colorUnitHue {\em color_unit_hue}.
					@param       color_unit_hue the colorUnitHue being swapped with {\em *this} colorUnitHue 
					@see         ColorUnitHue::ColorUnitHue
			*/
			void swap(ColorUnitHue& color_unit_hue);

			/** Change the value of the colorUnitHue.
					Change the value of {\em *this} colorUnitHue to the value represented by 
					a pointer {\em value} to a string.

					@param       value the new value of {\em *this} colorUnitHue (a string in the format "00"-"168")
					@exception   InvalidRange - if {\tt string length > 3}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z or the hex number exceeds 168h
					@see         ColorUnitHue::getValue
			*/
			void set(const char* value);

			/** Assignment operator with char*
					Assign the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.

					@param       value the pointer to a string containing a value in hex format ("00"-"168")
					@exception   InvalidRange - if {\tt string length > 3}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z or the hex number exceeds 168h
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const char* value);
		
			/** Inspection of the colorUnitHue's value.
					Access the value of {\em *this} colorUnitHue by using a char pointer.
					Memory must be allocated for this parameter. The memory must be at least
					the size of 4 bytes.
					
					@param   value a char* receiving the value of {\em *this} colorUnitHue in hex format ("00"-"168")
					@see     ColorUnitHue::setValue
			*/
			void get(char* value) const;

			/** Change the value of the colorUnitHue.
					Change the value of {\em *this} colorUnitHue to the value represented by 
					a string {\em value}.

					@param       value a string (in the hex format "00"-"168")
					@exception   InvalidRange - if {\tt string length > 3}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z or the hex number exceeds 168h
					@see         ColorUnitHue::getValue
			*/
			void set(const String& value);

			/** Assignment operator with string
					Assign the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.

					@param       value a string containing a value in hex format ("00"-"168")
					@exception   InvalidRange - if {\tt string length > 3}
					@exception   NotInHexFormat - if string contains characters g-z or G-Z or the hex number exceeds 168h
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const String& value);
		
			/** Inspection of the colorUnitHue's value.
					Access the value of {\em *this} colorUnitHue by using a string.
					
					@param   value a string receiving the value of {\em *this} colorUnitHue in hex format ("00"-"168")
					@see     ColorUnitHue::setValue
			*/
			void get(String& value) const;

			/** Change the value of the colorUnitHue.
					Change the value of {\em *this} colorUnitHue to the value represented by 
					a short {\em value}.

					@param       value a short of range 0-360
					@exception   InvalidRange - if {\tt value < 0 || value > 360}
					@see         ColorUnitHue::getValue
			*/
			void set(const short value);

			/** Assignment operator with short
					Assign the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.

					@param       value a short containing a value of range 0-360
					@exception   InvalidRange - if {\tt value < 0 || value > 360}
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const short value);

			/** Inspection of the colorUnitHue's value.
					Access the value of {\em *this} colorUnitHue by using a short.
					
					@param   value a short receiving the value of {\em *this} colorUnitHue (range: 0-360)
					@see     ColorUnitHue::setValue
			*/
			void get(short& value) const;

			/** Change the value of the colorUnitHue.
					Change the value of {\em *this} colorUnitHue to the value represented by 
					an unsigned short {\em value}.

					@param       value an unsigned short of range 0-360
					@exception   InvalidRange - if {\tt value > 360}
					@see         ColorUnitHue::getValue
			*/
			void set(const unsigned short value);

			/** Assignment operator with unsigned short
					Assign the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.

					@param       value an unsigned short containing a value of range 0-360
					@exception   InvalidRange - if {\tt value > 360}
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const unsigned short value);

			/** Inspection of the colorUnitHue's value.
					Access the value of {\em *this} colorUnitHue by using an unsigned short.
					
					@param   value an unsigned short receiving the value of {\em *this} colorUnitHue (range: 0-360)
					@see     ColorUnitHue::setValue
			*/
			void get(unsigned short& value) const;

			/** Change the value of the colorUnitHue.
					Change the value of {\em *this} colorUnitHue to the value represented by 
					an int {\em value}.

					@param       value an int of range 0-360
					@exception   InvalidRange - if {\tt value < 0 || value > 360}
					@see         ColorUnitHue::getValue
			*/
			void set(const int value);

			/** Assignment operator with int
					Assign the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.

					@param       value an int containing a value of range 0-360
					@exception   InvalidRange - if {\tt value < 0 || value > 360}
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const int value);

			/** Inspection of the colorUnitHue's value.
					Access the value of {\em *this} colorUnitHue by using an int.
					
					@param   value an int receiving the value of {\em *this} colorUnitHue (range: 0-360)
					@see     ColorUnitHue::setValue
			*/
			void get(int& value) const;

			/** Change the value of the colorUnitHue.
					Change the value of {\em *this} colorUnitHue to the value represented by 
					an unsigned int {\em value}.

					@param       value an unsigned int of range 0-360
					@exception   InvalidRange - if {\tt value > 360}
					@see         ColorUnitHue::getValue
			*/
			void set(const unsigned int value);

			/** Assignment operator with unsigned int
					Assign the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.

					@param       value an unsigned int containing a value of range 0-360
					@exception   InvalidRange - if {\tt value > 360}
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const unsigned int value);

			/** Inspection of the colorUnitHue's value.
					Access the value of {\em *this} colorUnitHue by using an unsigned int.
					
					@param   value an unsigned int receiving the value of {\em *this} colorUnitHue (range: 0-360)
					@see     ColorUnitHue::setValue
			*/
			void get(unsigned int& value) const;

			/** Change the value of the colorUnitHue.
					Change the value of {\em *this} colorUnitHue to the value represented by 
					a long {\em value}.

					@param       value a long of range 0-360
					@exception   InvalidRange - if {\tt value < 0 || value > 360}
					@see         ColorUnitHue::getValue
			*/
			void set(const long value);

			/** Assignment operator with long
					Assign the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.

					@param       value a long containing a value of range 0-360
					@exception   InvalidRange - if {\tt value < 0 || value > 360}
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const long value);

			/** Inspection of the colorUnitHue's value.
					Access the value of {\em *this} colorUnitHue by using a long.
					
					@param   value a long receiving the value of {\em *this} colorUnitHue (range: 0-360)
					@see     ColorUnitHue::setValue
			*/
			void get(long& value) const;

			/** Change the value of the colorUnitHue.
					Change the value of {\em *this} colorUnitHue to the value represented by 
					an unsigned long {\em value}.

					@param       value an unsigned long of range 0-360
					@exception   InvalidRange - if {\tt value > 360}
					@see         ColorUnitHue::getValue
			*/
			void set(const unsigned long value);

			/** Assignment operator with unsigned long
					Assign the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.

					@param       value an unsigned long containing a value of range 0-360
					@exception   InvalidRange - if {\tt value > 360}
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const unsigned long value);

			/** Inspection of the colorUnitHue's value.
					Access the value of {\em *this} colorUnitHue by using an unsigned long.
					
					@param   value an unsigned long receiving the value of {\em *this} colorUnitHue (range: 0-360)
					@see     ColorUnitHue::setValue
			*/
			void get(unsigned long& value) const;

			/** Change the value of the colorUnitHue.
					Change the value of {\em *this} colorUnitHue to the value represented by 
					a float {\em value}.

					@param       value a float of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@see         ColorUnitHue::getValue
			*/
			void set(const float value);

			/** Assignment operator with float
					Assign the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.

					@param       value a float containing a value of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const float init);

			/** Inspection of the colorUnitHue's value.
					Access the value of {\em *this} colorUnitHue by using a float.
					
					@param   value a float receiving the value of {\em *this} colorUnitHue (range: 0-1.0)
					@see     ColorUnitHue::setValue
			*/
			void get(float& value) const;

			/** Change the value of the colorUnitHue.
					Change the value of {\em *this} colorUnitHue to the value represented by 
					a double {\em value}.

					@param       value a double of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@see         ColorUnitHue::getValue
			*/
			void set(const double value);

			/** Assignment operator with double
					Assign the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.

					@param       value a double containing a value of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const double value);

			/** Inspection of the colorUnitHue's value.
					Access the value of {\em *this} colorUnitHue by using a double.
					
					@param   value a double receiving the value of {\em *this} colorUnitHue (range: 0-1.0)
					@see     ColorUnitHue::setValue
			*/
			void get(double& value) const;

			/** Change the value of the colorUnitHue.
					Change the value of {\em *this} colorUnitHue to the value represented by 
					a long double {\em value}.

					@param       value a long double of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@see         ColorUnitHue::getValue
			*/
			void set(const long double value);

			/** Assignment operator with long double
					Assign the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{ColorUnitHue::set}.

					@param       value a long double containing a value of range 0-1.0
					@exception   InvalidRange - if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue& - {\em *this} colorUnitHue
					@see         ColorUnitHue::set
			*/
			ColorUnitHue& operator = (const long double value);

			/** Inspection of the colorUnitHue's value.
					Access the value of {\em *this} colorUnitHue by using a long double.
					
					@param   value a long double receiving the value of {\em *this} colorUnitHue (range: 0-1.0)
					@see     ColorUnitHue::setValue
			*/
			void get(long double& value) const;
			
			//@}

			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
					Test if {\em color_unit_hue} is equal to {\em *this} colorUnitHue.

					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is equal to {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator == (const ColorUnitHue& color_unit_hue) const;

			/** Unequality test.
					Test if {\em color_unit_hue} is not equal to {\em *this} colorUnitHue.

					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is not equal to {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator != (const ColorUnitHue& color_unit_hue) const;
			
			/** Smaller test.
					Test if {\em *this} is smaller than {\em color_unit_hue} colorUnitHue.

					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is smaller than {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator <	(const ColorUnitHue& color_unit_hue) const;
			
			/** Smaller or equal test.
					Test if {\em *this} is smaller or equal than {\em color_unit_hue} colorUnitHue.

					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is smaller or equal than {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator <= (const ColorUnitHue& color_unit_hue) const;
			
			/** Greater test.
					Test if {\em *this} is greater than {\em color_unit_hue} colorUnitHue.

					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is greater than {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator > (const ColorUnitHue& color_unit_hue) const;

			/** Greater or equal test.
					Test if {\em *this} is greater or equal than {\em color_unit_hue} colorUnitHue.

					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool -	{\tt true} if the value of {\em *this} is greater or equal than {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator >= (const ColorUnitHue& color_unit_hue) const;
			//@}
			
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dump the current value of {\em *this} colorUnitHue to 
					the output ostream {\em s} with dumping depth {\em depth}.

					@param   s output stream where to output the value of {\em *this} colorUnitHue
					@param   depth the dumping depth (depth is irrelevant here)
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/	
			//@{

			/** Persistent stream input and value restorage.
					Read persistent colorUnitHue data from the input stream {\em s} and restore the value of {\em *this}.
					\\
					{\bf Note:} Not yet implemented.
					
					@param  s input stream from where to restore the value of {\em *this} colorUnitHue
					@exception   NotImplemented - always
			*/
			virtual void read(std::istream& s);

			/** Persistent stream output and value storage.
					Write persistent colorUnitHue data to the output stream {\em s} and store the value of {\em *this}.
					\\
					{\bf Note:} Not yet implemented.
					
					@param  s output stream to where the value of {\em *this} colorUnitHue will be stored
					@exception   NotImplemented - always
			*/
			virtual void write(std::ostream& s) const;

			/** Friend stream input.
					Read colorUnitHue data from the input stream {\em s} and restore the value of {\em *this}.
					The value will be needed in integer format of range 0-360.
					This method is provided for convienience.

					@param  s input stream from where to restore the value of {\em *this} colorUnitHue
					@param  color_unit_hue the colorUnitHue to which the value will be restored
			*/
			friend std::istream& operator >> (std::istream& s, ColorUnitHue& color_unit_hue);

			/** Friend stream output.
					Write colorUnitHue data to the output stream {\em s} and store the value of {\em *this}.
					The value will be stored in integer format of range 0-360.
					This method is provided for convienience.

					@param  s output stream to where the value of {\em *this} colorUnitHue will be stored
					@param  color_unit_hue the colorUnitHue from which the value will be stored
			*/
			friend std::ostream& operator << (std::ostream& s, const ColorUnitHue& color_unit_hue);
			//@}

			private:
			
			float hexToFloat_(const char* string);
			
			float value_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorUnitHue.iC>
#		endif

	} // namspace VIEW
	
} // namespace BALL

#endif // BALL_VIEW_DATATYPE_COLORUNITHUE_H
