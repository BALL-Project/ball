// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceModel.h,v 1.13 2005/02/06 20:57:06 oliver Exp $
//

#ifndef BALL_VIEW_MODELS_SURFACEMODEL_H
#define BALL_VIEW_MODELS_SURFACEMODEL_H

#ifndef BALL_VIEW_MODELS_MODELPROCESSOR_H
#	include <BALL/VIEW/MODELS/modelProcessor.h>
#endif

#ifndef BALL_STRUCTURE_SURFACEPROCESSOR_H
#	include <BALL/STRUCTURE/surfaceProcessor.h>
#endif

namespace BALL
{
	class Atom;

	namespace VIEW
	{
		/** AddSurfaceModel class.
				The class AddSurfaceModel is a model processor that creates a surface model
				of the given Composite object. 
				A Mesh object is created and filled with the method SurfaceProcessor::getSurface.
				The surface can be either a solvent excluded surface (SES, default) or the
				solvent accesible surface (SAS). See SurfaceProcessor for details.
				\see     Mesh
				\see     SurfaceProcessor
				\ingroup  ViewModels
		*/
		class BALL_EXPORT AddSurfaceModel
			: public ModelProcessor
		{
			public:

			/**	@name	Constructors and Destructor
			*/	
			//@{

			/// Default Constructor.
			AddSurfaceModel()
				throw();

			/// Copy constructor.
			AddSurfaceModel(const AddSurfaceModel& add_surface_model)
				throw();

			/// Destructor.
			virtual ~AddSurfaceModel()
				throw();

			/** Explicit default initialization.
					Calls ModelProcessor::clear.
			*/
			virtual void clear()
				throw();

			///
			virtual void clearComposites()
				throw();


			//@} 
			/**	@name Accessors 
			*/	
			//{
			
			/// Return the surface type (SES/SAS)
			SurfaceProcessor::SurfaceType getType() const { return type_; }
			
			/// Set the surface type (SES/SAS)
			void setType(SurfaceProcessor::SurfaceType type) { type_ = type; }
			
			//}
			/**	@name Processor specific methods 
			*/ 
			//@{
			
			/**	Operator method.
					This method stores all Composite objects. In the finish()
					method for all stored start composite a Mesh object will be created.
					The resulting Representation is named after the first processed Composite.
					\param  composite the Composite object that will be processed
					\return Processor::Result the result 
			*/
			virtual Processor::Result operator() (Composite& composite);

			//@} 
			/**	@name	debuggers and diagnostics 
			*/ 
			//@{
			
			/** Internal value dump.
					Dump the current state to the output ostream with a given dumping depth.
					Calls ModelProcessor::dump.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			///
			void setProbeRadius(float radius) 
				throw() { probe_radius_ = radius;}

			///
			float getProbeRadius() const
				throw() { return probe_radius_;}
			
			/// 
			virtual bool createGeometricObjects()
				throw();
		
		  private:

			HashSet<Atom*> 		atoms_;

			SurfaceProcessor::SurfaceType type_;

			float 						probe_radius_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_SURFACEMODEL_H
