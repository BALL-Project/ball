// $Id: colorCalculator.h,v 1.7 2001/05/13 14:55:22 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H
#define BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_PTE_H
#	include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORTABLE_H
#	include <BALL/VIEW/DATATYPE/colorTable.h>
#endif

#ifndef BALL_MOLVIEW_COLORS_ELEMENTCOLOR_H
#	include <BALL/MOLVIEW/COLORS/elementColor.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
#endif

  
namespace BALL
{

	namespace MOLVIEW
	{
		
		/** ColorCalculator class.
				{\bf Framework:} BALL/MOLVIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/colorCalculator.h} \\
				{\bf Concept:} visitor design pattern\\	\\
				The class ColorCalculator is derived from both \Ref{Visitor} of \Ref{Atom}
				and \Ref{Composite}. So there are \Ref{visit} methods for both \Ref{Atom} and
				\Ref{Composite} objects. The class ColorCalculator is used for calculating
				a \Ref{ColorRGBA} for each given \Ref{Atom} object. A \Ref{ColorMap} is used
				to store the colors with a given name. That name will be calculated by the method
				\Ref{calculateKey} from the given object. If a name is not inserted into the 
				color map a default color	will be returned. 
				This class and their derived classes will be used by the model processors to
				calculate a color for each \Ref{Atom} object they process.
				@memo    ColorCalculator class (BALL MOLVIEW functor framework)
				@author  $Author: hekl $
				@version $Revision: 1.7 $
				@date    $Date: 2001/05/13 14:55:22 $
		*/
		class ColorCalculator
			: public Visitor<Atom>,
				public Visitor<Composite>
		{
			public:
			
			/**	@name	Type definitions
			*/
			//@{
				
			/**	ColorMap type.
					Define the type of a colorMap. A colorMap is a string hash map from strings
					to \Ref{ColorRGBA} objects.
					@see   StringHashMap
					@see   ColorRGBA
			*/
			typedef StringHashMap<ColorRGBA>	ColorMap;
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new colorCalculator.
					@return      ColorCalculator new constructed colorCalculator
					@see         Visitor
			*/
			ColorCalculator()
				throw();

			/** Copy constructor.
					Construct new colorCalculator by copying the colorCalculator {\em color_calculator}.
					@param       color_calculator the colorCalculator to be copied
					@return      ColorCalculator new constructed ColorCalculator copied from {\em colorCalculator}
					@see         Visitor
					@see         color_map_
					@see         default_color_
					@see         color_
			*/
			ColorCalculator
				(const ColorCalculator& color_calculator)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} colorCalculator.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~ColorCalculator()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} colorCalculator to:
					\begin{itemize}
					  \item  {\em color_} set to red ({\tt "FF0000FF"})
					  \item  {\em default_color_} set to red ({\tt "FF0000FF"})
					  \item  {\em color_map_} is cleared
					\end{itemize}
					@see  color_
					@see  default_color_
					@see  color_map_
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Asignment
			*/
			//@{

			/** Assignment.
					Assign the colorCalculator {\em color_calculator} to {\em *this}
					colorCalculator.
					The colors and the colorMap of {\em *this} are initialized to the colors and 
					the colorMap of the	colorCalculator {\em color_calculator}.
					@param       color_calculator the colorCalculator to be copied 
					@see         color_
					@see         default_color_
					@see         color_map_
			*/
			void set
				(const ColorCalculator& color_calculator)
				throw();

			/** Assignment operator.
					Assign the colorCalculator {\em color_calculator} to {\em *this}
					colorCalculator.
					Calls \Ref{set}.
					The colors and the colorMap of {\em *this} are initialized to the colors and 
					the colorMap of the colorCalculator {\em color_calculator}. 
					@param       color_calculator the colorCalculator to be copied
					@return      ColorCalculator& constant reference of {\em *this} colorCalculator
					@see         set
			*/
			const ColorCalculator&  operator =
				(const ColorCalculator& color_calculator)
				throw();

			/** Copying.
					Copy {\em *this} colorCalculator to the colorCalculator 
					{\em color_calculator}.
					Calls \Ref{set}.
					The colors and the colorMap of {\em *this} are initialized to the colors and 
					the colorMap of the colorCalculator {\em color_calculator}. 
					@param       color_calculator the colorCalculator to be assigned to
					@see         set
			*/
			void get
				(ColorCalculator&  color_calculator) const
				throw();

			/** Swapping of colorCalculator's.
					Swap the colors and the colorMap of {\em *this} colorCalculator with
					the colorCalculator	{\em color_calculator}.
					Calls \Ref{ColorRGBA::swap}.
					Calls \Ref{StringHashMap::swap}.
					@param       color_calculator the colorCalculator being swapped with {\em *this} colorCalculator 
					@see         color_
					@see         default_color_
					@see         color_map_
			*/
			void swap
				(ColorCalculator&  color_calculator)
				throw();
			//@}

			
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the default color.
					Change the default color of {\em *this} colorCalculator.
					If the object given to the \Ref{visit} method has a name that is not inserted
					into the \Ref{color_map_} the default color will be written into the variable
					\Ref{color_}. So the method \Ref{getColor} always returns a valid color.
					@param color the new default color.
					@see   getDefaultColor
					@see   getColor
					@see   visit
					@see   default_color_
			*/
			void setDefaultColor(const ColorRGBA& color)
				throw();

			/** Non-mutable inspection of the default color.
					Access the constant reference of the default color of {\em *this} colorCalculator.
					@return  ColorRGBA& a constant reference to the default \Ref{ColorRGBA} of {\em *this} colorCalculator
					@see     setDefaultColor
					@see     default_color_
			*/
			const ColorRGBA& getDefaultColor() const
				throw();

			/** Non-mutable inspection of the chosen color.
					Access the constant reference of the chosen color of {\em *this} colorCalculator.
					@return  ColorRGBA& a constant reference to the chosen \Ref{ColorRGBA} of {\em *this} colorCalculator
					@see     visit
					@see     color_
			*/
			const ColorRGBA& getColor() const
				throw();

			/** Change the color map.
					Change the color map of {\em *this} colorCalculator. The given \Ref{ColorMap}
					{\em color_map} will be used in the \Ref{visit} method to retrieve a 
					\Ref{ColorRGBA} for a given object.
					@param  color_map the new \Ref{ColorMap} of {\em *this} colorCalculator
					@see    getColorMap
					@see    visit
					@see    ColorMap
					@see    color_map_
			*/
			void setColorMap(const ColorMap& color_map)
				throw();

			/** Non-mutable inspection of the color map.
					Access the constant reference of the color map of {\em *this}	colorCalculator.
					@return  ColorMap& a constant reference to the \Ref{color_map_} of {\em *this} colorCalculator
					@see     setColorMap
					@see     color_map_
					@see     ColorMap
			*/
			const ColorMap& getColorMap() const
				throw();				

			/** Visit method.
					Calculate a key with the method \Ref{calculateKey} of the given \Ref{Atom} {\em atom}
					and searches the key in the \Ref{color_map_}. If such a key exists the 
					corresponding \Ref{ColorRGBA} object will be written into the variable \Ref{color_}.
					If no such key exists the \Ref{default_color_} will be written into the variable
					\Ref{color_}.
					@param  atom the \Ref{Atom} object whose color should be retrieved
					@see    Atom
					@see    calculateKey
					@see    getColor
					@see    color_
					@see    setDefaultColor
					@see    getDefaultColor
					@see    default_color_
					@see    setColorMap
					@see    getColorMap
					@see    color_map_
			*/
			virtual void visit(Atom& atom)
				throw();

			/** Visit method.
					If the \Ref{Composite} {\em composite} is of kind \Ref{Atom} than the method
					\Ref{visit} will be called else the \Ref{default_color_} will be written into
					the variable \Ref{color_}.
					@see    visit
					@see    Composite
					@see    Atom
					@see    calculateKey
					@see    getColor
					@see    color_
					@see    setDefaultColor
					@see    getDefaultColor
					@see    default_color_
					@see    setColorMap
					@see    getColorMap
					@see    color_map_
			*/
			virtual void visit(Composite& composite)
				throw();

			/** Calculate a key to an atom.
					Calculate a key to the given \Ref{Atom} {\em atom}.
					This method can be overridden to calculate a string key of the \Ref{Atom} object.
					In this implementation the key of {\em atom} is the symbol of the element of the
					\Ref{Atom} object.
					This calculated key will be used to access the \Ref{ColorMap} to store a
					\Ref{ColorRGBA} for the given \Ref{Atom} object.
					@param  atom the \Ref{Atom} object for which a string key should be calculated
					@return String& a constant reference to a string key
					@see    visit
			*/
			virtual const String& calculateKey(const Atom& atom) const
				throw();
			//@}
			

			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dump the current state of {\em *this} colorCalculator to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} colorCalculator
					@param   depth the dumping depth
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			protected:
			
			/** @name Protected member variables
					This variables are available for access from derived classes.
			*/
			//@{
			/** Access to the colorMap.
					This variable contains all \Ref{ColorRGBA} object that are inserted with a
					certain name.
					@see   ColorMap
			*/
			ColorMap		color_map_;

			/** Access to the default color.
				  This variable contains a color that will be used if a color with a name
				  is searched in the colorMap but not found.
				  @see     ColorMap
				  @see     ColorRGBA
			*/
			ColorRGBA		default_color_;

			/** Access to the found color.
				  This variable contains the color of the color whose name was found
				  in the colorMap. 
				  @see     ColorMap
				  @see     ColorRGBA
			*/
			ColorRGBA		color_;
			//@}
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/colorCalculator.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H
