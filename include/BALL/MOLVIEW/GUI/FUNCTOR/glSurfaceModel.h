// $Id: glSurfaceModel.h,v 1.5 2001/07/16 14:49:28 amoll Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLSURFACEMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLSURFACEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLMESH_H
#	include <BALL/VIEW/GUI/PRIMITIV/glmesh.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_SURFACEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/surfaceModel.h>
#endif



namespace BALL
{

	namespace MOLVIEW
	{

		/** AddGLSurfaceModel class.
				The class AddGLSurfaceModel is derived from the class 
				\Ref{SurfaceModel} and extents this class by overriding the creation
				method used for creating the \Ref{Surface}. This new primitive contains
				OpenGL implementation to generate the graphical visualization of the surface.
				This class has the same functionality as its base class \Ref{AddSurfaceModel}. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/FUNCTOR/glSurfaceModel.h}
		*/
		class AddGLSurfaceModel: public AddSurfaceModel
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new addGLSurfaceModel.
					@return      AddGLSurfaceModel new constructed addGLSurfaceModel
					@see         AddSurfaceModel
			*/
			AddGLSurfaceModel()
				throw();

			/** Copy constructor.
					Constructs new addGLSurfaceModel by copying the addGLSurfaceModel
					{\em add_gl_surface_model}. Initializes the state of {\em this} addGLSurfaceModel 
					to the state of {\em add_gl_surface_model}.
					@param       add_gl_surface_model the addGLSurfaceModel to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em add_gl_surface_model}
					@return      AddGLSurfaceModel new constructed addGLSurfaceModel copied from {\em add_gl_surface_model}
					@see         AddSurfaceModel
			*/
			AddGLSurfaceModel
				(const AddGLSurfaceModel& add_gl_surface_model, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addGLSurfaceModel.
					Calls \Ref{AddSurfaceModel::destroy}.
					@see  AddSurfaceModel
			*/
			virtual ~AddGLSurfaceModel()
				throw();
			//@}


			protected:

			/** @name Creation method
					This method creates primitives with OpenGL implementation used
					for generating the graphical representation of the shapes they
					represent.
					This method is the overridden method of the base class 
					\Ref{AddSurfaceModel}.
					@see  AddSurfaceModel
			*/
			//@{
			/** Creates a mesh.
					Creates a \Ref{GLMesh} object and returns it as \Ref{Mesh}.
					This overridden method of the class \Ref{AddSurfaceModel} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create a mesh object
					for the start \Ref{Composite} object.
					@see  GLMesh
					@see  Mesh
					@see  AddSurfaceModel
			*/
			virtual Mesh* createMesh_();
			//@}
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/glSurfaceModel.iC>
#			endif
		
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLSURFACEMODEL_H
