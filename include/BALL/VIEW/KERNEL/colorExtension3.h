// $Id: colorExtension3.h,v 1.6 2001/07/09 17:37:47 sturm Exp $

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION3_H
#define BALL_VIEW_KERNEL_COLOREXTENSION3_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION2_H
#	include <BALL/VIEW/KERNEL/colorExtension2.h>
#endif


namespace BALL
{
	namespace VIEW
	{

		/** ColorExtension3 class.
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/colorExtension3.h}\\ \\
				The class ColorExtension3 is used as a base class for all geometric 
				objects that needs three colors \Ref{ColorRGBA}. It provides the derived
				class with methods for accessing these colors.
				@memo    ColorExtension3 class (BALL VIEW kernel framework)
				@author  $Author: sturm $
				@version $Revision: 1.6 $
				@date    $Date: 2001/07/09 17:37:47 $
		*/
		class ColorExtension3: public ColorExtension2
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new colorExtension3.
					The colors of {\em *this} colorExtension3 are set to (0.0, 0.0, 0.0, 1.0).
					Calls \Ref{ColorExtension2}.
					@return      ColorExtension3 new constructed colorExtension3
					@see         ColorRGBA
					@see         ColorExtension2
			*/
			ColorExtension3()
				throw();

			/** Copy constructor.
					Construct new colorExtension3 by copying the colorExtension3 
					{\em color_extension}. The colors of {\em *this} colorExtension3 are
					initialized to the colors of {\em color_extension}.
					Calls \Ref{ColorExtension2}.
					@param       color_extension the colorExtension3 to be copied
					@return      ColorExtension3 new constructed colorExtension3 copied from {\em color_extension}
					@see         ColorRGBA
					@see         ColorExtension2
			*/
			ColorExtension3(const ColorExtension3& color_extension)
				throw();

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} colorExtension3.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~ColorExtension3()
				throw();

			/** Explicit default initialization.
					Set the colors of {\em *this} colorExtension3 to the color 
					(0.0, 0.0, 0.0, 1.0).
					Calls \Ref{ColorExtension2::clear}.
					Calls \Ref{ColorRGBA::clear}.		
					@see  ColorExtension2::clear
					@see  ColorRGBA::clear
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
					Assign the colorExtension3 {\em color_extension} to {\em *this} 
					colorExtension3.
					The colors of {\em *this} colorExtension3 are initialized to the colors of 
					the colorExtension3 {\em color_extension}.
					Calls \Ref{ColorExtension2::set}.
					Calls \Ref{ColorRGBA::set}.
					@param       color_extension the colorExtension3 to be copied
					@see         ColorExtension2::set
					@see         ColorRGBA
					@see         ColorRGBA::set
			*/
			void set(const ColorExtension3& color_extension)
				throw();

			/** Assignment operator.
					Assign the colorExtension3 {\em color_extension} to {\em *this}
					colorExtension3.
					Calls \Ref{set}.
					The colors of {\em *this} colorExtension3 are initialized to the colors 
					of the colorExtension3 {\em color_extension}.\\
					@param       color_extension the colorExtension3 to be copied
					@return      ColorExtension3& constant reference of {\em *this} colorExtension3
					@see         set
			*/
			const ColorExtension3& operator = (const ColorExtension3& color_extension)
				throw();

			/** Copying.
					Copy {\em *this} colorExtension3 to the colorExtension3
					{\em color_extension}.
					Calls \Ref{set}.
					The colors of the colorExtension3 {\em color_extension} 
					are initialized to the colors of {\em *this} colorExtension3.\\
					@param       color_extension the colorExtension3 to be assigned to
					@see         set
			*/
			void get(ColorExtension3& color_extension) const
				throw();

			/** Swapping of colorExtensions.
					Swap the colors of {\em *this} colorExtension3 with the colorExtension3
					{\em color_extension}.
					Calls \Ref{ColorExtension2::swap}.
					Calls \Ref{ColorRGBA::swap}.
					@param       color_extension the colorExtension3 being swapped with {\em *this} colorExtension 
					@see         ColorExtension3
					@see         ColorExtension2::swap
					@see         ColorRGBA::swap
			*/
			void swap(ColorExtension3& color_extension)
				throw();
			//@}

			
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the third color of {\em *this} colorExtension3.
					Change the third color of {\em *this} colorExtension3 to the color
					represented by the parameter {\em color}.
					@param       color the new third color of {\em *this} colorExtension3
					@see         getColor3
					@see         ColorRGBA
			*/
			void setColor3(const ColorRGBA& color)
				throw();
			
			/** Change the third color of {\em *this} colorExtension3.
					Change the third color of {\em *this} colorExtension3 to the color 
					represented	by the parameters {\em red}, {\em green}, {\em blue} and
					{\em alpha}.
					@param       red the red component of the new third color of {\em *this} colorExtension3
					@param       green the green component of the new third color of {\em *this} colorExtension3
					@param       blue the blue component of the new third color of {\em *this} colorExtension3
					@param       alpha the alpha component of the new third color of {\em *this} colorExtension3 (Default=1.0)
					@see         getColor3
					@see         ColorUnit
			*/
			void setColor3
				(const ColorUnit& red, const ColorUnit& green,
				 const ColorUnit& blue, const ColorUnit& alpha = (float)1)
				throw();
			
			/** Mutable inspection of the third color of {\em *this} colorExtension3.
					Access the mutual reference of the third color of {\em *this}
					colorExtension3.
					@return      ColorRGBA& mutable reference to the third color of {\em *this} colorExtension3
					@see         setColor3
					@see         ColorRGBA
			*/
			ColorRGBA& getColor3()
				throw();

			/** Non-mutable inspection of the third color of {\em *this} colorExtension3.
					Access the constant reference of the third color of {\em *this}
					colorExtension3.
					@return      ColorRGBA& constant reference to the third color of {\em *this} colorExtension3
					@see         setColor3
					@see         ColorRGBA
			*/
			const ColorRGBA& getColor3() const
				throw();

			/** Inspection of the third color of {\em *this} colorExtension3.
					For further information see \Ref{getColor3}.
			*/
			void getColor3(ColorRGBA& color) const
				throw();

			/** Inspection of all the components of the third color of {\em *this} colorExtension3.
					Access all the components of the third color of {\em *this} colorExtension3
					by using \Ref{ColorUnit}.
					@param       red the red component of the third color of {\em *this} colorExtension3
					@param       green the green component of the third color of {\em *this} colorExtension3
					@param       blue the blue component of the third color of {\em *this} colorExtension3
					@param       alpha the alpha component of the third color of {\em *this} colorExtension3
					@see         setColor3
					@see         ColorUnit
			*/
			void getColor3
				(ColorUnit& red, ColorUnit& green,
				 ColorUnit& blue, ColorUnit& alpha) const
				throw();

			/** Inspection of the components of the third color of {\em *this}
					colorExtension3.
					Access only the red, green and blue components of the third color 
					of {\em *this} colorExtension3	by using \Ref{ColorUnit}.
					@param       red the red component of the third color of {\em *this} colorExtension3
					@param       green the green component of the third color of {\em *this} colorExtension3
					@param       blue the blue component of the third color of {\em *this} colorExtension3
					@see         setColor3
					@see         ColorUnit
			*/
			void getColor3(ColorUnit& red, ColorUnit& green, ColorUnit& blue) const
				throw();

			/** Change the first, second and third color of {\em *this} colorExtension3.
					Change the first, second and third color of {\em *this} colorExtension3
					to the colors	represented by the parameters {\em a}, {\em b} and {\em c}.
					Calls \Ref{ColorExtension2::setColors}.
					@param       a the new first color of {\em *this} colorExtension3
					@param       b the new second color of {\em *this} colorExtension3
					@param       c the new third color of {\em *this} colorExtension3
					@see         getColors
					@see         ColorExtension2::setColors
					@see         ColorRGBA
			*/
			void setColors(const ColorRGBA& a, const ColorRGBA& b, const ColorRGBA& c)
				throw();

			/** Inspection of the first, second and third color of {\em *this}
					colorExtension3.
					Access the first, second and third color of {\em *this} colorExtension3
					by using \Ref{ColorRGBA}.
					Calls \Ref{ColorExtension2::getColors}.
					@param       a the color receiving the first color of {\em *this} colorExtension3
					@param       b the color receiving the second color of {\em *this} colorExtension3
					@param       c the color receiving the third color of {\em *this} colorExtension3
					@see         setColors
					@see         ColorExtension2::getColors
					@see         ColorRGBA
			*/
			void getColors(ColorRGBA& a, ColorRGBA& b, ColorRGBA& c) const
				throw();
				
			//@}

			/**	@name	debuggers and diagnostics
			*/	
			//@{

			/** Internal value dump.
					Dump the current state of {\em *this} colorExtension3 to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} colorExtension3
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
  			  Read persistent colorExtension3 data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal state of {\em *this} colorExtension3
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent colorExtension3 data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} colorExtension3
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}

			private:
		
			/* color 3 */
			ColorRGBA color3_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/colorExtension3.iC>
#		endif

	} // namespace VIEW
	
} // namespace BALL

#endif // BALL_VIEW_KERNEL_COLOREXTENSION3_H
