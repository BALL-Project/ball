// $Id: colorRGBA.h,v 1.4 2001/07/11 14:08:47 aubertin Exp $

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
				{\bf Definition:} \URL{BALL/VIEW/DATATYPE/colorRGBA.h}\\ \\
				An instance of ColorRGBA represents a color consisting of the components
				red, green, blue and alpha. The components are \Ref{ColorUnit}'s. This class
				allows many types of input parameters. See description of this class for further
				informations.
				@memo    ColorRGBA class (BALL VIEW datatype framework)
				@author  $Author: aubertin $
				@version $Revision: 1.4 $
				@date    $Date: 2001/07/11 14:08:47 $
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
					Constructs new colorRGBA. Initialized with color black (red=0, green=0, blue=0, alpha=1).
					@return      ColorRGBA new constructed colorRGBA
			*/
			ColorRGBA()
				throw();

			/** Copy constructor.
					Constructs new colorRGBA by copying the colorRGBA {\em color}.
					@param       color the colorRGBA to be copied
					@return      ColorRGBA new constructed colorRGBA copied from {\em color}
			*/
			ColorRGBA(const ColorRGBA& color)
				throw();

			/** Constructor with ColorHSV.
					Constructs new colorRGBA by converting the colorHSV {\em color}.
					@param       color the colorHSV to be converted. See \Ref{ColorHSV}
					@return      ColorRGBA new constructed colorRGBA converted from {\em color}
					@see         ColorHSV
			*/
			ColorRGBA(const ColorHSV& color)
				throw();

			/** Constructor with Parameter char*.
					Constructs new colorRGBA by initialising its value with the given pointer to
					a string representing a color value in hex format. The length of the string can
					either be 6 or 8 characters depending whether the alpha component should 
					be considered. If the string is only 6 characters long the alpha component will
					be set to 1.0.					
					@param       value the pointer to a string representing a color
					@exception   InvalidRange if {\tt string length != 6 && string length != 8}
					@exception   NotInHexFormat if the string is not a valid hex format string
					@return      ColorRGBA new constructed colorRGBA
			*/
			ColorRGBA(const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Constructor with Parameter string.
					Same as constructor with parameter char*.
					@param       value a string representing a color
					@exception   InvalidRange if {\tt string length != 6 && string length != 8}
					@exception   NotInHexFormat if the string is not a valid hex format string
					@return      ColorRGBA new constructed colorRGBA
			*/
			ColorRGBA(const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Detailed component initializing constructor.
					Constructs new colorRGBA using the given component parameters.
					@param       red_value the red component. See \Ref{ColorUnit}
					@param       green_value the green component. See \Ref{ColorUnit}
					@param       blue_value the blue component. See \Ref{ColorUnit}
					@param       alpha_value the alpha component. See \Ref{ColorUnit}. Default value 1.0
					@return      ColorRGBA new constructed colorRGBA
					@see         ColorUnit
			*/
			ColorRGBA
				(const ColorUnit& red_value, const ColorUnit& green_value,
				 const ColorUnit& blue_value, const ColorUnit& alpha_value = (float)1)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} colorRGBA.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~ColorRGBA()
				throw();

			/** Explicit default initialization.
					Sets the color of {\em *this} colorRGBA to the default color (black).
					The value of {\em *this} colorRGBA is:
					\begin{itemize}
						\item red component is zero (0)
						\item green component is zero (0)
						\item blue component is zero (0)
						\item alpha component is 1 (1.0)
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
					This operator casts an instance of colorRGBA to the string type.
					@return      a string with hex format {\tt "00000000"-"FFFFFFFF"}. The string has the same format as the one given to the constructor.
					@see         ColorRGBA::ColorRGBA
			*/
			operator String() const
				throw();
			//@}

			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assigns the colorRGBA {\em color} to {\em *this} colorRGBA.
					The color of {\em *this} is initialized to the color of the colorRGBA {\em color}.\\
					@param       color the colorRGBA to be copied 
			*/
			void set(const ColorRGBA& color)
				throw();

			/** Assignment operator.
					Assigns the colorRGBA {\em color} to {\em *this} colorRGBA.
					Calls \Ref{set}.
					The color of {\em *this} is initialized to the color of the colorRGBA {\em color}.\\
					@param       color the colorRGBA to be copied
					@return      ColorRGBA& constant reference of {\em *this} colorRGBA
					@see         set
			*/
			const ColorRGBA& operator = (const ColorRGBA& color)
				throw();

			/** Copying.
					Copies {\em *this} colorRGBA to the colorRGBA {\em color}.
					Calls \Ref{set}.
					The color of the colorRGBA {\em color} is initialized to the color of {\em *this} colorRGBA.\\
					@param       color the colorRGBA to be assigned to
					@see         set
			*/
			void get(ColorRGBA& color) const
				throw();

			/** Assignment with ColorHSV.
					Assigns the colorHSV {\em color} to {\em *this} colorRGBA.
					The color of {\em *this} is initialized to the color of the colorHSV {\em color}.\\
					@param       color the colorHSV to be copied 
			*/
			void set(const ColorHSV& color)
				throw();

			/** Assignment operator with ColorHSV.
					Assigns the colorHSV {\em color} to {\em *this} colorRGBA.
					Calls \Ref{set}.
					The color of {\em *this} is initialized to the color of the colorHSV {\em color}.\\
					@param       color the colorHSV to be copied
					@return      ColorRGBA& constant reference of {\em *this} colorRGBA
					@see         set
					@see         ColorHSV
			*/
			const ColorRGBA& operator = (const ColorHSV& color)
				throw();

			/** Copying to ColorHSV.
					Copies {\em *this} colorRGBA to the colorHSV {\em color}.
					Calls \Ref{set}.
					The color of the colorHSV {\em color} is initialized to the color of {\em *this} colorRGBA.\\
					@param       color the colorHSV to be assigned to
					@see         set
					@see         ColorHSV
			*/
			void get(ColorHSV& color) const
				throw();

			/** Assignment with char*.
					Assigns the color represented by the string {\em value} to {\em *this} colorRGBA.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.
					@param       value the color represented by a string 
					@exception   InvalidRange if {\tt string length != 6 && string length != 8}
					@exception   NotInHexFormat if the string is not a valid hex format string
			*/
			void set(const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Assignment operator with char*.
					Assigns the color represented by the string {\em value} to {\em *this} colorRGBA.
					Calls \Ref{set}.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.
					@param       value the color represented by a string
					@exception   InvalidRange if {\tt string length != 6 && string length != 8}
					@exception   NotInHexFormat if the string is not a valid hex format string
					@return      ColorRGBA& {\em *this} colorRGBA
					@see         set
			*/
			const ColorRGBA& operator = (const char* value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Copying to char*.
					Copies the color of {\em *this} colorRGBA to the string represented by the pointer {\em value}.
					Calls \Ref{set}.
					The string {\em value} is initialized to the color of {\em *this} colorRGBA.
					At least 9 bytes must be allocated for the string. The format of the string is
					the same as the one given to the constructor.
					@param       value the pointer to string (at least 9 bytes long)
					@see         set
			*/
			void get(char* value) const
				throw();

			/** Assignment with string.
					Assigns the color represented by the string {\em value} to {\em *this} colorRGBA.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.
					@param       value the color represented by a string 
					@exception   InvalidRange if {\tt string length != 6 && string length != 8}
					@exception   NotInHexFormat if the string is not a valid hex format string
			*/
			void set(const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Assignment operator with string.
					Assigns the color represented by the string {\em value} to {\em *this} colorRGBA.
					Calls \Ref{set}.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.
					@param       value the color represented by a string
					@exception   InvalidRange if {\tt string length != 6 && string length != 8}
					@exception   NotInHexFormat if the string is not a valid hex format string
					@return      ColorRGBA& {\em *this} colorRGBA
					@see         set
			*/
			const ColorRGBA& operator = (const String& value)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);

			/** Copying to string.
					Copies the color of {\em *this} colorRGBA to the string {\em value}.
					Calls \Ref{set}.
					The string {\em value} is initialized to the color of {\em *this} colorRGBA.
					The format of the string is	the same as the one given to the constructor.
					See constructor for format description.
					@param       value the string receiving the color in hex format
					@see         set
			*/
			void get(String& value) const
				throw();

			/** Swapping of colorRGBAs.
					Swaps the color of {\em *this} colorRGBA with the colorRGBA {\em color}.
					@param       color the colorRGBA being swapped with {\em *this} colorRGBA 
			*/
			void swap(ColorRGBA& color)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Changes the value of the red component.
					Changes the value of of the red component of {\em *this} colorRGBA to the value
					represented by the parameter {\em red_value}. See \Ref{ColorUnit} for further information.
					@param       red_value the new red component of {\em *this} colorRGBA
					@see         getRed
					@see         ColorUnit
			*/
			void setRed(const ColorUnit& red_value)
				throw();

			/** Mutable inspection of the red component.
					Accesses the mutual reference of the red component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					@return      ColorUnit& mutable reference to the red component of {\em *this} colorRGBA
					@see         setRed
					@see         ColorUnit
			*/
			ColorUnit& getRed()
				throw();

			/** Non-mutable inspection of the red component.
					For further information see {\em ColorUnit& getRed()}.
			*/
			const ColorUnit& getRed() const
				throw();

			/** Changes the value of the green component.
					Changes the value of of the green component of {\em *this} colorRGBA to the value
					represented by the parameter {\em green_value}. See \Ref{ColorUnit} for further information.
					@param       green_value the new green component of {\em *this} colorRGBA
					@see         getGreen
					@see         ColorUnit
			*/
			void setGreen(const ColorUnit& green_value)
				throw();

			/** Mutable inspection of the green component.
					Accesses the mutual reference of the green component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					@return      ColorUnit& mutable reference to the green component of {\em *this} colorRGBA
					@see         setGreen
					@see         ColorUnit
			*/
			ColorUnit& getGreen()
				throw();

			/** Non-mutable inspection of the green component.
					For further information see {\em ColorUnit& getGreen()}.
			*/
			const ColorUnit& getGreen() const
				throw();

			/** Changes the value of the blue component.
					Changes the value of of the blue component of {\em *this} colorRGBA to the value
					represented by the parameter {\em blue_value}. See \Ref{ColorUnit} for further information.
					@param       blue_value the new blue component of {\em *this} colorRGBA
					@see         getBlue
					@see         ColorUnit
			*/
			void setBlue(const ColorUnit& blue_value)
				throw();

			/** Mutable inspection of the blue component.
					Accesses the mutual reference of the blue component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					@return      ColorUnit& mutable reference to the blue component of {\em *this} colorRGBA
					@see         setBlue
					@see         ColorUnit
			*/
			ColorUnit& getBlue()
				throw();

			/** Non-mutable inspection of the blue component.
    					For further information see {\em ColorUnit& getBlue()}
			*/
			const ColorUnit& getBlue() const
				throw();

			/** Changes the value of the alpha component.
					Changes the value of of the alpha component of {\em *this} colorRGBA to the value
					represented by the parameter {\em alpha_value}. See \Ref{ColorUnit} for further information.
					@param       alpha_value the new alpha component of {\em *this} colorRGBA
					@see         getAlpha
					@see         ColorUnit
			*/
			void setAlpha(const ColorUnit& alpha_value)
				throw();

			/** Mutable inspection of the alpha component.
					Accesses the mutual reference of the alpha component of {\em *this} colorRGBA.
					See \Ref{ColorUnit} for further information concerning type conversion.
					@return      ColorUnit& mutable reference to the alpha component of {\em *this} colorRGBA
					@see         setAlpha
					@see         ColorUnit
			*/
			ColorUnit& getAlpha()
				throw();

			/** Non-mutable inspection of the alpha component.
			                For further information see {\em ColorUnit& getAlpha()}.
			*/
			const ColorUnit& getAlpha() const
				throw();

			/** Changes the values of the red, green, blue and alpha component.
					Changes the values of of the red, green, blue and alpha component of {\em *this}
					colorRGBA to the values represented by the parameters. See \Ref{ColorUnit} for
					further information concerning type conversions.
					@param       red_value the new red component of {\em *this} colorRGBA
					@param       green_value the new green component of {\em *this} colorRGBA
					@param       blue_value the new blue component of {\em *this} colorRGBA
					@param       alpha_value the new alpha component of {\em *this} colorRGBA. Default 1.0
					@see         get
					@see         ColorUnit
			*/
			void set
				(const ColorUnit& red_value, const ColorUnit& green_value,
				 const ColorUnit& blue_value, const ColorUnit& alpha_value = (float)1)
				throw();

			/** Inspection of the colorRGBA's components.
					Accesses all the components of {\em *this} colorRGBA by using \Ref{ColorUnit}'s.
					@param      red_value the colorUnit receiving the red component of {\em *this} colorRGBA
					@param      green_value the colorUnit receiving the green component of {\em *this} colorRGBA
					@param      blue_value the colorUnit receiving the blue component of {\em *this} colorRGBA
					@param      alpha_value the colorUnit receiving the alpha component of {\em *this} colorRGBA
					@see        set
					@see        ColorUnit
			*/
			void get
				(ColorUnit& red_value, ColorUnit& green_value,
				 ColorUnit& blue_value, ColorUnit& alpha_value) const
				throw();

			/** Inspection of the colorRGBA's components.
					Accesses only the red, green and blue components of {\em *this} colorRGBA by using \Ref{ColorUnit}'s.
					@param      red_value the colorUnit receiving the red component of {\em *this} colorRGBA
					@param      green_value the colorUnit receiving the green component of {\em *this} colorRGBA
					@param      blue_value the colorUnit receiving the blue component of {\em *this} colorRGBA
					@see        set
					@see        ColorUnit
			*/
			void get
				(ColorUnit& red_value,
				 ColorUnit& green_value,
				 ColorUnit& blue_value) const
				throw();
			 
			//@}

			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
					Tests if {\em color} is equal to {\em *this} colorRGBA.
					Equality means that all components are equal. See \Ref{ColorUnit} for further
					information concerning equality tests.
					@param   color the colorRGBA to be compared with
					@return  bool {\tt true} if the color of {\em *this} is equal to {\em color}, {\tt false} otherwise
					@see     ColorUnit
			*/
			bool operator == (const ColorRGBA& color) const
				throw();

			/** Unequality test.
					Tests if {\em color} is not equal to {\em *this} colorRGBA.
					Unequality means that all components are unequal. See \Ref{ColorUnit} for further
					information concerning unequality tests.
					@param   color the colorRGBA to be compared with
					@return  bool {\tt true} if the color of {\em *this} is not equal to {\em color}, {\tt false} otherwise
					@see     ColorUnit
			*/
			bool operator !=  (const ColorRGBA& color) const
				throw();

			/** Smaller test.
					Tests if {\em *this} is smaller than {\em color} colorRGBA.
					Smaller means that all components of {\em *this} are smaller than all the
					components of {\em color} colorRGBA. See \Ref{ColorUnit} for further
					information concerning smaller tests.
					@param   color the colorRGBA to be compared with
					@return  bool {\tt true} if the color of {\em *this} is smaller than {\em color}, {\tt false} otherwise
					@see     ColorUnit
			*/
			bool operator < (const ColorRGBA& color) const
				throw();

			/** Smaller or equal test.
					Tests if {\em *this} is smaller or equal than {\em color} colorRGBA.
					Smaller or equal means that all components of {\em *this} are smaller or equal than all the
					components of {\em color} colorRGBA. See \Ref{ColorUnit} for further
					information concerning smaller or equal tests.
					@param   color the colorRGBA to be compared with
					@return  bool {\tt true} if the color of {\em *this} is smaller or equal than {\em color}, {\tt false} otherwise
					@see     ColorUnit
			*/
			bool operator <= (const ColorRGBA& color) const
				throw();

			/** Greater test.
					Tests if {\em *this} is greater than {\em color} colorRGBA.
					Greater means that all components of {\em *this} are greater than all the
					components of {\em color} colorRGBA. See \Ref{ColorUnit} for further
					information concerning greater tests.
					@param   color the colorRGBA to be compared with
					@return  bool {\tt true} if the color of {\em *this} is greater than {\em color}, {\tt false} otherwise
					@see     ColorUnit
			*/
			bool operator > (const ColorRGBA& color) const
				throw();

			/** Greater or equal test.
					Tests if {\em *this} is greater or equal than {\em color} colorRGBA.
					Greater or equal means that all components of {\em *this} are greater or equal than all the
					components of {\em color} colorRGBA. See \Ref{ColorUnit} for further
					information concerning greater or equal tests.
					@param   color the colorRGBA to be compared with
					@return  bool {\tt true} if the color of {\em *this} is greater or equal than {\em color}, {\tt false} otherwise
					@see     ColorUnit
			*/
			bool operator >= (const ColorRGBA& color) const
				throw();
			//@}
				
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dumps the current color of {\em *this} colorRGBA to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the color of {\em *this} colorRGBA
					@param   depth the dumping depth
					@see     ColorUnit::dump
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream input and color restorage.
					Reads persistent colorRGBA data from the input stream {\em s} and restores the color of {\em *this}.\\
					{\bf Note:} Not yet implemented.
					@param  s input stream from where to restore the color of {\em *this} colorRGBA
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and color storage.
					Writes persistent colorRGBA data to the output stream {\em s} and stores the color of {\em *this}.\\
					{\bf Note:} Not yet implemented.
					@param  s output stream to where the color of {\em *this} colorRGBA will be stored
			*/
			virtual void write(std::ostream& s) const
				throw();

			/** Friendly stream input.
					Reads colorRGBA data from the input stream {\em s} and restore the color of {\em *this}.
					The red, green, blue and alpha component are read as integers from the input stream.
					THe method expects an vector as written as the one by the output stream operator.
					This method is provided for convienience.
					@param  s input stream from where to restore the color of {\em *this} colorRGBA
					@param  color the colorRGBA to which the color will be restored
			*/
			friend std::istream& operator >> (std::istream& s, ColorRGBA& color);

			/** Friendly stream output.
					Writes colorRGBA data to the output stream {\em s} and store the color of {\em *this}.
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

			void stringToRGBA_(const String& s)
				throw(Exception::InvalidRange, ColorUnit::NotInHexFormat);
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorRGBA.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // <BALL/VIEW_DATATYPE_COLORRGBA_H
