// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceModel.h,v 1.11 2003/02/21 16:05:03 anhi Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#	include <BALL/VIEW/PRIMITIV/mesh.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModel.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{

		using VIEW::Mesh;

		/** AddSurfaceModel class.
				The class AddSurfaceModel is a model processor that creates a surface model
				of the given start  \link Composite Composite \endlink  object. 
				A  \link Mesh Mesh \endlink  object is created and filled with the method  \link getSurface getSurface \endlink 
				from the class  \link SurfaceProcessor SurfaceProcessor \endlink .
				The derived class  \link AddGLSurfaceModel AddGLSurfaceModel \endlink  uses the  \link GLMesh GLMesh \endlink  with openGL implementation.
				For information about the processor concept see  \link Processor Processor \endlink  in tbe BALL documentation.  \par
				<b>Definition:</b> BALL/MOLVIEW/FUNCTOR/surfaceModel.h
				@see     Mesh
				@see     SurfaceProcessor
		*/
		class AddSurfaceModel
			: public BaseModelProcessor,
				public UnaryProcessor<Composite*>
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new addSurfaceModel.
					@return      AddSurfaceModel new constructed addSurfaceModel
					@see         BaseModelProcessor
			*/
			AddSurfaceModel()
				throw();

			/** Copy constructor.
					Construct new addSurfaceModel by copying the addSurfaceModel
					{\em add_surface_model}.
					@param       add_surface_model the addSurfaceModel to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em add_surface_model}
					@return      AddSurfaceModel new constructed addSurfaceModel copied from {\em add_surface_model}
					@see         BaseModelProcessor
			*/
			AddSurfaceModel(const AddSurfaceModel& add_surface_model, bool deep = true)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addSurfaceModel.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~AddSurfaceModel()
				throw();

			/** Explicit default initialization.
					Clears all internal structures.
					Calls  \link BaseModelProcessor::clear BaseModelProcessor::clear \endlink .
					@see  BaseModelProcessor
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name Processor specific methods
			*/
			//@{
			
			/** Start method.
					Clears all internal structures.
					Calls  \link BaseModelProcessor::start BaseModelProcessor::start \endlink .
					@return bool <tt>true</tt> if the start of {\em *this} addSurfaceModel was successful, <tt>false</tt> otherwise
					@see    operator()
					@see    BaseModelProcessor
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the  \link Composite Composite \endlink  tree.
					The created  \link Mesh Mesh \endlink  object will be filled with the method  \link getSurface getSurface \endlink 
					from the  \link SurfaceProcessor SurfaceProcessor \endlink .
					@return bool <tt>true</tt> if the finish of {\em *this} addSurfaceModel was successful, <tt>false</tt> otherwise
					@exeception OutOfMemory thrown if the memory allocation for the  \link Mesh Mesh \endlink  object failed
					@see    Mesh
					@see    operator()
					@see    Composite
					@see    SurfaceProcessor
			*/
			virtual bool finish();
			
			/**	Operator method.
					This method stores only the start  \link Composite Composite \endlink  object. In the  \link finish finish \endlink 
					method for this stored start composite a  \link Mesh Mesh \endlink  object will be created.
					@param  composite the  \link Composite Composite \endlink  object that will be processed
					@return Processor::Result the result of {\em *this} addSurfaceModel
					@see    start
					@see    finish
					@see    Mesh
					@see    Composite
			*/
			virtual Processor::Result operator() (Composite& composite);

			///
			virtual Processor::Result operator() (Composite*& composite)
			{ return operator() (*composite);}

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{
			
			/** Internal value dump.
					Dump the current state of {\em *this} addSurfaceModel to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link BaseModelProcessor::dump BaseModelProcessor::dump \endlink .
					@param   s output stream where to output the state of {\em *this} addSurfaceModel
					@param   depth the dumping depth
					@see     BaseModelProcessor
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			protected:

			//@}
			/** @name Creation method
					This method creates a primitives without OpenGL implementation.
					This method is overridden by the class  \link AddGLSurfaceModel AddGLSurfaceModel \endlink 
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  AddGLSurfaceModel
			*/
			//@{
			
			/** Create a mesh.
					Create a  \link Mesh Mesh \endlink  object.
					This method is overridden by the class  \link AddGLSurfaceModel AddGLSurfaceModel \endlink  to
					create a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create a mesh
					for the start  \link Composite Composite \endlink  object.
					@see  Mesh
					@see  AddGLSurfaceModel
					@see  Composite
			*/
			virtual Mesh* createMesh_();

			//@}

		  private:

			bool 							get_composite_;

			Composite* 				start_composite_;

			HashSet<Atom*> 		atoms_;
		};

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H
