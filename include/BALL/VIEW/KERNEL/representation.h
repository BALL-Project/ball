// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef  BALL_VIEW_KERNEL_REPRESENTATION_H
#define  BALL_VIEW_KERNEL_REPRESENTATION_H

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_CONCEPT_MOLECULARINFORMATION_H
#	include <BALL/CONCEPT/molecularInformation.h>
#endif

#ifndef BALL_VIEW_KERNEL_MODELINFORMATION_H
#	include <BALL/VIEW/KERNEL/modelInformation.h>
#endif

// next two defines need to be included in header file, because of iC file
#ifndef BALL_VIEW_MODELS_MODELPROCESSOR_H
# include <BALL/VIEW/MODELS/modelProcessor.h>
#endif

#ifndef BALL_VIEW_MODELS_COLORPROCESSOR_H
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
		class RepresentationManager;

		/** Representation
		 		A Representation is a collection of geometric objects for a group of 
				composites and a given model, for example a surface.
				An instance of Representation manages the memory for the geometric objects, the ColorProcessor and 
				the ModelProcessor. So when a Representation is destroyed, so are its geometric objects
				and processors. 
				It stores the drawing precision and mode.
				\ingroup ViewKernelGeometricPrimitives
		*/
		class BALL_VIEW_EXPORT Representation
			: public PropertyManager
		{
			friend class UpdateRepresentationThread;
			friend class RepresentationManager;
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
				
			//@}
			/**	@name	Constructors and Destuctor
			*/	
			//@{

			/** Default Constructor
			*/
			Representation();

			///
			Representation(ModelType model_type,
										 DrawingPrecision drawing_precision,
										 DrawingMode drawing_mode);

			/** Copy constructor
			*/
			Representation(const Representation& representation);

			/** Destructor
			*/
			virtual ~Representation();

			//@}
			/**	@name	Predicats and Accessors
			*/	
			//@{
			
			///
			const Representation& operator = (const Representation& rep);
			
			///
			void clear();

			///
			void setHidden(bool state) ;

			///
			bool isHidden() const;

			///
			void setDrawingPrecision(DrawingPrecision precision);

			///
			DrawingPrecision getDrawingPrecision() const;

			///
			float getSurfaceDrawingPrecision() const;

			///
			void setSurfaceDrawingPrecision(float precision);

			///
			void setDrawingMode(DrawingMode mode);

			///
			DrawingMode getDrawingMode() const;
			
			/// get transparency (0 - 255)
			Size getTransparency() const;

			/// set transparency (0 - 255)
			void setTransparency(Size value);
			
			///
			const GeometricObjectList& getGeometricObjects() const;

			///
			GeometricObjectList& getGeometricObjects();

			///
			void setGeometricObjects(GeometricObjectList& gol) 
				{ geometric_objects_ = gol;}

			///
			void insert(GeometricObject& object) ;

			///
			const std::list<const Composite*>& getComposites() const
				{ return composites_;}

			///
			void setComposites(const std::list<const Composite*>& composites);

			///
			void setComposite(const Composite* composite);

			///
			const ModelProcessor* getModelProcessor() const;

			///
			ModelProcessor* getModelProcessor();

			///
			void setModelProcessor(ModelProcessor* processor);
			
			///
			ColorProcessor* getColorProcessor();

			///
			void setColorProcessor(ColorProcessor* processor);

			///
			void setModelType(ModelType type);

			///
			ModelType getModelType() const;

			///
			String getName() const;

			///
			void setName(const String& name);

			/// Returns a human-readable, potentially abbreviated string describing the molecular entity this representation belongs to
			String getCompositeName() const;

			///
			void setColoringMethod(ColoringMethod type);

			///
			ColoringMethod getColoringMethod() const;

			///
			void enableModelUpdate(bool state);

			///
			void enableColoringUpdate(bool state) { coloring_update_enabled_ = state;}

			///
			bool modelUpdateEnabled() const { return model_update_enabled_;}

			///
			bool coloringUpdateEnabled() const { return coloring_update_enabled_;}

			/// Get a descpription string (nr triangles and geometric objects, transparency, mode)
			String getProperties() const;
			
			/// Check if drawing mode, transparency and drawing precision have reasonable values.
			bool isValid() const;

			/** Apply ModelProcessor (if rebuild) and ColorProcessor.
			 		The usage of these processors can be disabled, either by setting a NULL-pointer
					accordingly or call enableColoringUpdate(false) and enableModelUpdate(false).
			*/
			void update(bool rebuild);

			/// Clear and destroy all stored GeometricObject.
			void clearGeometricObjects();

			/** Get the time when the model was builded
			 		This is needed for updating the Representation, after
					the vector of atom attributes was modified.
					@see Atom::getAttributesModificationTime
			*/
			const PreciseTime& getModelBuildTime() const;

			/** Returns true, if Representation needs to be updated.
			 		Called by GeometricControl.
					Uses needs_update_.
			*/
			bool needsUpdate() const;

			///
			void setNeedsUpdate(bool state=true);

			/// Dum to ostream for debugging
			void dump(std::ostream& s = std::cout, Size depth = 0) const;

			/// Get a String containing all settings for Usage in project files.
			String toString() const;

			/// Set a custom ModelInformation e.g. when new models were added external of the library.
			void setModelInformation(const ModelInformation& mi);

			///
			const ModelInformation& getModelInformation() const;

			/// 
			bool operator == (const Representation& object) const;

		
			/// Needed for MSVC
			bool operator < (const Representation& object) const
				{ return this < &object;}

			//@}

			protected:

			/** Wrapper method for multithreading.
			 		Can be called by update() directly, or by the RepresentationManager' s 
					UpdateRepresentationThread.
			*/
			void update_();

			// Create a hashmap with the numerical position of every composite in its root Composite. 
			// Needed for toString().
			void collectRecursive_(const Composite& c, HashMap<const Composite*, Position>& hashmap) const;

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
			std::list<const Composite*> composites_;

			//_
			PreciseTime 				model_build_time_;

			//_ set to true, if update is called, while representation is hidden
			bool 								needs_update_;

			//_ true means the ModelProcessor will be applied in the next update
			bool 								rebuild_;

			//_ true means the ColorProcessor was changed since the last update call, so apply it!
			bool 								changed_color_processor_;

			//_
			bool 								hidden_;

			//_
			GeometricObjectList geometric_objects_;

			//_
			bool 								model_update_enabled_;

			//_
			bool 								coloring_update_enabled_;

			String 							name_;

			//_ 							  used for getName()
			static 							MolecularInformation information_;
			
			//_ 							  used for getName()
			static 							ModelInformation model_information_;

			const ModelInformation* custom_model_information_;
		};

		///
		typedef std::list<Representation*> RepresentationList;

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/representation.iC>
#	endif

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_REPRESENTATION_H
