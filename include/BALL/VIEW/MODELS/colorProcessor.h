// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.h,v 1.5 2003/10/16 06:28:38 oliver Exp $
//

#ifndef BALL_VIEW_MODELS_COLORPROCESSOR_H
#define BALL_VIEW_MODELS_COLORPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORTABLE_H
#	include <BALL/VIEW/DATATYPE/colorTable.h>
#endif

#ifndef BALL_COMPOSITE_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_COMPOSITE_PTE_H
# include <BALL/KERNEL/PTE.h>
#endif

namespace BALL
{
	namespace VIEW
	{
				
		/** ColorProcessor class.
				The class ColorProcessor is used for calculating a ColorRGBA for each given GeometricObject. 
				A ColorMap is used to map colors to names. These names will be calculated by the method
				calculateKey from the given object. If a name is not inserted into the 
				color map a default color	will be returned. 
		*/
		class ColorProcessor
			: public UnaryProcessor<GeometricObject*>
		{
			public:
			
			/**	@name	Type definitions
			*/
			//@{
				
			/**	ColorMap type.
					Define the type of a ColorMap as a StringHashMap to ColorRGBA objects.
					\see   StringHashMap
					\see   ColorRGBA
			*/
			typedef StringHashMap<ColorRGBA>	ColorMap;

			//@} 
			/**	@name	Constructors 
			*/	
			//@{

			/** Default Constructor.
			*/
			ColorProcessor()
				throw();

			/** Copy constructor.
			*/
			ColorProcessor(const ColorProcessor& color_calculator)
				throw();

			//@} 
			/** @name Destructors 
			*/ 
			//@{

			/** Destructor.
			*/
			virtual ~ColorProcessor()
				throw();

			/** Explicit default initialization.
					Reset the state of this ColorProcessor to:
					  -  <b> color_</b> set to red (<tt> "FF0000FF"</b>)
					  -  <b> default_color_</b> set to red (<tt> "FF0000FF"</b>)
					  -  <b> color_map_</b> is cleared
					\par
			*/
			virtual void clear()
				throw();

			//@} /**	@name	Asignment */ //@{

			/** Assignment.
			*/
			void set(const ColorProcessor& color_calculator)
				throw();

			/** Assignment operator.
					Calls set.
			*/
			const ColorProcessor&  operator = (const ColorProcessor& color_calculator)
				throw();

			/** Swapping of ColorProcessor's.
			*/
			void swap(ColorProcessor&  color_calculator)
				throw();

			//@} /**	@name	Accessors: inspectors and mutators */ //@{

			/** Change the default color.
					If the object given to the visit method has a name that is not inserted
					into the color_map_ the default color will be written into the variable
					color_. 
			*/
			void setDefaultColor(const ColorRGBA& color)
				throw() {default_color_ = color;}

			/** Non-mutable inspection of the default color.
					\see     setDefaultColor
			*/
			const ColorRGBA& getDefaultColor() const
				throw() {return default_color_;}

			/** Non-mutable inspection of the chosen color.
			*/
			const ColorRGBA& getColor() const
				throw() {return color_;}

			/** Change the color map.
					The given ColorMap <b> color_map will be used in the visit 
					method to retrieve a ColorRGBA for a given object.
					\param  color_map the new ColorMap of this ColorProcessor
					\see    getColorMap
			*/
			void setColorMap(const ColorMap& color_map)
				throw() {color_map_ = color_map;}

			/** Non-mutable inspection of the color map.
					\see     setColorMap
			*/
			const ColorMap& getColorMap() const
				throw() {return color_map_;}

			/** Visit method.
					Calculate a key with the method calculateKey of the given Atom <b> atom
					and searches the key in the color_map_. If such a key exists the 
					corresponding ColorRGBA object will be written into the variable color_.
					If no such key exists the default_color_ will be written into the variable
					color_.
					\see    calculateKey
					\see    getColor
			*/
			virtual Processor::Result operator() (GeometricObject*& object);

			///
			virtual Processor::Result operator() (const Composite* composite);

			///
			void getColor(const String& key); 

			//@} 
			/**	@name	debuggers and diagnostics 
			*/ 
			//@{

			/** Internal value dump.
					Dump the current state of this ColorProcessor to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this ColorProcessor
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			protected:
			
			//@} 
			/** @name Protected member variables 
			*/ 
			//@{

			/** Access to the colorMap.
					This variable contains all ColorRGBA object that are inserted with a certain name.
					\see   ColorMap
			*/
			ColorMap		color_map_;

			/** Access to the default color.
				  This variable contains a color that will be used if a color with a name
				  is searched in the colorMap but not found.
				  \see     ColorMap
				  \see     ColorRGBA
			*/
			ColorRGBA		default_color_;

			/** Access to the found color.
				  This variable contains the color of the color whose name was found in the colorMap. 
				  \see     ColorMap
				  \see     ColorRGBA
			*/
			ColorRGBA		color_;
			//@}
		};

} } // namespaces

#endif // BALL_VIEW_MODELS_COLORPROCESSOR_H
