// $Id: colorUnitHue.h,v 1.8 2001/07/08 11:10:23 aubertin Exp $

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
				{\bf Framework:} BALL/VIEW/DATATYPE\\
				{\bf Definition:} \URL{BALL/VIEW/DATATYPE/colorUnitHue.h}\\ \\
				An instance of ColorUnitHue represents a value for the hue
				component of the class \Ref{ColorHSV}.
				The class handles the many possible variations a hue value can be set
				(e.g. integer, float, string values can be given as parameter). The integer values
				are of range {\tt 0-360}, the float values are of range 0-1.0 and the strings
				are given in hex format {\tt "000"-"168"} for the values {\tt 0-360}.
				ColorUnitHue provides certain conversion functions as well that will convert
				its value to the needed type.
				This class is used by the class \Ref{ColorHSV}.
				@memo    ColorUnitHue class (BALL VIEW datatype framework)
				@author  $Author: aubertin $
				@version $Revision: 1.8 $
				@date    $Date: 2001/07/08 11:10:23 $
		*/
		class ColorUnitHue
		{
			public:
			
			/**	@name	Constructors
			*/
			//@{

			/** Default Constructor.
					Constructs new colorUnitHue. Its value will be set to zero.
					@return      ColorUnitHue new constructed colorUnitHue
			*/
			ColorUnitHue()
				throw();
		
			/** Copy constructor.
					Constructs new colorUnitHue by copying the colorUnitHue {\em color_h_unit}.
					@param       color_h_unit the colorUnitHue to be copied
					@return      ColorUnitHue new constructed colorUnitHue copied from {\em color_h_unit}
			*/
			ColorUnitHue(const ColorUnitHue& colcor_h_unit)
				throw();

			/** Constructor with Parameter char*.
					Constructs new colorUnitHue by initialising its value with the given pointer to
					a string representing a value in hex format ({\tt 00 - 168h}).
					@param       value the pointer to a string ({\tt 00 - 168h})
					@exception   InvalidRange if {\tt string length > 3}
					@exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
					@return      ColorUnitHue new constructed colorUnitHue
			*/
			ColorUnitHue(const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Constructor with Parameter String.
					Constructs new colorUnitHue by initialising its value with the given 
					string representing a value in hex format ({\tt 00 - 168h}).
					@param       value a string ({\tt 00 - 168h})
					@exception   InvalidRange if {\tt string length > 3}
					@exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
					@return      ColorUnitHue new constructed colorUnitHue
			*/
			ColorUnitHue(const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Constructor with Parameter short.
					Constructs new colorUnitHue by initialising its value with the given 
					short representing a value in integer format ({\tt 0 - 360}).
					@param       value a short value ({\tt 0 - 360})
					@exception   InvalidRange if {\tt value < 0 || value > 360}
					@return      ColorUnit new constructed colorUnit
			*/
			ColorUnitHue(const short value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter unsigned short.
					Constructs new colorUnitHue by initialising its value with the given 
					unsigned short representing a value in integer format ({\tt 0 - 360}).
					@param       value a unsigned short value ({\tt 0 - 360})
					@exception   InvalidRange if {\tt value > 360}
					@return      ColorUnitHue new constructed colorUnitHue
			*/
			ColorUnitHue(const unsigned short value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter int.
					Constructs new colorUnitHue by initialising its value with the given 
					int representing a value in integer format ({\tt 0 - 360}).
					@param       value an int value ({\tt 0 - 360})
					@exception   InvalidRange if {\tt value < 0 || value > 360}
					@return      ColorUnitHue new constructed colorUnitHue
			*/
			ColorUnitHue(const int value)
				throw(Exception::InvalidRange);
		
			/** Constructor with Parameter unsigned int.
					Constructs new colorUnitHUe by initialising its value with the given 
					unsigned int representing a value in integer format ({\tt 0 - 360}).
					@param       value an unsigned int value ({\tt 0 - 360})
					@exception   InvalidRange if {\tt value > 360}
					@return      ColorUnitHue new constructed colorUnitHue
			*/
			ColorUnitHue(const unsigned int value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter long.
					Constructs new colorUnitHue by initialising its value with the given 
					long representing a value in integer format ({\tt 0 - 360}).
					@param       value an long value ({\tt 0 - 360})
					@exception   InvalidRange if {\tt value < 0 || value > 360}
					@return      ColorUnitHue new constructed colorUnitHue
			*/
			ColorUnitHue(const long value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter unsigned long.
					Constructs new colorUnitHue by initialising its value with the given 
					unsigned long representing a value in integer format ({\tt 0 - 360}).
					@param       value an unsigned long value ({\tt 0 - 360})
					@exception   InvalidRange if {\tt value > 360}
					@return      ColorUnitHue new constructed colorUnitHue
			*/
			ColorUnitHue(const unsigned long value)
				throw(Exception::InvalidRange);
		
			/** Constructor with Parameter float.
					Constructs new colorUnitHue by initialising its value with the given 
					float representing a value of range ({\tt 0 - 1.0}).
					@param       value a float value ({\tt 0 - 1.0})
					@exception   InvalidRange if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue new constructed colorUnitHue
			*/
			ColorUnitHue(const float value)
				throw(Exception::InvalidRange);
		
			/** Constructor with Parameter double.
					Constructs new colorUnitHue by initialising its value with the given 
					double representing a value of range ({\tt 0 - 1.0}).
					@param       value a double value ({\tt 0 - 1.0})
					@exception   InvalidRange if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue new constructed colorUnitHue
			*/
			ColorUnitHue(const double value)
				throw(Exception::InvalidRange);

			/** Constructor with Parameter long double.
					Constructs new colorUnitHue by initialising its value with the given 
					long double representing a value of range ({\tt 0 - 1.0}).
					@param       value a long double value ({\tt 0 - 1.0})
					@exception   InvalidRange if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue new constructed colorUnitHue
			*/
			ColorUnitHue(const long double value)
				throw(Exception::InvalidRange);

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} colorUnitHue.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~ColorUnitHue()
				throw();

			/** Explicit default initialization.
					Sets the value of {\em *this} colorUnitHue to the default value.
					The value of {\em *this} colorUnitHue is:
					\begin{itemize}
						\item value is zero (0)
					\end{itemize}
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}
				
			/**	@name	Converters
			*/
			//@{
			/** Conversion to string.
					This operator casts an instance of colorUnitHue to the string type.
					@return      string with hex format {\tt "00"-"168"}
			*/
			operator String () const
				throw();

			/** Conversion to short.
					This operator casts an instance of colorUnitHue to the short type.
					@return      short with a value of range {\tt 0 - 360}
			*/
			operator short () const
				throw();
			
			/** Conversion to unsigned short.
					This operator casts an instance of colorUnitHue to the unsigned short type.
					@return      unsigned short with a value of range {\tt 0 - 360}
			*/
			operator unsigned short () const
				throw();
			
			/** Conversion to int.
					This operator casts an instance of colorUnitHue to the int type.
					@return      int with a value of range {\tt 0 - 360}
			*/
			operator int () const
				throw();

			/** Conversion to unsigned int.
					This operator casts an instance of colorUnitHue to the unsigned int type.

					@return      unsigned int with a value of range {\tt 0 - 360}
			*/
			operator unsigned int () const
				throw();

			/** Conversion to long.
					This operator casts an instance of colorUnitHue to the long type.
					@return      long with a value of range {\tt 0 - 360}
			*/
			operator long () const
				throw();

			/** Conversion to unsigned long.
					This operator casts an instance of colorUnitHue to the unsigned long type.
					@return      unsigned long with a value of range {\tt 0 - 360}
			*/
			operator unsigned long () const
				throw();

			/** Conversion to float.
					This operator casts an instance of colorUnitHue to the float type.
					@return      float with a value of range {\tt 0 - 1.0}
			*/
			operator float () const
				throw();

			/** Conversion to double.
					This operator casts an instance of colorUnitHue to the double type.
					@return      double with a value of range {\tt 0 - 1.0}
			*/
			operator double () const
				throw();
			
			/** Conversion to long double.
					This operator casts an instance of colorUnitHue to the long double type.
					@return      long double with a value of range {\tt 0 - 1.0}
			*/
			operator long double () const
				throw();

			//@}
			
			/**	@name	Assignment methods
			*/
			//@{
			
			/** Assignment.
					Assigns the colorUnitHue {\em color_unit_hue} to {\em *this} colorUnitHue.
					The value of {\em *this} is initialized to the value of the colorUnitHue {\em color_unit_hue}.\\
					@param       color_unit_hue the colorUnitHue to be copied 
			*/
			void set(const ColorUnitHue& color_unit_hue)
				throw();

			/** Assignment operator.
					Assigns the colorUnitHue {\em color_unit_hue} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					The value of {\em *this} is initialized to the value of the colorUnitHue {\em color_unit_hue}.\\
					@param       color_unit_hue the colorUnitHue to be copied
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const ColorUnitHue& color_unit_hue)
				throw();
		
			/** Copying.
					Copies {\em *this} colorUnitHue to the colorUnitHue {\em color_unit_hue}.
					Calls \Ref{set}.
					The value of the colorUnitHue {\em color_unit_hue} is initialized to the value of {\em *this} colorUnitHue.\\
					@param       color_unit_hue the colorUnitHue to be assigned to
					@see         set
			*/
			void get(ColorUnitHue& color_unit_hue) const
				throw();

			/** Swapping of colorUnitHues.
					Swaps the values of {\em *this} colorUnitHue with the colorUnitHue {\em color_unit_hue}.
					@param       color_unit_hue the colorUnitHue being swapped with {\em *this} colorUnitHue 
					@see         ColorUnitHue
			*/
			void swap(ColorUnitHue& color_unit_hue)
				throw();

			/** Changes the value of the colorUnitHue.
					Changes the value of {\em *this} colorUnitHue to the value represented by 
					a pointer {\em value} to a string.
					@param       value the new value of {\em *this} colorUnitHue (a string in the format {\tt "00"-"168"})
					@exception   InvalidRange if {\tt string length > 3}
					@exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
					@see         get
			*/
			void set(const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Assignment operator with char*.
					Assigns the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					@param       value the pointer to a string containing a value in hex format ({\tt "00"-"168"})
					@exception   InvalidRange if {\tt string length > 3}
					@exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);
		
			/** Inspection of the colorUnitHue's value.
					Accesses the value of {\em *this} colorUnitHue by using a char pointer.
					Memory must be allocated for this parameter. The memory must be at least
					the size of 4 bytes.
					@param   value a char* receiving the value of {\em *this} colorUnitHue in hex format ({\tt "00"-"168"})
					@see     set
			*/
			void get(char* value) const
				throw();

			/** Changes the value of the colorUnitHue.
					Changes the value of {\em *this} colorUnitHue to the value represented by 
					a string {\em value}.
					@param       value a string (in the hex format {\tt "00"-"168"})
					@exception   InvalidRange if {\tt string length > 3}
					@exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
					@see         get
			*/
			void set(const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Assignment operator with string.
					Assigns the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					@param       value a string containing a value in hex format ({\tt "00"-"168"})
					@exception   InvalidRange if {\tt string length > 3}
					@exception   NotInHexFormat if string contains characters g-z or G-Z or the hex number exceeds 168h
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);
		
			/** Inspection of the colorUnitHue's value.
					Accesses the value of {\em *this} colorUnitHue by using a string.
					@param   value a string receiving the value of {\em *this} colorUnitHue in hex format ({\tt "00"-"168"})
					@see     set
			*/
			void get(String& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					Changes the value of {\em *this} colorUnitHue to the value represented by 
					a short {\em value}.
					@param       value a short of range {\tt 0-360}
					@exception   InvalidRange if {\tt value < 0 || value > 360}
					@see         get
			*/
			void set(const short value)
				throw(Exception::InvalidRange);

			/** Assignment operator with short.
					Assigns the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					@param       value a short containing a value of range {\tt 0-360}
					@exception   InvalidRange if {\tt value < 0 || value > 360}
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const short value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					Accesses the value of {\em *this} colorUnitHue by using a short.
					@param   value a short receiving the value of {\em *this} colorUnitHue (range: {\tt 0-360})
					@see     set
			*/
			void get(short& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					Changes the value of {\em *this} colorUnitHue to the value represented by 
					an unsigned short {\em value}.
					@param       value an unsigned short of range {\tt 0-360}
					@exception   InvalidRange if {\tt value > 360}
					@see         get
			*/
			void set(const unsigned short value)
				throw(Exception::InvalidRange);

			/** Assignment operator with unsigned short.
					Assigns the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					@param       value an unsigned short containing a value of range {\tt 0-360}
					@exception   InvalidRange if {\tt value > 360}
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const unsigned short value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					Accesses the value of {\em *this} colorUnitHue by using an unsigned short.
					@param   value an unsigned short receiving the value of {\em *this} colorUnitHue (range: {\tt 0-360})
					@see     set
			*/
			void get(unsigned short& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					Changes the value of {\em *this} colorUnitHue to the value represented by 
					an int {\em value}.
					@param       value an int of range {\tt 0-360}
					@exception   InvalidRange if {\tt value < 0 || value > 360}
					@see         get
			*/
			void set(const int value)
				throw(Exception::InvalidRange);

			/** Assignment operator with int.
					Assigns the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					@param       value an int containing a value of range {\tt 0-360}
					@exception   InvalidRange if {\tt value < 0 || value > 360}
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const int value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					Accesses the value of {\em *this} colorUnitHue by using an int.
					@param   value an int receiving the value of {\em *this} colorUnitHue (range: {\tt 0-360})
					@see     set
			*/
			void get(int& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					Changes the value of {\em *this} colorUnitHue to the value represented by 
					an unsigned int {\em value}.
					@param       value an unsigned int of range {\tt 0-360}
					@exception   InvalidRange if {\tt value > 360}
					@see         get
			*/
			void set(const unsigned int value)
				throw(Exception::InvalidRange);

			/** Assignment operator with unsigned int.
					Assigns the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					@param       value an unsigned int containing a value of range {\tt 0-360}
					@exception   InvalidRange if {\tt value > 360}
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const unsigned int value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					Accesses the value of {\em *this} colorUnitHue by using an unsigned int.
					@param   value an unsigned int receiving the value of {\em *this} colorUnitHue (range: {\tt 0-360})
					@see     set
			*/
			void get(unsigned int& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					Changes the value of {\em *this} colorUnitHue to the value represented by 
					a long {\em value}.
					@param       value a long of range {\tt 0-360}
					@exception   InvalidRange if {\tt value < 0 || value > 360}
					@see         get
			*/
			void set(const long value)
				throw(Exception::InvalidRange);

			/** Assignment operator with long.
					Assigns the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					@param       value a long containing a value of range {\tt 0-360}
					@exception   InvalidRange if {\tt value < 0 || value > 360}
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const long value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					Accesses the value of {\em *this} colorUnitHue by using a long.
					@param   value a long receiving the value of {\em *this} colorUnitHue (range: {\tt 0-360})
					@see     set
			*/
			void get(long& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					Changes the value of {\em *this} colorUnitHue to the value represented by 
					an unsigned long {\em value}.
					@param       value an unsigned long of range {\tt 0-360}
					@exception   InvalidRange if {\tt value > 360}
					@see         get
			*/
			void set(const unsigned long value)
				throw(Exception::InvalidRange);

			/** Assignment operator with unsigned long.
					Assigns the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					@param       value an unsigned long containing a value of range {\tt 0-360}
					@exception   InvalidRange if {\tt value > 360}
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const unsigned long value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					Accesses the value of {\em *this} colorUnitHue by using an unsigned long.
					@param   value an unsigned long receiving the value of {\em *this} colorUnitHue (range: {\tt 0-360})
					@see     set
			*/
			void get(unsigned long& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					Changes the value of {\em *this} colorUnitHue to the value represented by 
					a float {\em value}.
					@param       value a float of range 0-1.0
					@exception   InvalidRange if {\tt value < 0.0 || value > 1.0}
					@see         get
			*/
			void set(const float value)
				throw(Exception::InvalidRange);

			/** Assignment operator with float.
					Assigns the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					@param       value a float containing a value of range 0-1.0
					@exception   InvalidRange if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const float init)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					Accesses the value of {\em *this} colorUnitHue by using a float.
					@param   value a float receiving the value of {\em *this} colorUnitHue (range: 0-1.0)
					@see     set
			*/
			void get(float& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					Changes the value of {\em *this} colorUnitHue to the value represented by 
					a double {\em value}.
					@param       value a double of range 0-1.0
					@exception   InvalidRange if {\tt value < 0.0 || value > 1.0}
					@see         get
			*/
			void set(const double value)
				throw(Exception::InvalidRange);

			/** Assignment operator with double.
					Assigns the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					@param       value a double containing a value of range 0-1.0
					@exception   InvalidRange if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const double value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					Accesses the value of {\em *this} colorUnitHue by using a double.
					@param   value a double receiving the value of {\em *this} colorUnitHue (range: 0-1.0)
					@see     set
			*/
			void get(double& value) const
				throw();

			/** Changes the value of the colorUnitHue.
					Changes the value of {\em *this} colorUnitHue to the value represented by 
					a long double {\em value}.
					@param       value a long double of range 0-1.0
					@exception   InvalidRange if {\tt value < 0.0 || value > 1.0}
					@see         get
			*/
			void set(const long double value)
				throw(Exception::InvalidRange);

			/** Assignment operator with long double.
					Assigns the contents of {\em value} to {\em *this} colorUnitHue.
					Calls \Ref{set}.
					@param       value a long double containing a value of range 0-1.0
					@exception   InvalidRange if {\tt value < 0.0 || value > 1.0}
					@return      ColorUnitHue& constant reference of {\em *this} colorUnitHue
					@see         set
			*/
			const ColorUnitHue& operator = (const long double value)
				throw(Exception::InvalidRange);

			/** Inspection of the colorUnitHue's value.
					Accesses the value of {\em *this} colorUnitHue by using a long double.
					@param   value a long double receiving the value of {\em *this} colorUnitHue (range: 0-1.0)
					@see     set
			*/
			void get(long double& value) const
				throw();
			
			//@}

			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
					Tests if {\em color_unit_hue} is equal to {\em *this} colorUnitHue.
					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool	{\tt true} if the value of {\em *this} is equal to {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator == (const ColorUnitHue& color_unit_hue) const
				throw();

			/** Unequality test.
					Tests if {\em color_unit_hue} is not equal to {\em *this} colorUnitHue.
					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool	{\tt true} if the value of {\em *this} is not equal to {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator != (const ColorUnitHue& color_unit_hue) const
				throw();
			
			/** Smaller test.
					Tests if {\em *this} is smaller than {\em color_unit_hue} colorUnitHue.
					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool	{\tt true} if the value of {\em *this} is smaller than {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator <	(const ColorUnitHue& color_unit_hue) const
				throw();
			
			/** Smaller or equal test.
					Tests if {\em *this} is smaller or equal than {\em color_unit_hue} colorUnitHue.
					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool	{\tt true} if the value of {\em *this} is smaller or equal than {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator <= (const ColorUnitHue& color_unit_hue) const
				throw();
			
			/** Greater test.
					Tests if {\em *this} is greater than {\em color_unit_hue} colorUnitHue.
					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool	{\tt true} if the value of {\em *this} is greater than {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator > (const ColorUnitHue& color_unit_hue) const
				throw();

			/** Greater or equal test.
					Tests if {\em *this} is greater or equal than {\em color_unit_hue} colorUnitHue.
					@param   color_unit_hue the colorUnitHue to be compared with
					@return  bool	{\tt true} if the value of {\em *this} is greater or equal than {\em color_unit_hue}, {\tt false} otherwise
			*/
			bool operator >= (const ColorUnitHue& color_unit_hue) const
				throw();
			//@}
			
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dumps the current value of {\em *this} colorUnitHue to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the value of {\em *this} colorUnitHue
					@param   depth the dumping depth (depth is irrelevant here)
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/	
			//@{

			/** Persistent stream input and value restorage.
					Reads persistent colorUnitHue data from the input stream {\em s} and restores the value of {\em *this}.\\
					{\bf Note:} Not yet implemented.
					@param  s input stream from where to restore the value of {\em *this} colorUnitHue
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and value storage.
					Writes persistent colorUnitHue data to the output stream {\em s} and stores the value of {\em *this}.\\
					{\bf Note:} Not yet implemented.
					@param  s output stream to where the value of {\em *this} colorUnitHue will be stored
			*/
			virtual void write(std::ostream& s) const
				throw();

			/** Friend stream input.
					Reads colorUnitHue data from the input stream {\em s} and restore the value of {\em *this}.
					The value will be needed in integer format of range {\tt 0-360}.
					This method is provided for convienience.
					@param  s input stream from where to restore the value of {\em *this} colorUnitHue
					@param  color_unit_hue the colorUnitHue to which the value will be restored
			*/
			friend std::istream& operator >> (std::istream& s, ColorUnitHue& color_unit_hue);

			/** Friend stream output.
					Writes colorUnitHue data to the output stream {\em s} and store the value of {\em *this}.
					The value will be stored in integer format of range {\tt 0-360}.
					This method is provided for convienience.
					@param  s output stream to where the value of {\em *this} colorUnitHue will be stored
					@param  color_unit_hue the colorUnitHue from which the value will be stored
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
