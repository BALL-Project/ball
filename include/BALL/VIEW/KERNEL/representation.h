// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.h,v 1.28 2004/12/13 16:05:25 amoll Exp $
//

#ifndef  BALL_VIEW_KERNEL_REPRESENTATION_H
#define  BALL_VIEW_KERNEL_REPRESENTATION_H

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODELPROCESSOR_H
# include <BALL/VIEW/MODELS/modelProcessor.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLORPROCESSOR_H
# include <BALL/VIEW/MODELS/colorProcessor.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class ModelProcessor;
		class ColorProcessor;
		class GeometricObject;
		class UpdateRepresentationThread;
		class PrimitiveManager;

		/** Representation
		 		A Representation is a collection of geometric objects for a group of 
				composites and a given model, for example a surface.
				An instance of Representation manages the memory for the geometric objects, the ColorProcessor and 
				the ModelProcessor. So when a Representation is destroyed, so are its geometric objects
				and processors. 
				It stores the drawing precision and mode.
				\ingroup ViewKernelGeometricPrimitives
		*/
		class BALL_EXPORT Representation
			: public PropertyManager
		{
			friend class UpdateRepresentationThread;
			friend class PrimitiveManager;
			public:

			BALL_CREATE(Representation)

			/** @name Enums
			 */
			//@{
			
			/// Properties
			enum Properties
			{
				///
				PROPERTY__ALWAYS_FRONT = 0,
				
				///
				PROPERTY__IS_COORDINATE_SYSTEM
			};
				
			/** @name Type definitions
			 */
			//@{
			/// 
			typedef List<GeometricObject*> 		 GeometricObjectList;

			///
			typedef HashSet<const Composite*>  CompositeSet;

			///
			typedef CompositeSet::Iterator CompositesIterator;

			///
			typedef CompositeSet::ConstIterator CompositesConstIterator;

			//@}
			/**	@name	Constructors and Destuctor
			*/	
			//@{

			/** Default Constructor
			*/
			Representation()
				throw();

			///
			Representation(ModelType model_type,
										 DrawingPrecision drawing_precision,
										 DrawingMode drawing_mode)
				throw();

			///
			Representation(const CompositeSet& composites, 
										 ModelProcessor* rep_processor)
				throw();

			///
			Representation(const GeometricObjectList& object_list)
				throw();

			/** Copy constructor
			*/
			Representation(const Representation& representation)
				throw();

			/** Destructor
			*/
			virtual ~Representation()
				throw();

			//@}
			/**	@name	Predicats and Accessors
			*/	
			//@{
			
			///
			const Representation& operator = (const Representation& rep)
				throw();
			
			///
			void clear()
				throw();

			///
			void setHidden(bool state) 
				throw();

			///
			bool isHidden() const
				throw();

			///
			void setDrawingPrecision(DrawingPrecision precision)
				throw();

			///
			DrawingPrecision getDrawingPrecision() const
				throw();

			///
			float getSurfaceDrawingPrecision() const
				throw();

			///
			void setSurfaceDrawingPrecision(float precision)
				throw();

			///
			void setDrawingMode(DrawingMode mode)
				throw();

			///
			DrawingMode getDrawingMode() const
				throw();
			
			///
			Size getTransparency() const
				throw();

			///
			void setTransparency(Size value)
				throw();
			
			///
			const GeometricObjectList& getGeometricObjects() const
				throw();

			///
			GeometricObjectList& getGeometricObjects()
				throw();

			///
			void insert(GeometricObject& object) 
				throw();

			///
			const CompositeSet& getComposites() const
				throw();

			///
			CompositeSet& getComposites()
				throw();

			///
			const ModelProcessor* getModelProcessor() const
				throw();

			///
			ModelProcessor* getModelProcessor()
				throw();

			///
			void setModelProcessor(ModelProcessor* processor)
				throw();
			
			///
			ColorProcessor* getColorProcessor()
				throw();

			///
			void setColorProcessor(ColorProcessor* processor)
				throw();

			///
			String getModelName() const
				throw();

			///
			void setModelType(ModelType type)
				throw();

			///
			ModelType getModelType() const
				throw();

			///
			String getColoringName() const
				throw();

			///
			void setColoringMethod(ColoringMethod type)
				throw();

			///
			ColoringMethod getColoringMethod() const
				throw();

			///
			String getProperties() const
				throw();
			
			///
			bool isValid() const
				throw();

			///
			void update(bool rebuild)
				throw();

			///
			void clearGeometricObjects()
				throw();

			/** Get the time when the model was builded
			 		This is needed for updating the Representation, after
					the vector of atom attributes was modified.
					@see Atom::getAttributesModificationTime
			*/
			const PreciseTime& getModelBuildTime() const
				throw();

			/** Returns true, if Representation needs to be updated.
			 		Called by GeometricControl.
					Uses needs_update_.
			*/
			bool needsUpdate() const
				throw();

			///
			void dump(std::ostream& s, Size depth) const
				throw();

			///
			String toString() const
				throw();

			///
			CompositesIterator begin() 
				throw();

			///
			CompositesConstIterator begin() const
				throw();
			
			///
			CompositesIterator end() 
				throw();

			///
			CompositesConstIterator end() const
				throw();

			//@}

			protected:

			/** Wrapper method for multithreading.
			 		Can be called by update() directly, or by the PrimitiveManager' s 
					UpdateRepresentationThread.
			*/
			void update_()
				throw();

			void collectRecursive_(const Composite& c, HashMap<const Composite*, Position>& hashmap) const
				throw();

			//_
			DrawingMode 				drawing_mode_;

			//_
			DrawingPrecision 		drawing_precision_;

			//_
			float 							surface_drawing_precision_;

			//_
			ModelType 					model_type_;

			//_
			ColoringMethod 			coloring_method_;

			//_
			Size  							transparency_;

			//_
			ModelProcessor* 		model_processor_;

			//_
			ColorProcessor* 		color_processor_;

			//_
			CompositeSet 				composites_;

			//_
			PreciseTime 				model_build_time_;

			//_ set to true, if update is called, while representation is hidden
			bool 								needs_update_;

			//_ true means the ModelProcessor will be applied in the next update
			bool 								rebuild_;

			//_
			bool 								hidden_;

			// prevent usage of geometric_objects_ in derived classes
			private:

			//_
			GeometricObjectList geometric_objects_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/representation.iC>
#	endif

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_REPRESENTATION_H
