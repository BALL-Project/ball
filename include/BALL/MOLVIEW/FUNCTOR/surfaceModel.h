// $Id: surfaceModel.h,v 1.4 2001/07/15 18:49:19 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H
#define BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_KERNEL_FOREACH_H
#	include <BALL/KERNEL/forEach.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#	include <BALL/VIEW/PRIMITIV/mesh.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
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
				{\bf Framework:} BALL/MOLVIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/surfaceModel.h}\\ \\
				The class AddSurfaceModel is a model processor that creates a surface model
				of the given start \Ref{Composite} object. If the start composite object
				is of kind \Ref{System} than the \Ref{SurfaceProcessor} is applied to it.
				A \Ref{Mesh} object is created and filled with the method \Ref{getSurface}
				from the class \Ref{SurfaceProcessor}.
				The derived class \Ref{AddGLSurfaceModel} uses the \Ref{GLMesh} with
				openGL implementation.
				For information about the processor concept see \Ref{Processor} in tbe BALL
				documentation.
				@see     Mesh
				@see     SurfaceProcessor
				@memo    AddSurfaceModel class (BALL MOLVIEW functor framework)
				@author  $Author: oliver $
				@version $Revision: 1.4 $
				@date    $Date: 2001/07/15 18:49:19 $
		*/
		class AddSurfaceModel: public BaseModelProcessor
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
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em add_surface_model}
					@return      AddSurfaceModel new constructed addSurfaceModel copied from {\em add_surface_model}
					@see         BaseModelProcessor
			*/
			AddSurfaceModel
				(const AddSurfaceModel& add_surface_model, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addSurfaceModel.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~AddSurfaceModel()
				throw();

			/** Explicit default initialization.
					Clears all internal structures.
					Calls \Ref{BaseModelProcessor::clear}.
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
					Calls \Ref{BaseModelProcessor::start}.
					@return bool {\tt true} if the start of {\em *this} addSurfaceModel was successful, {\tt false} otherwise
					@see    operator()
					@see    BaseModelProcessor
			*/
			virtual bool start();
			
			/** Finish method.
					This method will be internally called from the processor mechanism if the processor
					has finished processing the \Ref{Composite} tree.
					If the start \Ref{Composite} object determined by the \Ref{operator()} method
					is of kind \Ref{System} than the \Ref{SurfaceProcessor} will be applied to it.
					The created \Ref{Mesh} object will be filled with the method \Ref{getSurface}
					from the \Ref{SurfaceProcessor}.
					@return bool {\tt true} if the finish of {\em *this} addSurfaceModel was successful, {\tt false} otherwise
					@exeception OutOfMemory thrown if the memory allocation for the \Ref{Mesh} object failed
					@see    Mesh
					@see    operator()
					@see    Composite
					@see    System
					@see    SurfaceProcessor
			*/
			virtual bool finish();
			
			/**	Operator method.
					This method stores only the start \Ref{Composite} object. In the \Ref{finish}
					method for this stored start composite a \Ref{Mesh} object will be created.
					@param  composite the \Ref{Composite} object that will be processed
					@return Processor::Result the result of {\em *this} addSurfaceModel
					@see    start
					@see    finish
					@see    Mesh
					@see    Composite
			*/
			virtual Processor::Result operator() (Composite& composite);
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal value dump.
					Dump the current state of {\em *this} addSurfaceModel to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{BaseModelProcessor::dump}.
					@param   s output stream where to output the state of {\em *this} addSurfaceModel
					@param   depth the dumping depth
					@see     BaseModelProcessor
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			protected:

			/** @name Creation method
					This method creates a primitives without OpenGL implementation.
					This method is overridden by the class \Ref{AddGLSurfaceModel}
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  AddGLSurfaceModel
			*/
			//@{
			/** Create a mesh.
					Create a \Ref{Mesh} object.
					This method is overridden by the class \Ref{AddGLSurfaceModel} to
					create a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create a mesh
					for the start \Ref{Composite} object.
					@see  Mesh
					@see  AddGLSurfaceModel
					@see  Composite
			*/
			virtual Mesh* createMesh_();
			//@}

		  private:

			bool get_composite_;

			Composite* start_composite_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/surfaceModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H
