// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorProcessor.h,v 1.12 2003/10/21 13:03:40 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_COLORPROCESSOR_H
#define BALL_VIEW_MODELS_COLORPROCESSOR_H

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_DATATYPE_HASHGRID_H
# include <BALL/DATATYPE/hashGrid.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
# include <BALL/DATATYPE/list.h>
#endif

namespace BALL
{
	class Atom;

	namespace VIEW
	{
		class Mesh;
				
		/** Calculating colors for given GeometricObject instances. 
				This class defines the interface and basic algorithms to colorize geometric objects, e.g. 
				Sphere, Tube, TwoColoredTube, etc.
				Derived classes implement special ways to colorize, e.g. by charge or element.
				They have to overload the method getColor(Composite*).\par
				If no color can be calculated for a GeometricObject, e.g. if it has no Composite, the default
				color is set.\par
				A bit tricky is the coloring of meshes. The goal was to give every triangle of a mesh a color
				according to its nearest atom. To do so, a threedimensional HashGrid3 is used. It is
				computed, when the ColorProcessor first encounters a Mesh in the operator ().
				For this a pointer to the HashSet with the used Composite instances exists, which is set by the 
				Representation.
		*/
		class ColorProcessor
			: public UnaryProcessor<GeometricObject*>
		{
			public:
			
			/**	@name	Type definitions
			*/
			//@{
				
			/// A HashSet with the used Composites, see also Representation
			typedef HashSet<const Composite*>  CompositeSet;

			/// a threedimensioal grid with the pointers to the atoms
			typedef HashGrid3<const Atom*>  AtomGrid;

			/// a single box in the threedimensional grid
			typedef HashGridBox3<const Atom*> AtomBox;

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
					 <b> default_color_</b> to red (<tt> "FF0000FF"</b>).
					The grid is cleared and the pointer to the CompositeSet is set to NULL.
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

			/** Calculate a color for a Composite.
			 		This method is called by the operator().
					Here it just returns the default color.
					You have to overload this operator in derived classes.
			*/
			virtual ColorRGBA getColor(const Composite* /*composite*/)
			{ return default_color_;}

			/** Set the pointer to the CompositeSet.
			 		This method is called by Representation::setColorProcessor and Representation::update.
			*/
			void setComposites(const CompositeSet* composites)
				throw();

			/// Return a pointer to the CompositeSet.
			const CompositeSet* getComposites()
				throw() { return composites_;}

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
			
			//_ Create the threedimensional grid from the CompositeSet.
			virtual void createAtomGrid_()
				throw();

			//_ Colorize the mesh with the computed grid.
			virtual void colorMeshFromGrid_(Mesh& mesh)
				throw();

			//@} 
			/** @name Protected member variables 
			*/ 
			//@{

			//_ a color that will be used if no other color can be calculated.
			ColorRGBA		default_color_;

			const 			CompositeSet* composites_;

			AtomGrid 		atom_grid_;
			bool 				atom_grid_created_;
			//@}
		};

		
		/// Base class for ColorProcessors, that interpolate between two values
		class InterpolateColorProcessor
			: public ColorProcessor
		{
			public: 

			///
			InterpolateColorProcessor();

			///
			void setMinColor(const ColorRGBA& color)
				throw();

			///
			void setMaxColor(const ColorRGBA& color)
				throw();

			///
			const ColorRGBA& getMinColor() const
				throw();
			
			///
			const ColorRGBA& getMaxColor() const
				throw();
			
			///
			void setMinMinColor(const ColorRGBA& color)
				throw();

			///
			void setMaxMaxColor(const ColorRGBA& color)
				throw();

			///
			const ColorRGBA& getMinMinColor() const
				throw();
			
			///
			const ColorRGBA& getMaxMaxColor() const
				throw();


			///
			void setMaxValue(float value)
				throw();

			///
			float getMaxValue() const
				throw();

			///
			virtual ColorRGBA interpolateColor(float value);

			protected:

			ColorRGBA min_color_,
								max_color_,
								min_min_color_,
								max_max_color_;
			float 		max_value_;
			float 		min_value_;
		};


		
} } // namespaces

#endif // BALL_VIEW_MODELS_COLORPROCESSOR_H
