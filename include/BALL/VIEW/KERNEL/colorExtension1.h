// $Id: colorExtension1.h,v 1.7 2001/07/16 14:49:32 amoll Exp $

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION1_H
#define BALL_VIEW_KERNEL_COLOREXTENSION1_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif


namespace BALL
{
	namespace VIEW
	{

		/** ColorExtension class.
				The class ColorExtension is used as a base class for all geometric 
				objects that need a single color \Ref{ColorRGBA}. It provides the derived
				class with methods for accessing that color.
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/colorExtension1.h}
		*/
		class ColorExtension
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new colorExtension.
					The color of {\em *this} colorExtension is set to (0.0, 0.0, 0.0, 1.0).
					@return      ColorExtension new constructed colorExtension
					@see         ColorRGBA
			*/
			ColorExtension()
				throw();

			/** Copy constructor.
					Construct new colorExtension by copying the colorExtension 
					{\em color_extension}. The color of {\em *this} colorExtension is
					initialized to the color of {\em color_extension}.
					@param       color_extension the colorExtension to be copied
					@return      ColorExtension new constructed colorExtension copied from {\em color_extension}
					@see         ColorRGBA
			*/
			ColorExtension(const ColorExtension& color_extension)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} colorExtension.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~ColorExtension()
				throw();

			/** Explicit default initialization.
					Set the color of {\em *this} colorExtension to the color 
					(0.0, 0.0, 0.0, 1.0).
					Calls \Ref{ColorRGBA::clear}.
					@see ColorRGBA::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}
		
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the colorExtension {\em color_extension} to {\em *this} 
					colorExtension.
					The color of {\em *this} colorExtension is initialized to the color of 
					the colorExtension {\em color_extension}.
					Calls \Ref{ColorRGBA::set}.
					@param       color_extension the colorExtension to be copied
					@see         ColorRGBA
					@see         ColorRGBA::set
			*/
			void set(const ColorExtension& color_extension)
				throw();

			/** Assignment operator.
					Assign the colorExtension {\em color_extension} to {\em *this}
					colorExtension.
					Calls \Ref{set}.
					The color of {\em *this} colorExtension is initialized to the color 
					of the colorExtension {\em color_extension}.\\
					@param       color_extension the colorExtension to be copied
					@return      ColorExtension& {\em *this} colorExtension
					@see         set
			*/
			const ColorExtension& operator = (const ColorExtension& color_extension)
				throw();

			/** Copying.
					Copy {\em *this} colorExtension to the colorExtension {\em color_extension}.
					Calls \Ref{set}.
					The color of the colorExtension {\em color_extension} is initialized to the
					color of {\em *this} colorExtension.\\
					@param       color_extension the colorExtension to be assigned to
					@see         set
			*/
			void get(ColorExtension& color_extension) const
				throw();

			/** Swapping of colorExtensions.
					Swap the color of {\em *this} colorExtension with the colorExtension
					{\em color_extension}.
					Calls \Ref{ColorRGBA::swap}.
					@param       color_extension the colorExtension being swapped with {\em *this} colorExtension 
					@see         ColorExtension
					@see         ColorRGBA::swap
			*/
			void swap(ColorExtension& color_extension)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the color of {\em *this} colorExtension.
					Change the color of {\em *this} colorExtension to the color
					represented by the parameter {\em color_extension}.
					@param       color_extension the new color of {\em *this} colorExtension
					@see         getColor
					@see         ColorRGBA
			*/
			void setColor(const ColorRGBA& color)
				throw();
			
			/** Change the color of {\em *this} colorExtension.
					Change the color of {\em *this} colorExtension to the color represented
					by the parameters {\em red}, {\em green}, {\em blue} and {\em alpha}.
					@param       red the red component of the new color of {\em *this} colorExtension
					@param       green the green component of the new color of {\em *this} colorExtension
					@param       blue the blue component of the new color of {\em *this} colorExtension
					@param       alpha the alpha component of the new color of {\em *this} colorExtension (Default=1.0)
					@see         getColor
					@see         ColorUnit
			*/
			void setColor
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1)
				throw();
			
			/** Mutable inspection of the color of {\em *this} colorExtension.
					Access the mutual reference of the color of {\em *this} colorExtension.
					@return      ColorRGBA& mutable reference to the color of {\em *this} colorExtension
					@see         setColor
					@see         ColorRGBA
			*/
			ColorRGBA& getColor()
				throw();

			/** Non-mutable inspection of the color of {\em *this} colorExtension.
					For further information see \Ref{getColor}.
			*/
			const ColorRGBA& getColor() const
				throw();

			/** Inspection of the color of {\em *this} colorExtension.
					Access the color of {\em *this} colorExtension by using \Ref{ColorRGBA}.
					@param       color the color receiving the color of {\em *this} colorExtension
					@see         setColor
					@see         ColorRGBA
			*/
			void getColor(ColorRGBA& color) const
				throw();

			/** Inspection of all the components of the color of {\em *this} colorExtension.
					Access all the components of the color of {\em *this} colorExtension
					by using \Ref{ColorUnit}.
					@param       red the red component of the color of {\em *this} colorExtension
					@param       green the green component of the color of {\em *this} colorExtension
					@param       blue the blue component of the color of {\em *this} colorExtension
					@param       alpha the alpha component of the color of {\em *this} colorExtension
					@see         setColor
					@see         ColorUnit
			*/
			void getColor
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const
				throw();

			/** Inspection of the components of the color of {\em *this} colorExtension.
					Access only the red, green and blue components of the color 
					of {\em *this} colorExtension	by using \Ref{ColorUnit}.
					@param       red the red component of the color of {\em *this} colorExtension
					@param       green the green component of the color of {\em *this} colorExtension
					@param       blue the blue component of the color of {\em *this} colorExtension
					@see         setColor
					@see         ColorUnit
			*/
			void getColor
				(ColorUnit& red,
				 ColorUnit& green,
				 ColorUnit& blue) const
				throw();
			//@}

			/**	@name	debuggers and diagnostics
			*/	
			//@{

			/** Internal value dump.
					Dump the current state of {\em *this} colorExtension to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} colorExtension
					@param   depth the dumping depth
					@see     ColorRGBA::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			 Read persistent colorExtension data from the input stream {\em s} and 
				 restore the state of {\em *this}.\\
				 {\bf Note:} Not yet implemented.
				 @param       s input stream from where to restore the internal state of {\em *this} colorExtension
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			 Write persistent colorExtension data to the output stream {\em s} and 
				 store the state of {\em *this}.\\
				 {\bf Note:} Not yet implemented.
				 @param       s output stream to where to store the internal state of {\em *this} colorExtension
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}

			private:
		
			/* color */
			ColorRGBA color_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension1.iC>
#		endif

	} // namespace VIEW

} // namespace BALL


#endif // BALL_VIEW_KERNEL_COLOREXTENSION1_H
