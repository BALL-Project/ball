// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
#define BALL_VIEW_DATATYPE_COLORRGBA_H

#ifndef BALL_VIEW_DATATYPE_COLORUNIT_H
#	include <BALL/VIEW/DATATYPE/colorUnit.h>
#endif

#ifndef BALL_CONCEPT_TIMESTAMP_H
 #include <BALL/CONCEPT/timeStamp.h>
#endif

class QColor;
namespace BALL 
{
	namespace VIEW 
	{
		// forward declarations 
		class ColorHSV;

		/** ColorRGBA class.
				An instance of ColorRGBA represents a color consisting of the components
				red, green, blue and alpha. The components are  ColorUnit 's, which
 				allow many types of input parameters. See description of this class for further
				informations.  \par
		\ingroup ViewDatatypeColors		
		*/
		class BALL_VIEW_EXPORT ColorRGBA
		{
			/** @name Class friends
						- class ColorHSV
			*/
			friend class ColorHSV;

			public:

//			BALL_CREATE(ColorRGBA)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Initialized with color black (red=0, green=0, blue=0, alpha=1).
			*/
			ColorRGBA();

			/** Copy constructor.
			*/
			ColorRGBA(const ColorRGBA& color);

			/** Constructor with ColorHSV.
					\see         ColorHSV
			*/
			ColorRGBA(const ColorHSV& color);

			/** Constructor with Parameter char*.
					Constructs new colorRGBA by initialising its value with the given pointer to
					a string representing a color value in hex format. The length of the string can
					either be 6 or 8 characters depending whether the alpha component should 
					be considered. If the string is only 6 characters long the alpha component will
					be set to 1.0.					
					\param       value the pointer to a string representing a color
					\exception   InvalidRange if <tt> string length != 6 && string length != 8</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
					\return      ColorRGBA new constructed colorRGBA
			*/
			ColorRGBA(const char* value);

			/** Constructor with Parameter string.
					Same as constructor with parameter char*.
					\param       value a string representing a color
					\exception   InvalidRange if <tt> string length != 6 && string length != 8</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
					\return      ColorRGBA new constructed colorRGBA
			*/
			ColorRGBA(const String& value);

			/** Detailed component initializing constructor.
					\param       red_value the red component. 
					\param       green_value the green component.
					\param       blue_value the blue component.
					\param       alpha_value the alpha component. Default value 1.0
					\see         ColorUnit
			*/
			ColorRGBA
				(const ColorUnit& red_value, const ColorUnit& green_value,
				 const ColorUnit& blue_value, const ColorUnit& alpha_value = (float)1);

			/// Assignment from a QT QColor
			ColorRGBA(const QColor& color);

			/// Assignment from a QT QColor
			ColorRGBA(const QColor& color, const ColorUnit& alpha);

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			~ColorRGBA();

			/** Explicit default initialization.
					Sets the color of this colorRGBA to the default color (black).
					The value of this colorRGBA is:
						- red component is zero (0)
						- green component is zero (0)
						- blue component is zero (0)
						- alpha component is 1 (1.0)
			*/
			void clear();

			//@}
			/**	@name	Converters
			*/
			//@{

			/** Conversion to string.
					This operator casts an instance of colorRGBA to the string type.
					\return      a string with hex format <tt> "00000000"-"FFFFFFFF"</tt>. 
											 The string has the same format as the one given to the constructor.
					\see         ColorRGBA::ColorRGBA
			*/
			operator String() const;

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					\param       color the colorRGBA to be copied 
			*/
			void set(const ColorRGBA& color);

			/** Assignment operator.
					\see         set
			*/
			const ColorRGBA& operator = (const ColorRGBA& color);

			/** Assignment with ColorHSV.
			*/
			void set(const ColorHSV& color);

			/** Assignment operator with ColorHSV.
					Calls set.
			*/
			const ColorRGBA& operator = (const ColorHSV& color);

			/** Copying to ColorHSV.
			*/
			void get(ColorHSV& color) const;

			/** Assignment with char*.
					Assigns the color represented by the string <b> value</b> to this colorRGBA.
					See constructor for format description.
					\param       value the color represented by a string 
					\exception   InvalidRange if <tt> string length != 6 && string length != 8</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
			*/
			void set(const char* value);

			/** Assignment operator with char*.
					Calls set.
					See constructor for format description.
					\param       value the color represented by a string
					\exception   InvalidRange if <tt> string length != 6 && string length != 8</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
					\return      ColorRGBA& this colorRGBA
			*/
			const ColorRGBA& operator = (const char* value);

			/** Copying to char*.
					Copies the color to the string represented by a pointer.
					Calls set.
					At least 9 bytes must be allocated for the string. 
					The format of the string is the same as the one given to the constructor.
					\param       value the pointer to string (at least 9 bytes long)
					\see         set
			*/
			void get(char* value) const;

			/** Assign the color represented by a String.
					See constructor for format description.
					\param       value the color represented by a string 
					\exception   InvalidRange if <tt> string length != 6 && string length != 8</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
			*/
			void set(const String& value);

			/** Assign the color represented by the string <b> value</b> to this colorRGBA.
					Calls set.
					The color of this is initialized to the color represented by the string <b> value</b>.
					See constructor for format description.
					\param       value the color represented by a string
					\exception   InvalidRange if <tt> string length != 6 && string length != 8</tt>
					\exception   NotInHexFormat if the string is not a valid hex format string
					\return      ColorRGBA& this colorRGBA
					\see         set
			*/
			const ColorRGBA& operator = (const String& value);

			/** Copying to string.
					Calls set.
					The format of the string is	the same as the one given to the constructor.
					\param       value the string receiving the color in hex format
					\see         set
			*/
			void get(String& value) const;

			/// Assign to a QT QColor
			void get(QColor& color) const;

			/** Assign from a QT QColor.
			 		Aborts, if the QColor is invalid (see QColor::isValid()) and returns false.
					The alpha-value of this color is set to max.
			*/
			bool set(const QColor& color);

			/// Get a QColor representing this color
			QColor getQColor() const;

			/** Swapping of colorRGBAs.
					\param       color the colorRGBA being swapped with this colorRGBA 
			*/
			void swap(ColorRGBA& color);

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Changes the value of the red component.
					\see         getRed
			*/
			void setRed(const ColorUnit& red_value);

			/** Mutable inspection of the red component.
					\return      ColorUnit& mutable reference to the red component of this colorRGBA
					\see         setRed
			*/
			ColorUnit& getRed();

			/** Non-mutable inspection of the red component.
			*/
			const ColorUnit& getRed() const;

			/** Changes the value of the green component.
					\param       green_value the new green component of this colorRGBA
					\see         getGreen
			*/
			void setGreen(const ColorUnit& green_value);

			/** Mutable inspection of the green component.
					\return      ColorUnit& mutable reference to the green component of this colorRGBA
					\see         setGreen
			*/
			ColorUnit& getGreen();

			/** Non-mutable inspection of the green component.
					For further information see <b> ColorUnit& getGreen()</b>.
			*/
			const ColorUnit& getGreen() const;

			/** Changes the value of the blue component.
					\param       blue_value the new blue component of this colorRGBA
					\see         getBlue
			*/
			void setBlue(const ColorUnit& blue_value);

			/** Mutable inspection of the blue component.
					\see         setBlue
			*/
			ColorUnit& getBlue();

			/** Non-mutable inspection of the blue component.
    			For further information see <b> ColorUnit& getBlue()</b>
			*/
			const ColorUnit& getBlue() const;

			/** Changes the value of the alpha component.
					\param       alpha_value the new alpha component of this colorRGBA
					\see         getAlpha
			*/
			void setAlpha(const ColorUnit& alpha_value);

			/** Mutable inspection of the alpha component.
					\return      ColorUnit& mutable reference to the alpha component of this colorRGBA
					\see         setAlpha
			*/
			ColorUnit& getAlpha();

			/** Non-mutable inspection of the alpha component.
			    For further information see <b> ColorUnit& getAlpha()</b>.
			*/
			const ColorUnit& getAlpha() const;

			/** Changes the values of the red, green, blue and alpha component.
					Changes the values of of the red, green, blue and alpha component of this
					colorRGBA to the values represented by the parameters. See ColorUnit for
					further information concerning type conversions.
					\param       red_value the new red component of this colorRGBA
					\param       green_value the new green component of this colorRGBA
					\param       blue_value the new blue component of this colorRGBA
					\param       alpha_value the new alpha component of this colorRGBA. Default 1.0
					\see         get
					\see         ColorUnit
			*/
			void set
				(const ColorUnit& red_value, const ColorUnit& green_value,
				 const ColorUnit& blue_value, const ColorUnit& alpha_value = (float)1);

			/** Inspection of the colorRGBA's components.
					Accesses all the components of this colorRGBA by using ColorUnit's.
					\param      red_value the colorUnit receiving the red component of this colorRGBA
					\param      green_value the colorUnit receiving the green component of this colorRGBA
					\param      blue_value the colorUnit receiving the blue component of this colorRGBA
					\param      alpha_value the colorUnit receiving the alpha component of this colorRGBA
					\see        set
					\see        ColorUnit
			*/
			void get(ColorUnit& red_value, ColorUnit& green_value,
							 ColorUnit& blue_value, ColorUnit& alpha_value) const;

			/** Inspection of the colorRGBA's components.
					Accesses only the red, green and blue components of this colorRGBA by using ColorUnit's.
					\param      red_value the colorUnit receiving the red component of this colorRGBA
					\param      green_value the colorUnit receiving the green component of this colorRGBA
					\param      blue_value the colorUnit receiving the blue component of this colorRGBA
					\see        set
					\see        ColorUnit
			*/
			void get(ColorUnit& red_value,
							 ColorUnit& green_value,
							 ColorUnit& blue_value) const;

			///
			ColorRGBA getInverseColor() const;
			 
			//@}
			/**	@name	Predicates
			*/
			//@{

			/** Equality test.
					Tests if <b> color</b> is equal to this colorRGBA.
			*/
			bool operator == (const ColorRGBA& color) const;

			/** Unequality test.
			*/
			bool operator !=  (const ColorRGBA& color) const;

			/** Smaller test.
					Smaller means that all components of this are smaller than all the
					components of <b> color</b> colorRGBA. 
			*/
			bool operator < (const ColorRGBA& color) const;

			/** Smaller or equal test.
					Smaller or equal means that all components of this are smaller or equal than all the
					components of <b> color</b> colorRGBA. 
			*/
			bool operator <= (const ColorRGBA& color) const;

			/** Greater test.
					Greater means that all components of this are greater than all the
					components of <b> color</b> colorRGBA.
			*/
			bool operator > (const ColorRGBA& color) const;

			/** Greater or equal test.
					Greater or equal means that all components of this are greater or equal than all the
					components of <b> color</b> colorRGBA.
			*/
			bool operator >= (const ColorRGBA& color) const;
			
			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dumps the current color of this colorRGBA to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the color of this colorRGBA
					\param   depth the dumping depth
					\see     ColorUnit::dump
			*/
			void dump(std::ostream& s = std::cout, Size depth = 0) const;
			
			//@}
			/**	@name	Storers
			*/
			//@{

			/** Friendly stream input.
					Reads colorRGBA data from the input stream <b> s</b> and restore the color of this.
					The red, green, blue and alpha component are read as integers from the input stream.
					THe method expects an vector as written as the one by the output stream operator.
					This method is provided for convienience.
					\param  s input stream from where to restore the color of this colorRGBA
					\param  color the colorRGBA to which the color will be restored
			*/
			friend BALL_VIEW_EXPORT std::istream& operator >> (std::istream& s, ColorRGBA& color);

			/** Friendly stream output.
					Writes colorRGBA data to the output stream <b> s</b> and store the color of this.
					The color will be stored as an vector in integer format. The output will look
					like '(0-255, 0-255, 0-255, 0-255)'.
					This method is provided for convienience.
					\param  s output stream to where the color of this colorRGBA will be stored
					\param  color the colorRGBA from which the color will be stored
			*/
			friend BALL_VIEW_EXPORT std::ostream& operator << (std::ostream& s, const ColorRGBA& color);

			//@}

			private:

			ColorUnit red_;
			ColorUnit green_;
			ColorUnit blue_;
			ColorUnit alpha_;

			// @throw Exception::InvalidRange 
			// @throw Exception::NotInHexFormat
			void stringToRGBA_(const String& s);
		};

		/** Static instance for the color of selected geometric objects */
		extern ColorRGBA 				BALL_SELECTED_COLOR;
		extern PreciseTime 		 	BALL_SELECTED_COLOR_CHANGE_TIME;

		/** needed for windows dlls **/
#ifdef BALL_COMPILER_MSVC
#include <vector>
template class BALL_VIEW_EXPORT std::vector<ColorRGBA>;
#endif

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/colorRGBA.iC>
#		endif
  		
	} // namespace VIEW
} // namespace BALL

#endif // <BALL/VIEW_DATATYPE_COLORRGBA_H
