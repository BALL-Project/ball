// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.h,v 1.21 2004/05/27 19:49:49 oliver Exp $
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

namespace BALL
{
	namespace VIEW
	{
		class ModelProcessor;
		class ColorProcessor;
		class GeometricObject;
		class UpdateRepresentationThread;

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
			public:

			BALL_CREATE(Representation)

			/** @name Enums
			 */
			//@{
			
			/// Properties
			enum Properties
			{
				///
				PROPERTY__HIDDEN = 0,
				
				///
				PROPERTY__ALWAYS_FRONT,
				
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
			void setDrawingPrecision(DrawingPrecision precision)
				throw();

			///
			DrawingPrecision getDrawingPrecision() const
				throw() { return drawing_precision_;}

			///
			float getSurfaceDrawingPrecision() const
				throw() { return surface_drawing_precision_;}

			///
			void setSurfaceDrawingPrecision(float precision)
				throw();

			///
			void setDrawingMode(DrawingMode mode)
				throw() { drawing_mode_ = mode;}

			///
			DrawingMode getDrawingMode() const
				throw() { return drawing_mode_;}
			
			///
			Size getTransparency() const
				throw() { return transparency_;}

			///
			void setTransparency(Size value)
				throw();
			
			///
			const GeometricObjectList& getGeometricObjects() const
				throw() { return geometric_objects_; }

			///
			GeometricObjectList& getGeometricObjects()
				throw() { return geometric_objects_; }

			///
			void insert(GeometricObject& object) 
				throw() { geometric_objects_.push_back(&object); }

			///
			const CompositeSet& getComposites() const
				throw() { return composites_; }

			///
			CompositeSet& getComposites()
				throw() { return composites_; }

			///
			const ModelProcessor* getModelProcessor() const
				throw() { return model_processor_;}

			///
			ModelProcessor* getModelProcessor()
				throw() { return model_processor_;}

			///
			void setModelProcessor(ModelProcessor* processor)
				throw();
			
			///
			ColorProcessor* getColorProcessor()
				throw() { return color_processor_;}

			///
			void setColorProcessor(ColorProcessor* processor)
				throw();

			///
			String getModelName() const
				throw();

			///
			void setModelType(ModelType type)
				throw() { model_type_ = type;}

			///
			ModelType  getModelType() const
				throw() { return model_type_;}

			///
			String getColoringName() const
				throw();

			///
			void setColoringMethod(ColoringMethod type)
				throw() { coloring_method_= type;}

			///
			ColoringMethod getColoringMethod() const
				throw() { return coloring_method_;}

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
				throw() { return model_build_time_;}

			/** Returns true, if Representation needs to be updated.
			 		Called by GeometricControl.
					Uses needs_update_.
			*/
			bool needsUpdate() const
				throw();

			/// Return true, if the Representation is currently updated by a thread
			bool updateRunning() const
				throw() { return update_running_;}

			///
			void dump(std::ostream& s, Size depth) const
				throw();

			///
			CompositesIterator begin() 
				throw() { return composites_.begin();}

			///
			CompositesConstIterator begin() const
				throw() { return composites_.begin();}
			
			///
			CompositesIterator end() 
				throw() { return composites_.end();}

			///
			CompositesConstIterator end() const
				throw() { return composites_.end();}
			
			//@}

			protected:

			/// Can be called by rebuild directly, or by UpdateRepresentationThread
			void update_(bool rebuild)
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
			GeometricObjectList geometric_objects_;

			//_
			CompositeSet 				composites_;

			//_
			PreciseTime 				model_build_time_;

			//_ set to true, if update is called, while representation is hidden
			bool 								needs_update_;

			//_ 								true, if thread is currently updateing this representation
			bool 								update_running_;

			static UpdateRepresentationThread* thread_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_REPRESENTATION_H
