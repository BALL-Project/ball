// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorCalculator.h,v 1.13 2003/02/25 15:53:35 sturm Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H
#define BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_PTE_H
#	include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORTABLE_H
#	include <BALL/VIEW/DATATYPE/colorTable.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
    /** \ingroup ColorCalculators
     *  @{
     */							
		using VIEW::ColorTable;
		using VIEW::ColorRGBA;
		
		/** ColorCalculator class.
				The class ColorCalculator is derived from both  \link Visitor Visitor \endlink  of  \link Atom Atom \endlink 
				and  \link Composite Composite \endlink . So there are  \link visit visit \endlink  methods for both \Ref{Atom} and
				 \link Composite Composite \endlink  objects. The class ColorCalculator is used for calculating
				a  \link ColorRGBA ColorRGBA \endlink  for each given  \link Atom Atom \endlink  object. A \Ref{ColorMap} is used
				to store the colors with a given name. That name will be calculated by the method
				 \link calculateKey calculateKey \endlink  from the given object. If a name is not inserted into the 
				color map a default color	will be returned. 
				This class and their derived classes will be used by the model processors to
				calculate a color for each  \link Atom Atom \endlink  object they process.  \par
				<b>Definition:</b> BALL/MOLVIEW/FUNCTOR/colorCalculator.h
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
					to  \link ColorRGBA ColorRGBA \endlink  objects.
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
			ColorCalculator(const ColorCalculator& color_calculator)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} colorCalculator.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~ColorCalculator()
				throw();

			/** Explicit default initialization.
					Reset the state of {\em *this} colorCalculator to:
					\begin{itemize}
					  \item  {\em color_} set to red (<tt>"FF0000FF"</tt>)
					  \item  {\em default_color_} set to red (<tt>"FF0000FF"</tt>)
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
			void set(const ColorCalculator& color_calculator)
				throw();

			/** Assignment operator.
					Assign the colorCalculator {\em color_calculator} to {\em *this}
					colorCalculator.
					Calls  \link set set \endlink .
					The colors and the colorMap of {\em *this} are initialized to the colors and 
					the colorMap of the colorCalculator {\em color_calculator}. 
					@param       color_calculator the colorCalculator to be copied
					@return      ColorCalculator& constant reference of {\em *this} colorCalculator
					@see         set
			*/
			const ColorCalculator&  operator = (const ColorCalculator& color_calculator)
				throw();

			/** Copying.
					Copy {\em *this} colorCalculator to the colorCalculator 
					{\em color_calculator}.
					Calls  \link set set \endlink .
					The colors and the colorMap of {\em *this} are initialized to the colors and 
					the colorMap of the colorCalculator {\em color_calculator}. 
					@param       color_calculator the colorCalculator to be assigned to
					@see         set
			*/
			void get(ColorCalculator&  color_calculator) const
				throw();

			/** Swapping of colorCalculator's.
					Swap the colors and the colorMap of {\em *this} colorCalculator with
					the colorCalculator	{\em color_calculator}.
					Calls  \link ColorRGBA::swap ColorRGBA::swap \endlink .
					Calls  \link StringHashMap::swap StringHashMap::swap \endlink .
					@param       color_calculator the colorCalculator being swapped with {\em *this} colorCalculator 
					@see         color_
					@see         default_color_
					@see         color_map_
			*/
			void swap(ColorCalculator&  color_calculator)
				throw();
			//@}

			
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the default color.
					Change the default color of {\em *this} colorCalculator.
					If the object given to the  \link visit visit \endlink  method has a name that is not inserted
					into the  \link color_map_ color_map_ \endlink  the default color will be written into the variable
					 \link color_ color_ \endlink . So the method  \link getColor getColor \endlink  always returns a valid color.
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
					@return  ColorRGBA& a constant reference to the default  \link ColorRGBA ColorRGBA \endlink  of {\em *this} colorCalculator
					@see     setDefaultColor
					@see     default_color_
			*/
			const ColorRGBA& getDefaultColor() const
				throw();

			/** Non-mutable inspection of the chosen color.
					Access the constant reference of the chosen color of {\em *this} colorCalculator.
					@return  ColorRGBA& a constant reference to the chosen  \link ColorRGBA ColorRGBA \endlink  of {\em *this} colorCalculator
					@see     visit
					@see     color_
			*/
			const ColorRGBA& getColor() const
				throw();

			/** Change the color map.
					Change the color map of {\em *this} colorCalculator. The given  \link ColorMap ColorMap \endlink 
					{\em color_map} will be used in the  \link visit visit \endlink  method to retrieve a 
					 \link ColorRGBA ColorRGBA \endlink  for a given object.
					@param  color_map the new  \link ColorMap ColorMap \endlink  of {\em *this} colorCalculator
					@see    getColorMap
					@see    visit
					@see    ColorMap
					@see    color_map_
			*/
			void setColorMap(const ColorMap& color_map)
				throw();

			/** Non-mutable inspection of the color map.
					Access the constant reference of the color map of {\em *this}	colorCalculator.
					@return  ColorMap& a constant reference to the  \link color_map_ color_map_ \endlink  of {\em *this} colorCalculator
					@see     setColorMap
					@see     color_map_
					@see     ColorMap
			*/
			const ColorMap& getColorMap() const
				throw();				

			/** Visit method.
					Calculate a key with the method  \link calculateKey calculateKey \endlink  of the given  \link Atom Atom \endlink  {\em atom}
					and searches the key in the  \link color_map_ color_map_ \endlink . If such a key exists the 
					corresponding  \link ColorRGBA ColorRGBA \endlink  object will be written into the variable  \link color_ color_ \endlink .
					If no such key exists the  \link default_color_ default_color_ \endlink  will be written into the variable
					 \link color_ color_ \endlink .
					@param  atom the  \link Atom Atom \endlink  object whose color should be retrieved
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
			virtual void visit(Atom& atom);

			/** Visit method.
					If the  \link Composite Composite \endlink  {\em composite} is of kind  \link Atom Atom \endlink  than the method
					 \link visit visit \endlink  will be called else the  \link default_color_ default_color_ \endlink  will be written into
					the variable  \link color_ color_ \endlink .
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
			virtual void visit(Composite& composite);

			/** Calculate a key to an atom.
					Calculate a key to the given  \link Atom Atom \endlink  {\em atom}.
					This method can be overridden to calculate a string key of the  \link Atom Atom \endlink  object.
					In this implementation the key of {\em atom} is the symbol of the element of the
					 \link Atom Atom \endlink  object.
					This calculated key will be used to access the  \link ColorMap ColorMap \endlink  to store a
					 \link ColorRGBA ColorRGBA \endlink  for the given  \link Atom Atom \endlink  object.
					@param  atom the  \link Atom Atom \endlink  object for which a string key should be calculated
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
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			protected:
			
			/** @name Protected member variables
					This variables are available for access from derived classes.
			*/
			//@{
			/** Access to the colorMap.
					This variable contains all  \link ColorRGBA ColorRGBA \endlink  object that are inserted with a
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
  /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_COLORCALCULATOR_H
