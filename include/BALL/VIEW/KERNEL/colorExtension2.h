// $Id: colorExtension2.h,v 1.8 2001/07/16 14:49:32 amoll Exp $

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION2_H
#define BALL_VIEW_KERNEL_COLOREXTENSION2_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
# include <BALL/VIEW/DATATYPE/color.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** ColorExtension2 class.
				The class ColorExtension2 is used as a base class for all geometric 
				objects that needs two colors \Ref{ColorRGBA}. It provides the derived
				class with methods for accessing these colors.
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/colorExtension2.h}
		*/
		class ColorExtension2
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new colorExtension2.
					The colors of {\em *this} colorExtension2 are set to (0.0, 0.0, 0.0, 1.0).
					@return      ColorExtension2 new constructed colorExtension2
					@see         ColorRGBA
			*/
			ColorExtension2()
				throw();

			/** Copy constructor.
					Construct new colorExtension2 by copying the colorExtension2 
					{\em color_extension}. The colors of {\em *this} colorExtension2 are
					initialized to the colors of {\em color_extension}.
					@param       color_extension the colorExtension2 to be copied
					@return      ColorExtension2 new constructed colorExtension2 copied from {\em color_extension}
					@see         ColorRGBA
			*/
			ColorExtension2(const ColorExtension2& color_extension)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} colorExtension2.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~ColorExtension2()
				throw();

			/** Explicit default initialization.
					Set the colors of {\em *this} colorExtension2 to the color 
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
					Assign the colorExtension2 {\em color_extension} to {\em *this} 
					colorExtension2.
					The colors of {\em *this} colorExtension2 are initialized to the colors of 
					the colorExtension2 {\em color_extension}.
					Calls \Ref{ColorRGBA::set}.
					@param       color_extension the colorExtension2 to be copied
					@see         ColorRGBA
					@see         ColorRGBA::set
			*/
			void set(const ColorExtension2& color_extension)
				throw();

			/** Assignment operator.
					Assign the colorExtension2 {\em color_extension} to {\em *this}
					colorExtension2.
					Calls \Ref{set}.
					The colors of {\em *this} colorExtension2 are initialized to the colors 
					of the colorExtension2 {\em color_extension}.\\
					@param       color_extension the colorExtension2 to be copied
					@return      ColorExtension2& const reference of {\em *this} colorExtension2
					@see         set
			*/
			const ColorExtension2& operator = (const ColorExtension2& color_extension)
				throw();

			/** Copying.
					Copy {\em *this} colorExtension2 to the colorExtension2
					{\em color_extension}.
					Calls \Ref{set}.
					The colors of the colorExtension2 {\em color_extension} 
					are initialized to the colors of {\em *this} colorExtension2.\\
					@param       color_extension the colorExtension2 to be assigned to
					@see         set
			*/
			void get(ColorExtension2& color_extension) const
				throw();

			/** Swapping of colorExtensions.
					Swap the colors of {\em *this} colorExtension2 with the colorExtension2
					{\em color_extension}.
					Calls \Ref{ColorRGBA::swap}.
					@param       color_extension the colorExtension2 being swapped with {\em *this} colorExtension 
					@see         ColorExtension2
					@see         ColorRGBA::swap
			*/
			void swap(ColorExtension2& color_extension)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the first color of {\em *this} colorExtension2.
					Change the first color of {\em *this} colorExtension2 to the color
					represented by the parameter {\em color}.
					@param       color the new first color of {\em *this} colorExtension2
					@see         getColor1
					@see         ColorRGBA
			*/
			void setColor1(const ColorRGBA& color)
				throw();
			
			/** Change the first color of {\em *this} colorExtension2.
					Change the first color of {\em *this} colorExtension2 to the color 
					represented	by the parameters {\em red}, {\em green}, {\em blue} and
					{\em alpha}.
					@param       red the red component of the new first color of {\em *this} colorExtension2
					@param       green the green component of the new first color of {\em *this} colorExtension2
					@param       blue the blue component of the new first color of {\em *this} colorExtension2
					@param       alpha the alpha component of the new first color of {\em *this} colorExtension2 (Default=1.0)
					@see         getColor1
					@see         ColorUnit
			*/
			void setColor1
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1)
				throw();
			
			/** Mutable inspection of the first color of {\em *this} colorExtension2.
					Access the mutual reference of the first color of {\em *this}
					colorExtension2.
					@return      ColorRGBA& mutable reference to the first color of {\em *this} colorExtension2
					@see         setColor1
					@see         ColorRGBA
			*/
			ColorRGBA& getColor1()
				throw();

			/** Non-mutable inspection of the first color of {\em *this} colorExtension2.
					For further information see \Ref{getColor1}.
			*/
			const ColorRGBA& getColor1() const
				throw();

			/** Inspection of all the components of the first color of {\em *this} colorExtension2.
					Access all the components of the first color of {\em *this} colorExtension2
					by using \Ref{ColorUnit}.
					@param       red the red component of the first color of {\em *this} colorExtension2
					@param       green the green component of the first color of {\em *this} colorExtension2
					@param       blue the blue component of the first color of {\em *this} colorExtension2
					@param       alpha the alpha component of the first color of {\em *this} colorExtension2
					@see         setColor1
					@see         ColorUnit
			*/
			void getColor1
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const
				throw();
			
			/** Inspection of the components of the first color of {\em *this}
					colorExtension2.
					Access only the red, green and blue components of the first color 
					of {\em *this} colorExtension2	by using \Ref{ColorUnit}.
					@param       red the red component of the first color of {\em *this} colorExtension2
					@param       green the green component of the first color of {\em *this} colorExtension2
					@param       blue the blue component of the first color of {\em *this} colorExtension2
					@see         setColor1
					@see         ColorUnit
			*/
			void getColor1
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue) const
				throw();
			
			/** Inspection of the first color of {\em *this} colorExtension2.
					Access the first color of {\em *this} colorExtension2 by using
					\Ref{ColorRGBA}.
					@param       color the color receiving the first color of {\em *this} colorExtension2
					@see         setColor1
					@see         ColorRGBA
			*/
			void getColor1(ColorRGBA& color) const
				throw();

			/** Change the second color of {\em *this} colorExtension2.
					Change the second color of {\em *this} colorExtension2 to the color
					represented by the parameter {\em color}.
					@param       color the new second color of {\em *this} colorExtension2
					@see         getColor2
					@see         ColorRGBA
			*/
			void setColor2(const ColorRGBA& color)
				throw();
			
			/** Change the second color of {\em *this} colorExtension2.
					Change the second color of {\em *this} colorExtension2 to the color 
					represented	by the parameters {\em red}, {\em green}, {\em blue} and
					{\em alpha}.
					@param       red the red component of the new second color of {\em *this} colorExtension2
					@param       green the green component of the new second color of {\em *this} colorExtension2
					@param       blue the blue component of the new second color of {\em *this} colorExtension2
					@param       alpha the alpha component of the new second color of {\em *this} colorExtension2 (Default=1.0)
					@see         getColor2
					@see         ColorUnit
			*/
			void setColor2
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1)
				throw();
			
			/** Inspection of all the components of the second color of {\em *this}
					colorExtension2.
					Access all the components of the second color of {\em *this} colorExtension2
					by using \Ref{ColorUnit}.
					@param       red the red component of the second color of {\em *this} colorExtension2
					@param       green the green component of the second color of {\em *this} colorExtension2
					@param       blue the blue component of the second color of {\em *this} colorExtension2
					@param       alpha the alpha component of the second color of {\em *this} colorExtension2
					@see         setColor2
					@see         ColorUnit
			*/
			void getColor2
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const
				throw();
			
			/** Inspection of the components of the second color of {\em *this}
					colorExtension2.
					Access only the red, green and blue components of the second color 
					of {\em *this} colorExtension2	by using \Ref{ColorUnit}.
					@param       red the red component of the second color of {\em *this} colorExtension2
					@param       green the green component of the second color of {\em *this} colorExtension2
					@param       blue the blue component of the second color of {\em *this} colorExtension2
					@see         setColor2
					@see         ColorUnit
			*/
			void getColor2
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue) const
				throw();
			
			/** Mutable inspection of the second color of {\em *this} colorExtension2.
					Access the mutual reference of the second color of {\em *this}
					colorExtension2.
					@return      ColorRGBA& mutable reference to the second color of {\em *this} colorExtension2
					@see         setColor2
					@see         ColorRGBA
			*/
			ColorRGBA& getColor2()
				throw();

			/** Non-mutable inspection of the second color of {\em *this} colorExtension2.
					For further information see \Ref{getColor2}.
			*/
			const ColorRGBA& getColor2() const
				throw();

			/** Inspection of the second color of {\em *this} colorExtension2.
					Access the second color of {\em *this} colorExtension2 by using
					\Ref{ColorRGBA}.
					@param       color the color receiving the second color of {\em *this} colorExtension2
					@see         setColor2
					@see         ColorRGBA
			*/
			void getColor2(ColorRGBA& color) const
				throw();

			/** Change the first and second color of {\em *this} colorExtension2.
					Change the first and second color of {\em *this} colorExtension2
					to the colors	represented by the parameters {\em a} and {\em b}.
					@param       a the new first color of {\em *this} colorExtension2
					@param       b the new second color of {\em *this} colorExtension2
					@see         getColors
					@see         ColorRGBA
			*/
			void setColors(const ColorRGBA& a, const ColorRGBA& b)
				throw();
				
			/** Inspection of the first and second color of {\em *this} colorExtension2.
					Access the first and second color of {\em *this} colorExtension2 by using
					\Ref{ColorRGBA}.
					@param       a the color receiving the first color of {\em *this} colorExtension2
					@param       b the color receiving the second color of {\em *this} colorExtension2
					@see         setColors
					@see         ColorRGBA
			*/
			void getColors(ColorRGBA& a, ColorRGBA& b) const
				throw();

			//@}

			/**	@name	debuggers and diagnostics
			*/	
			//@{

			/** Internal value dump.
					Dump the current state of {\em *this} colorExtension2 to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} colorExtension2
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
  			  Read persistent colorExtension2 data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
					@param       s input stream from where to restore the internal state of {\em *this} colorExtension2
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
   			  Write persistent colorExtension2 data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} colorExtension2
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}

			private:
		
			/* color 1 */
			ColorRGBA color1_;

			/* color 2 */
			ColorRGBA color2_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension2.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_COLOREXTENSION2_H
