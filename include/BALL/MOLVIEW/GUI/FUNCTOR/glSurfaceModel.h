// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glSurfaceModel.h,v 1.9 2003/03/03 08:17:30 sturm Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLSURFACEMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLSURFACEMODEL_H

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
    /** \ingroup MolviewGuiFunctor
     *  @{
     */
		/** AddGLSurfaceModel class.
				The class AddGLSurfaceModel is derived from the class 
				 \link SurfaceModel SurfaceModel \endlink  and extents this class by overriding the creation
				method used for creating the  \link Surface Surface \endlink . This new primitive contains
				OpenGL implementation to generate the graphical visualization of the surface.
				This class has the same functionality as its base class  \link AddSurfaceModel AddSurfaceModel \endlink .  \par
				<b>Definition:</b> BALL/MOLVIEW/GUI/FUNCTOR/glSurfaceModel.h
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
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em add_gl_surface_model}
					@return      AddGLSurfaceModel new constructed addGLSurfaceModel copied from {\em add_gl_surface_model}
					@see         AddSurfaceModel
			*/
			AddGLSurfaceModel(const AddGLSurfaceModel& add_gl_surface_model, bool deep = true)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addGLSurfaceModel.
					Calls  \link AddSurfaceModel::destroy AddSurfaceModel::destroy \endlink .
					@see  AddSurfaceModel
			*/
			virtual ~AddGLSurfaceModel()
				throw();

			protected:

			//@}
			/** @name Creation method
					This method creates primitives with OpenGL implementation used
					for generating the graphical representation of the shapes they represent.
					This method is the overridden method of the base class 
					 \link AddSurfaceModel AddSurfaceModel \endlink .
					@see  AddSurfaceModel
			*/
			//@{
			
			/** Creates a mesh.
					Creates a  \link GLMesh GLMesh \endlink  object and returns it as  \link Mesh Mesh \endlink .
					This overridden method of the class  \link AddSurfaceModel AddSurfaceModel \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create a mesh object
					for the start  \link Composite Composite \endlink  object.
					@see  GLMesh
					@see  Mesh
					@see  AddSurfaceModel
			*/
			virtual Mesh* createMesh_();
			//@}
		};
     /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLSURFACEMODEL_H
