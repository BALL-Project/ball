// $Id: colorRGBA.h,v 1.1 2001/02/04 15:58:18 hekl Exp $

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
#define BALL_VIEW_DATATYPE_COLORRGBA_H

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

		// forward declarations 
		class ColorHSV;

		/** ColorRGBA class.

				{\bf Framework:} BALL/VIEW/DATATYPE\\
				{\bf Definition:} \URL{BALL/VIEW/DATATYPE/colorRGBA.h}
				\\
				
				An instance of ColorRGBA represents a color consisting of the components
				red, green, blue and alpha. The components are \Ref{ColorUnit}'s. This class
				allows many types of input parameters. See description of this class for further
				informations.

				@memo    ColorRGBA class (BALL VIEW datatype framework)
				@author  $Author: hekl $
				@version $Revision: 1.1 $
				@date    $Date: 2001/02/04 15:58:18 $
		*/
		class ColorRGBA
		{
			/** @name Class friends

					\begin{itemize}
						\item class ColorHSV
					\end{itemize}
			*/

			friend class ColorHSV;

			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new colorRGBA. Initialized with color black (red=0, green=0, blue=0, alpha=1).

					@return      ColorRGBA - new constructed colorRGBA
			*/
			ColorRGBA();

			/** Copy constructor.
					Construct new colorRGBA by copying the colorRGBA {\em color}.

					@param       color the colorRGBA to be copied
					@return      ColorRGBA - new constructed colorRGBA copied from {\em color}
			*/
			ColorRGBA(const ColorRGBA& color);

			/** Constructor with ColorHSV.
					Construct new colorRGBA by converting the colorHSV {\em color}.

					@param       color the colorHSV to be converted. See \Ref{ColorHSV}
					@return      ColorRGBA - new constructed colorRGBA converted from {\em color}
			*/
			ColorRGBA(const ColorHSV& color);

			/** Constructor with Parameter char*
					Construct new colorRGBA by initialising its value with the given pointer to
					a string representing a color value in hex format. The length of the string can
					either be 6 or 8 characters depending whether the alpha component should 
					be considered. If the string is only 6 characters long the alpha component will
					be set to 1.0.					

					@param       value the pointer to a string representing a color
					@exception   InvalidRange - if {\tt string length != 6 && string length != 8}
					@return      ColorRGBA - new constructed colorRGBA
			*/
			ColorRGBA(const char* value);

			/** Constructor with Parameter string
					Same as constructor with parameter char*.

					@param       value a string representing a color
					@exception   InvalidRange - if {\tt string length != 6 && string length != 8}
					@return      ColorRGBA - new constructed colorRGBA
			*/
			ColorRGBA(const String& value);

			/** Detailed component initializing constructor.
					Construct new colorRGBA using the given component parameters.

					@param       red the red component. See \Ref{ColorUnit}
					@param       green the green component. See \Ref{ColorUnit}
					@param       blue the blue component. See \Ref{ColorUnit}
					@param       alpha the alpha component. See \Ref{ColorUnit}. Default value 1.0
					@return      ColorHSV - new constructed colorHSV
					@see         ColorUnit::ColorUnit
					@see         ColorUnitHue::ColorUnitHue
			*/
			ColorRGBA
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1);

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} colorRGBA.

					Calls \Ref{ColorRGBA::destroy}.
					@see         ColorRGBA::destroy
			*/
			virtual ~ColorRGBA();

			/** Explicit default initialization.
					Set the color of {\em *this} colorRGBA to the default color (black).
					The value of {\em *this} colorRGBA is:
					\begin{itemize}
						\item red component is zero (0)
						\item green component is zero (0)
						\item blue component is zero (0)
						\item alpha component is 1 (1.0)
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
					This operator casts an instance of colorRGBA to the string type.
					
					@return      a string with hex format "00000000"-"FFFFFFFF". The string has the same format as the one given to the constructor.
					@see         ColorRGBA::ColorRGBA(const String &value)
			*/
			operator String() const;
			//@}

			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the colorRGBA {\em color} to {\em *this} colorRGBA.
					The color of {\em *this} is initialized to the color of the colorRGBA {\em color}.\\

					@param       color the colorRGBA to be copied 
			*/
			void set(const ColorRGBA& color);

			/** Assignment operator.
					Assign the colorRGBA {\em color} to {\em *this} colorRGBA.
					Calls \Ref{ColorRGBA::set}.
					The color of {\em *this} is initialized to the color of the colorRGBA {\em color}.\\

					@param       color the colorRGBA to be copied
					@return      ColorRGBA& - {\em *this} colorRGBA
					@see         ColorRGBA::set
			*/
			ColorRGBA& operator = (const ColorRGBA& color);

			/** Copying.
					Copy {\em *this} colorRGBA to the colorRGBA {\em color}.
					Calls \Ref{ColorRGBA::set}.
					The color of the colorRGBA {\em color} is initialized to the color of {\em *this} colorRGBA.\\

					@param       color the colorRGBA to be assigned to
					@see         ColorRGBA::set
			*/
			void get(ColorRGBA& color) const;

			/** Assignment with ColorHSV.
					Assign the colorHSV {\em color} to {\em *this} colorRGBA.
					The color of {\em *this} is initialized to the color of the colorHSV {\em color}.\\

					@param       color the colorHSV to be copied 
			*/
			void set(const ColorHSV& color);

			/** Assignment operator with ColorHSV.
					Assign the colorHSV {\em color} to {\em *this} colorRGBA.
					Calls \Ref{ColorRGBA::set}.
					The color of {\em *this} is initialized to the color of the colorHSV {\em color}.\\

					@param       color the colorHSV to be copied
					@return      ColorRGBA& - {\em *this} colorRGBA
					@see         ColorRGBA::set
			*/
			ColorRGBA& operator = (const ColorHSV& color);

			/** Copying to ColorHSV.
					Copy {\em *this} colorRGBA to the colorHSV {\em color}.
					Calls \Ref{ColorRGBA::set}.
					The color of the colorHSV {\em color} is initialized to the color of {\em *this} colorRGBA.\\

					@param       color the colorHSV to be assigned to
					@see         ColorRGBA::set
			*/
			void get(ColorHSV& color) const;

			/** Assignment with char*.
					Assign the color represented by the string {\em value} to {\em *this} colorRGBA.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.

					@param       value the color represented by a string 
					@exception   InvalidRange - if {\tt string length != 6 && string length != 8}
					@see         ColorRGBA::ColorRGBA
			*/
			void set(const char* value);

			/** Assignment operator with char*.
					Assign the color represented by the string {\em value} to {\em *this} colorRGBA.
					Calls \Ref{ColorRGBA::set}.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.

					@param       value the color represented by a string
					@exception   InvalidRange - if {\tt string length != 6 && string length != 8}
					@return      ColorRGBA& - {\em *this} colorRGBA
					@see         ColorRGBA::set
					@see         ColorRGBA::ColorRGBA
			*/
			ColorRGBA& operator = (const char* value);

			/** Copying to char*.
					Copy the color of {\em *this} colorRGBA to the string represented by the pointer {\em value}.
					Calls \Ref{ColorRGBA::set}.
					The string {\em value} is initialized to the color of {\em *this} colorRGBA.
					At least 9 bytes must be allocated for the string. The format of the string is
					the same as the one given to the constructor.

					@param       value the pointer to string (at least 9 bytes long)
					@see         ColorRGBA::set
					@see         ColorRGBA::ColorRGBA
			*/
			void get(char* value) const;

			/** Assignment with string.
					Assign the color represented by the string {\em value} to {\em *this} colorRGBA.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.

					@param       value the color represented by a string 
					@exception   InvalidRange - if {\tt string length != 6 && string length != 8}
					@see         ColorRGBA::ColorRGBA
			*/
			void set(const String& value);

			/** Assignment operator with string.
					Assign the color represented by the string {\em value} to {\em *this} colorRGBA.
					Calls \Ref{ColorRGBA::set}.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.

					@param       value the color represented by a string
					@exception   InvalidRange - if {\tt string length != 6 && string length != 8}
					@return      ColorRGBA& - {\em *this} colorRGBA
					@see         ColorRGBA::set
					@see         ColorRGBA::ColorRGBA
			*/
			ColorRGBA& operator = (const String& value);

			/** Copying to string.
					Copy the color of {\em *this} colorRGBA to the string {\em value}.
					Calls \Ref{ColorRGBA::set}.
					The string {\em value} is initialized to the color of {\em *this} colorRGBA.
					The format of the string is	the same as the one given to the constructor.
					See constructor for format description.

					@param       value the string receiving the color in hex format
					@see         ColorRGBA::set
					@see         ColorRGBA::ColorRGBA
			*/
			void get(String& value) const;

			/** Swapping of colorRGBAs.
					Swap the color of {\em *this} colorRGBA with the colorRGBA {\em color}.

					@param       color the colorRGBA being swapped with {\em *this} colorRGBA 
					@see         ColorRGBA::ColorRGBA
			*/
			void swap(ColorRGBA& color);
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the value of the red component.
					Change the value of of the red component of {\em *this} colorRGBA to the value
					represented by the parameter {\em red}. See \Ref{ColorUnit} for further information.

					@param       red the new red component of {\em *this} colorRGBA
					@see         ColorRGBA::red
					@see         ColorUnit::ColorUnit
			*/
			void setRed(const ColorUnit& red);

			/** Mutable inspection of the red component.
					Access the mutual reference of the red component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - mutable reference to the red component of {\em *this} colorRGBA
					@see         ColorRGBA::setRed
					@see         ColorUnit::ColorUnit
			*/
			ColorUnit& red();

			/** Non-mutable inspection of the red component.
					Access the constant reference of the red component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - constant reference to the red component of {\em *this} colorRGBA
					@see         ColorRGBA::setRed
					@see         ColorUnit::ColorUnit
			*/
			const ColorUnit& red() const;

			/** Change the value of the green component.
					Change the value of of the green component of {\em *this} colorRGBA to the value
					represented by the parameter {\em green}. See \Ref{ColorUnit} for further information.

					@param       green the new green component of {\em *this} colorRGBA
					@see         ColorRGBA::green
					@see         ColorUnit::ColorUnit
			*/
			void setGreen(const ColorUnit& green);

			/** Mutable inspection of the green component.
					Access the mutual reference of the green component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - mutable reference to the green component of {\em *this} colorRGBA
					@see         ColorRGBA::setGreen
					@see         ColorUnit::ColorUnit
			*/
			ColorUnit& green();

			/** Non-mutable inspection of the green component.
					Access the constant reference of the green component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - constant reference to the green component of {\em *this} colorRGBA
					@see         ColorRGBA::setGreen
					@see         ColorUnit::ColorUnit
			*/
			const ColorUnit& green() const;

			/** Change the value of the blue component.
					Change the value of of the blue component of {\em *this} colorRGBA to the value
					represented by the parameter {\em blue}. See \Ref{ColorUnit} for further information.

					@param       blue the new blue component of {\em *this} colorRGBA
					@see         ColorRGBA::blue
					@see         ColorUnit::ColorUnit
			*/
			void setBlue(const ColorUnit& blue);

			/** Mutable inspection of the blue component.
					Access the mutual reference of the blue component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - mutable reference to the blue component of {\em *this} colorRGBA
					@see         ColorRGBA::setBlue
					@see         ColorUnit::ColorUnit
			*/
			ColorUnit& blue();

			/** Non-mutable inspection of the blue component.
					Access the constant reference of the blue component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - constant reference to the blue component of {\em *this} colorRGBA
					@see         ColorRGBA::setBlue
					@see         ColorUnit::ColorUnit
			*/
			const ColorUnit& blue() const;

			/** Change the value of the alpha component.
					Change the value of of the alpha component of {\em *this} colorRGBA to the value
					represented by the parameter {\em alpha}. See \Ref{ColorUnit} for further information.

					@param       alpha the new alpha component of {\em *this} colorRGBA
					@see         ColorRGBA::alpha
					@see         ColorUnit::ColorUnit
			*/
			void setAlpha(const ColorUnit& alpha);

			/** Mutable inspection of the alpha component.
					Access the mutual reference of the alpha component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - mutable reference to the alpha component of {\em *this} colorRGBA
					@see         ColorRGBA::setAlpha
					@see         ColorUnit::ColorUnit
			*/
			ColorUnit& alpha();

			/** Non-mutable inspection of the alpha component.
					Access the constant reference of the alpha component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - constant reference to the alpha component of {\em *this} colorRGBA
					@see         ColorRGBA::setAlpha
					@see         ColorUnit::ColorUnit
			*/
			const ColorUnit& alpha() const;

			/** Change the values of the red, green, blue and alpha component.
					Change the values of of the red, green, blue and alpha component of {\em *this}
					colorRGBA to the values represented by the parameters. See \Ref{ColorUnit} for
					further information concerning type conversions.

					@param       red the new red component of {\em *this} colorRGBA
					@param       green the new green component of {\em *this} colorRGBA
					@param       blue the new blue component of {\em *this} colorRGBA
					@param       alpha the new alpha component of {\em *this} colorRGBA. Default 1.0
					@see         ColorRGBA::get
					@see         ColorUnit::ColorUnit
			*/
			void set
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1);

			/** Inspection of the colorRGBA's components.
					Access all the components of {\em *this} colorRGBA by using \Ref{ColorUnit}'s.

					@param      red the colorUnit receiving the red component of {\em *this} colorRGBA
					@param      green the colorUnit receiving the green component of {\em *this} colorRGBA
					@param      blue the colorUnit receiving the blue component of {\em *this} colorRGBA
					@param      alpha the colorUnit receiving the alpha component of {\em *this} colorRGBA
					@see        ColorRGBA::set
					@see        ColorUnit::ColorUnit
			*/
			void get
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const;

			/** Inspection of the colorRGBA's components.
					Access only the red, green and blue components of {\em *this} colorRGBA by using \Ref{ColorUnit}'s.

					@param      red the colorUnit receiving the red component of {\em *this} colorRGBA
					@param      green the colorUnit receiving the green component of {\em *this} colorRGBA
					@param      blue the colorUnit receiving the blue component of {\em *this} colorRGBA

					@see        ColorRGBA::set
					@see        ColorUnit::ColorUnit
			*/
			void get
				(ColorUnit& red,
				 ColorUnit& green,
				 ColorUnit& blue) const;
			 
			//@}

			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
					Test if {\em color} is equal to {\em *this} colorRGBA.
					Equality means that all components are equal. See \Ref{ColorUnit} for further
					information concerning equality tests.

					@param   color the colorRGBA to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is equal to {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
			*/
			bool operator == (const ColorRGBA& color) const;

			/** Unequality test.
					Test if {\em color} is not equal to {\em *this} colorRGBA.
					Unequality means that all components are unequal. See \Ref{ColorUnit} for further
					information concerning unequality tests.

					@param   color the colorRGBA to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is not equal to {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
			*/
			bool operator !=  (const ColorRGBA& color) const;

			/** Smaller test.
					Test if {\em *this} is smaller than {\em color} colorRGBA.
					Smaller means that all components of {\em *this} are smaller than all the
					components of {\em color} colorRGBA. See \Ref{ColorUnit} for further
					information concerning smaller tests.

					@param   color the colorRGBA to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is smaller than {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
			*/
			bool operator < (const ColorRGBA& color) const;

			/** Smaller or equal test.
					Test if {\em *this} is smaller or equal than {\em color} colorRGBA.
					Smaller or equal means that all components of {\em *this} are smaller or equal than all the
					components of {\em color} colorRGBA. See \Ref{ColorUnit} for further
					information concerning smaller or equal tests.

					@param   color the colorRGBA to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is smaller or equal than {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
			*/
			bool operator <= (const ColorRGBA& color) const;

			/** Greater test.
					Test if {\em *this} is greater than {\em color} colorRGBA.
					Greater means that all components of {\em *this} are greater than all the
					components of {\em color} colorRGBA. See \Ref{ColorUnit} for further
					information concerning greater tests.

					@param   color the colorRGBA to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is greater than {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
			*/
			bool operator > (const ColorRGBA& color) const;

			/** Greater or equal test.
					Test if {\em *this} is greater or equal than {\em color} colorRGBA.
					Greater or equal means that all components of {\em *this} are greater or equal than all the
					components of {\em color} colorRGBA. See \Ref{ColorUnit} for further
					information concerning greater or equal tests.

					@param   color the colorRGBA to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is greater or equal than {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
			*/
			bool operator >= (const ColorRGBA& color) const;
			//@}
				
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dump the current color of {\em *this} colorRGBA to 
					the output ostream {\em s} with dumping depth {\em depth}.

					@param   s output stream where to output the color of {\em *this} colorRGBA
					@param   depth the dumping depth
					@see     ColorUnit::dump
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream input and color restorage.
					Read persistent colorRGBA data from the input stream {\em s} and restore the color of {\em *this}.
					\\
					{\bf Note:} Not yet implemented.
					
					@param  s input stream from where to restore the color of {\em *this} colorRGBA
					@exception   NotYetImplemented - always
			*/
			virtual void read(std::istream& s);

			/** Persistent stream output and color storage.
					Write persistent colorRGBA data to the output stream {\em s} and store the color of {\em *this}.
					\\
					{\bf Note:} Not yet implemented.
					
					@param  s output stream to where the color of {\em *this} colorRGBA will be stored
					@exception   NotYetImplemented - always
			*/
			virtual void write(std::ostream& s) const;

			/** Friendly stream input.
					Read colorRGBA data from the input stream {\em s} and restore the color of {\em *this}.
					The red, green, blue and alpha component are read as integers from the input stream.
					THe method expects an vector as written as the one by the output stream operator.
					This method is provided for convienience.

					@param  s input stream from where to restore the color of {\em *this} colorRGBA
					@param  color the colorRGBA to which the color will be restored
			*/
			friend std::istream& operator >> (std::istream& s, ColorRGBA& color);

			/** Friendly stream output.
					Write colorRGBA data to the output stream {\em s} and store the color of {\em *this}.
					The color will be stored as an vector in integer format. The output will look
					like '(0-255, 0-255, 0-255, 0-255)'.
					This method is provided for convienience.

					@param  s output stream to where the color of {\em *this} colorRGBA will be stored
					@param  color the colorRGBA from which the color will be stored
			*/
			friend std::ostream& operator << (std::ostream& s, const ColorRGBA& color);

			//@}

			private:

			ColorUnit red_;
			ColorUnit green_;
			ColorUnit blue_;
			ColorUnit alpha_;

			void stringToRGBA_(const String& s);
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorRGBA.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // <BALL/VIEW_DATATYPE_COLORRGBA_H
