// $Id: colorHSV.h,v 1.1 2001/02/04 15:58:18 hekl Exp $

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

		// forward declarations 
		class ColorRGBA;

		/** ColorHSV class.

				{\bf Framework:} BALL/VIEW/DATATYPE\\
				{\bf Definition:} \URL{BALL/VIEW/DATATYPE/colorHSV.h}
				\\
				
				An instance of ColorHSV represents a color consisting of the components
				hue, saturation and value. The components staturation and value are 
				\Ref{ColorUnit}'s. The component hue is of class type \Ref{ColorUnitHue}.
				See description of these classes for further informations concerning type
				conversions.

				@memo    ColorHSV class (BALL VIEW datatype framework)
				@author  $Author: hekl $
				@version $Revision: 1.1 $
				@date    $Date: 2001/02/04 15:58:18 $
		*/
		class ColorHSV
		{
			/** @name Class friends

					\begin{itemize}
						\item class ColorRGBA
					\end{itemize}
			*/

			friend class ColorRGBA;

			public:

			/**	@name	Constructors
			*/
			//@{

			/** Default Constructor.
					Construct new colorHSV. Initialized with color black (hue=0, saturation=0, value=0).

					@return      ColorHSV - new constructed colorHSV
			*/
			ColorHSV();

			/** Copy constructor.
					Construct new colorHSV by copying the colorHSV {\em color}.

					@param       color the colorHSV to be copied
					@return      ColorHSV - new constructed colorHSV copied from {\em color}
			*/
			ColorHSV(const ColorHSV& color);

			/** Constructor with ColorRGBA.
					Construct new colorHSV by converting the colorRGBA {\em color}.

					@param       color the colorRGBA to be converted. See \Ref{ColorRGBA}
					@return      ColorHSV - new constructed colorHSV converted from {\em color}
			*/
			ColorHSV(const ColorRGBA& color);

			/** Constructor with Parameter char*
					Construct new colorHSV by initialising its value with the given pointer to
					a string representing a color value in hex format. The length of the string can
					either be 6 or 7 characters depending whether the hue component is 2 or 3 bytes
					long. See \Ref{ColorUnitHue} and \Ref{ColorUnit} for information concerning
					to string format.

					@param       value the pointer to a string representing a color
					@exception   InvalidRange - if {\tt string length != 6 && string length != 7}
					@return      ColorHSV - new constructed colorHSV
					@see         ColorUnit::ColorUnit
					@see         ColorUnitHue::ColorUnitHue
			*/
			ColorHSV(const char* value);

			/** Constructor with Parameter string
					Construct new colorHSV by initialising its value with  
					a string representing a color value in hex format. The length of the string can
					either be 6 or 7 characters depending whether the hue component is 2 or 3 bytes
					long. See \Ref{ColorUnitHue} and \Ref{ColorUnit} for information concerning
					to string format.

					@param       value a string representing a color
					@exception   InvalidRange - if {\tt string length != 6 && string length != 7}
					@return      ColorHSV - new constructed colorHSV
					@see         ColorUnit::ColorUnit
					@see         ColorUnitHue::ColorUnitHue
			*/
			ColorHSV(const String& value);

			/** Detailed component initializing constructor.
					Construct new colorHSV using the given component parameters.

					@param   hue the hue component. See \Ref{ColorUnitHue}
					@param   saturation the staturation component. See \Ref{ColorUnit}
					@param   value the value component. See \Ref{ColorUnit}
					@return      ColorHSV - new constructed colorHSV
					@see         ColorUnit::ColorUnit
					@see         ColorUnitHue::ColorUnitHue
			*/
			ColorHSV
				(const ColorUnitHue& hue,
				 const ColorUnit& saturation,
				 const ColorUnit& value);

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} colorHSV.

					Calls \Ref{ColorHSV::destroy}.
					@see         ColorHSV::destroy
			*/
			virtual ~ColorHSV();

			/** Explicit default initialization.
					Set the color of {\em *this} colorHSV to the default color (black).
					The value of {\em *this} colorHSV is:
					\begin{itemize}
						\item hue component is zero (0)
						\item staturation component is zero (0)
						\item value component is zero (0)
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
					This operator casts an instance of colorHSV to the string type.
					
					@return      a string with hex format. See constructor for information concerning the string format
					@see         ColorHSV::ColorHSV
			*/
			operator String () const;
			//@}

			/**	@name	Assignment	
			*/
			//@{

			/** Assignment.
					Assign the colorHSV {\em color} to {\em *this} colorHSV.
					The color of {\em *this} is initialized to the color of the colorHSV {\em color}.\\

					@param       color the colorHSV to be copied 
			*/
			void set(const ColorHSV& color);

			/** Assignment operator.
					Assign the colorHSV {\em color} to {\em *this} colorHSV.
					Calls \Ref{ColorHSV::set}.
					The color of {\em *this} is initialized to the color of the colorHSV {\em color}.\\

					@param       color the colorHSV to be copied
					@return      ColorHSV& - {\em *this} colorHSV
					@see         ColorHSV::set
			*/
			ColorHSV& operator = (const ColorHSV& color);

			/** Copying.
					Copy {\em *this} colorHSV to the colorHSV {\em color}.
					Calls \Ref{ColorHSV::set}.
					The color of the colorHSV {\em color} is initialized to the color of {\em *this} colorHSV.\\

					@param       color the colorHSV to be assigned to
					@see         ColorHSV::set
			*/
			void get(ColorHSV& color) const;

			/** Assignment with ColorRGBA.
					Assign the colorRGBA {\em color} to {\em *this} colorHSV.
					The color of {\em *this} is initialized to the color of the colorRGBA {\em color}.\\

					@param       color the colorRGBA to be copied 
			*/
			void set(const ColorRGBA& color);

			/** Assignment operator with ColorRGBA.
					Assign the colorRGBA {\em color} to {\em *this} colorHSV.
					Calls \Ref{ColorHSV::set}.
					The color of {\em *this} is initialized to the color of the colorRGBA {\em color}.\\

					@param       color the colorRGBA to be copied
					@return      ColorHSV& - {\em *this} colorHSV
					@see         ColorHSV::set
			*/
			ColorHSV& operator = (const ColorRGBA& color);

			/** Copying to ColorRGBA.
					Copy {\em *this} colorHSV to the colorRGBA {\em color}.
					Calls \Ref{ColorHSV::set}.
					The color of the colorRGBA {\em color} is initialized to the color of {\em *this} colorHSV.\\

					@param       color the colorRGBA to be assigned to
					@see         ColorHSV::set
			*/
			void get(ColorRGBA& color) const;

			/** Assignment with char*.
					Assign the color represented by the string {\em value} to {\em *this} colorHSV.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.

					@param       value the color represented by a string 
					@exception   InvalidRange - if {\tt string length != 6 && string length != 7}
					@see         ColorHSV::ColorHSV
			*/
			void set(const char* value);

			/** Assignment operator with char*.
					Assign the color represented by the string {\em value} to {\em *this} colorHSV.
					Calls \Ref{ColorHSV::set}.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.

					@param       value the color represented by a string
					@exception   InvalidRange - if {\tt string length != 6 && string length != 7}
					@return      ColorHSV& - {\em *this} colorHSV
					@see         ColorHSV::set
					@see         ColorHSV::ColorHSV
			*/
			ColorHSV& operator = (const char* value);

			/** Copying to char*.
					Copy the color of {\em *this} colorHSV to the string represented by the pointer {\em value}.
					Calls \Ref{ColorHSV::set}.
					The string {\em value} is initialized to the color of {\em *this} colorHSV.
					At least 8 bytes must be allocated for the string. The format of the string is
					the same as the one given to the constructor.

					@param       value the pointer to string (at least 8 bytes long)
					@see         ColorHSV::set
					@see         ColorHSV::ColorHSV
			*/
			void get(char* value) const;

			/** Assignment with string.
					Assign the color represented by the string {\em value} to {\em *this} colorHSV.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.

					@param       value the color represented by a string 
					@exception   InvalidRange - if {\tt string length != 6 && string length != 7}
					@see         ColorHSV::ColorHSV
			*/
			void set(const String& value);

			/** Assignment operator with string.
					Assign the color represented by the string {\em value} to {\em *this} colorHSV.
					Calls \Ref{ColorHSV::set}.
					The color of {\em *this} is initialized to the color represented by the string {\em value}.
					See constructor for format description.

					@param       value the color represented by a string
					@exception   InvalidRange - if {\tt string length != 6 && string length != 7}
					@return      ColorHSV& - {\em *this} colorHSV
					@see         ColorHSV::set
					@see         ColorHSV::ColorHSV
			*/
			ColorHSV& operator = (const String& value);

			/** Copying to string.
					Copy the color of {\em *this} colorHSV to the string represented by the pointer {\em value}.
					Calls \Ref{ColorHSV::set}.
					The string {\em value} is initialized to the color of {\em *this} colorHSV.
					The format of the string is	the same as the one given to the constructor.

					@param       value the string receiving the color in hex format
					@see         ColorHSV::set
					@see         ColorHSV::ColorHSV
			*/
			void get(String& value) const;

			/** Swapping of colorHSVs.
					Swap the color of {\em *this} colorHSV with the colorHSV {\em color}.

					@param       color the colorHSV being swapped with {\em *this} colorHSV
					@see         ColorHSV::ColorHSV
			*/
			void swap(ColorHSV& color);

			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the value of the hue component.
					Change the value of of the hue component of {\em *this} colorHSV to the value
					represented by the parameter {\em hue}. See \Ref{ColorUnitHue} for further
					information concerning the format.

					@param       hue the new hue component of {\em *this} colorHSV
					@see         ColorHSV::hue
					@see         ColorUnitHue::ColorUnitHue
			*/
			void setHue(const ColorUnitHue& hue);

			/** Mutable inspection of the hue component.
					Access the mutual reference of the hue component of {\em *this} colorHSV.
					See \Ref{ColorUnitHue} for further information concerning type conversion.
					
					@return      ColorUnitHue& - mutable reference to the hue component of {\em *this} colorHSV
					@see         ColorHSV::setHue
					@see         ColorUnitHue::ColorUnitHue
			*/
			ColorUnitHue& hue();

			/** Non-mutable inspection of the hue component.
					Access the constant reference of the hue component of {\em *this} colorHSV.
					See \Ref{ColorUnitHue} for further information concerning type conversion.
					
					@return      ColorUnitHue& - constant reference to the hue component of {\em *this} colorHSV
					@see         ColorHSV::setHue
					@see         ColorUnitHue::ColorUnitHue
			*/
			const ColorUnitHue& hue() const;

			/** Change the value of the saturation component.
					Change the value of of the saturation component of {\em *this} colorHSV to the value
					represented by the parameter {\em saturation}. See \Ref{ColorUnit} for further
					information concerning the format.

					@param       saturation the new saturation component of {\em *this} colorHSV
					@see         ColorHSV::saturation
					@see         ColorUnit::ColorUnit
			*/
			void setSaturation(const ColorUnit& saturation);

			/** Mutable inspection of the saturation component.
					Access the mutual reference of the saturation component of {\em *this} colorHSV.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - mutable reference to the saturation component of {\em *this} colorHSV
					@see         ColorHSV::setSaturation
					@see         ColorUnit::ColorUnit
			*/
			ColorUnit& saturation();

			/** Non-mutable inspection of the saturation component.
					Access the constant reference of the saturation component of {\em *this} colorHSV.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - constant reference to the saturation component of {\em *this} colorHSV
					@see         ColorHSV::setSaturation
					@see         ColorUnit::ColorUnit
			*/
			const ColorUnit& saturation() const;

			/** Change the value of the value component.
					Change the value of of the value component of {\em *this} colorHSV to the value
					represented by the parameter {\em value}. See \Ref{ColorUnit} for further
					information concerning the format.

					@param       value the new value component of {\em *this} colorHSV
					@see         ColorHSV::value
					@see         ColorUnit::ColorUnit
			*/
			void setValue(const ColorUnit& value);

			/** Mutable inspection of the value component.
					Access the mutual reference of the value component of {\em *this} colorHSV.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - mutable reference to the value component of {\em *this} colorHSV
					@see         ColorHSV::setValue
					@see         ColorUnit::ColorUnit
			*/
			ColorUnit& value();

			/** Non-mutable inspection of the value component.
					Access the constant reference of the value component of {\em *this} colorHSV.
					See \Ref{ColorUnit} for further information concerning type conversion.
					
					@return      ColorUnit& - constant reference to the value component of {\em *this} colorHSV
					@see         ColorHSV::setValue
					@see         ColorUnit::ColorUnit
			*/
			const ColorUnit& value() const;

			/** Change the values of the hue, saturation and value component.
					Change the values of of the hue, saturation and value component of {\em *this}
					colorHSV to the values represented by the parameters. See \Ref{ColorUnit}
					and \Ref{ColorUnitHue} for further information concerning type conversions.

					@param       hue the new hue component of {\em *this} colorHSV
					@param       saturation the new saturation component of {\em *this} colorHSV
					@param       value the new value component of {\em *this} colorHSV
					@see         ColorHSV::get
					@see         ColorUnit::ColorUnit
					@see         ColorUnitHue::ColorUnitHue
			*/
			void set
				(const ColorUnitHue& hue,
				 const ColorUnit& saturation,
				 const ColorUnit& value);

			/** Inspection of the colorHSV's components.
					Access all the components of {\em *this} colorHSV by using \Ref{ColorUnit}
					and \Ref{ColorUnitHue}.

					@param      hue the colorUnitHue receiving the hue component of {\em *this} colorHSV
					@param      saturation the colorUnit receiving the saturation component of {\em *this} colorHSV
					@param      value the colorUnit receiving the value component of {\em *this} colorHSV
					@see        ColorHSV::set
					@see        ColorUnit::ColorUnit
					@see        ColorUnitHue::ColorUnitHue
			*/
			void get
				(ColorUnitHue& hue,
				 ColorUnit& saturation,
				 ColorUnit& value) const;

			//@}
			
			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
					Test if {\em color} is equal to {\em *this} colorHSV.
					Equality means that all components are equal. See \Ref{ColorUnit} and
					\Ref{ColorUnitHue} for further information concerning equality tests.

					@param   color the colorHSV to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is equal to {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
					@see     ColorUnitHue::ColorUnitHue
			*/
			bool operator == (const ColorHSV& color) const;

			/** Unequality test.
					Test if {\em color} is not equal to {\em *this} colorHSV.
					Unequality means that all components are unequal. See \Ref{ColorUnit} and
					\Ref{ColorUnitHue} for further information concerning unequality tests.

					@param   color the colorHSV to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is not equal to {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
					@see     ColorUnitHue::ColorUnitHue
			*/
			bool operator != (const ColorHSV& color) const;

			/** Smaller test.
					Test if {\em *this} is smaller than {\em color} colorHSV.
					Smaller means that all components of {\em *this} are smaller than all the
					components of {\em color} colorHSV. See \Ref{ColorUnit} and
					\Ref{ColorUnitHue} for further information concerning unequality tests.

					@param   color the colorHSV to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is smaller than {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
					@see     ColorUnitHue::ColorUnitHue
			*/
			bool operator < (const ColorHSV& color) const;

			/** Smaller or equal test.
					Test if {\em *this} is smaller or equal than {\em color} colorHSV.
					Smaller or equal means that all components of {\em *this} are smaller or
					equal than all the components of {\em color} colorHSV. See \Ref{ColorUnit} and
					\Ref{ColorUnitHue} for further information concerning unequality tests.

					@param   color the colorHSV to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is smaller or equal than {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
					@see     ColorUnitHue::ColorUnitHue
			*/
			bool operator <= (const ColorHSV& color) const;

			/** Greater test.
					Test if {\em *this} is greater than {\em color} colorHSV.
					Greater means that all components of {\em *this} are greater than all the
					components of {\em color} colorHSV. See \Ref{ColorUnit} and
					\Ref{ColorUnitHue} for further information concerning greater tests.

					@param   color the colorHSV to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is greater than {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
					@see     ColorUnitHue::ColorUnitHue
			*/
			bool operator > (const ColorHSV& color) const;

			/** Greater or equal test.
					Test if {\em *this} is greater or equal than {\em color} colorHSV.
					Greater or equal means that all components of {\em *this} are greater or
					equal than all the components of {\em color} colorHSV. See \Ref{ColorUnit} and
					\Ref{ColorUnitHue} for further information concerning greater or equal tests.

					@param   color the colorHSV to be compared with
					@return  bool -	{\tt true} if the color of {\em *this} is greater or equal than {\em color}, {\tt false} otherwise
					@see     ColorUnit::ColorUnit
					@see     ColorUnitHue::ColorUnitHue
			*/
			bool operator >= (const ColorHSV& color) const;
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
	
			/** Internal value dump.
					Dump the current color of {\em *this} colorHSV to 
					the output ostream {\em s} with dumping depth {\em depth}.

					@param   s output stream where to output the color of {\em *this} colorHSV
					@param   depth the dumping depth
					@see     ColorUnit::dump
					@see     ColorUnitHue::dump
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;

			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream input and color restorage.
					Read persistent colorHSV data from the input stream {\em s} and restore the color of {\em *this}.
					\\
					{\bf Note:} Not yet implemented.
					
					@param  s input stream from where to restore the color of {\em *this} colorHSV
					@exception   NotImplemented - always
			*/
			virtual void read(std::istream& s);

			/** Persistent stream output and color storage.
					Write persistent colorHSV data to the output stream {\em s} and store the color of {\em *this}.
					\\
					{\bf Note:} Not yet implemented.
					
					@param  s output stream to where the color of {\em *this} colorHSV will be stored
					@exception   NotImplemented - always
			*/
			virtual void write(std::ostream& s) const;

			/** Friendly stream input.
					Read colorHSV data from the input stream {\em s} and restore the color of {\em *this}.
					The hue, saturation and value component are read as integers from the input stream.
					The method expects an vector as written as the one by the output stream operator.
					This method is provided for convienience.

					@param  s input stream from where to restore the color of {\em *this} colorHSV
					@param  color the colorHSV to which the color will be restored
			*/
			friend std::istream& operator >> (std::istream& s, ColorHSV& color);

			/** Friendly stream output.
					Write colorHSV data to the output stream {\em s} and store the color of {\em *this}.
					The color will be stored as an vector in integer format. The output will look
					like '(0-360, 0-255, 0-255)'.
					This method is provided for convienience.

					@param  s output stream to where the color of {\em *this} colorHSV will be stored
					@param  color the colorHSV from which the color will be stored
			*/
			friend std::ostream& operator << (std::ostream& s, const ColorHSV& color);

			//@}

			private:

			void calculateHSV_
				(const ColorUnit red,
				 const ColorUnit green,
				 const ColorUnit blue);

			void calculateRGB_
				(ColorUnit& red,
				 ColorUnit& green,
				 ColorUnit& blue) const;

			void stringToHSV_
				(const String& value);

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
