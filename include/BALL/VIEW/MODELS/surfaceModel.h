// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceModel.h,v 1.4 2003/09/01 10:27:42 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_SURFACEMODEL_H
#define BALL_VIEW_MODELS_SURFACEMODEL_H

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_VIEW_PRIMITIVES_MESH_H
#	include <BALL/VIEW/PRIMITIVES/mesh.h>
#endif

#ifndef BALL_VIEW_MODELS_MODELPROCESSOR_H
#	include <BALL/VIEW/MODELS/modelProcessor.h>
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
				\see     Mesh
				\see     SurfaceProcessor
				\ingroup  ViewModels
		*/
		class AddSurfaceModel
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

			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{
			
			/** Start method.
					Clears all internal structures.
					Calls ModelProcessor::start.
					\return bool true if the start was successful
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism when the processor
					has finished processing the Composite tree.
					\return bool true if the finish was successful
					@exeception OutOfMemory thrown if the memory allocation for the Mesh object failed
					\see    Mesh
			*/
			virtual bool finish();
			
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

		  private:

			bool 							get_composite_;

			Composite* 				start_composite_;

			HashSet<Atom*> 		atoms_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_SURFACEMODEL_H
