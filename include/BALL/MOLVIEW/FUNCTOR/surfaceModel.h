// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceModel.h,v 1.17 2003/08/26 08:04:37 oliver Exp $
//

#ifndef BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#	include <BALL/VIEW/PRIMITIV/mesh.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/molecularModel.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
		using VIEW::Mesh;

		/** AddSurfaceModel class.
				The class AddSurfaceModel is a model processor that creates a surface model
				of the given Composite object. 
				A Mesh object is created and filled with the method SurfaceProcessor::getSurface.
				\see     Mesh
				\see     SurfaceProcessor
				\ingroup  MolviewFunctorsModels
		*/
		class AddSurfaceModel
			: public MolecularModelProcessor
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
					Calls MolecularModelProcessor::clear.
			*/
			virtual void clear()
				throw();

			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{
			
			/** Start method.
					Clears all internal structures.
					Calls MolecularModelProcessor::start.
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
					Calls MolecularModelProcessor::dump.
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

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H
