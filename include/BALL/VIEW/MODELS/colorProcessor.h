// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.h,v 1.6 2003/10/17 16:17:23 amoll Exp $
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
			virtual ~ColorProcessor();

			/** Explicit default initialization.
					Reset the state of the
					 <b> default_color_</b> to red (<tt> "FF0000FF"</b>)
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
			*/
			void setDefaultColor(const ColorRGBA& color)
				throw() {default_color_ = color;}

			/** Non-mutable inspection of the default color.
			*/
			const ColorRGBA& getDefaultColor() const
				throw() {return default_color_;}

			/** Calculate a color for a GeometricObject.
			*/
			virtual Processor::Result operator() (GeometricObject*& object);

			///
			virtual ColorRGBA getColor(const Composite* /*composite*/)
			{ return default_color_;}

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

			/** Access to the default color.
				  This variable contains a color that will be used if no other color can be calculated.
			*/
			ColorRGBA		default_color_;

			//@}
		};

} } // namespaces

#endif // BALL_VIEW_MODELS_COLORPROCESSOR_H
