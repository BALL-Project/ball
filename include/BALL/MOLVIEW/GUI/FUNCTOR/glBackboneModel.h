// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glBackboneModel.h,v 1.10 2003/03/03 14:18:06 anhi Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLBACKBONEMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLBACKBONEMODEL_H

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLBACKBONE_H
#	include <BALL/MOLVIEW/GUI/PRIMITIV/glBackbone.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BACKBONEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/backboneModel.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
    /**  \addtogroup  MolviewGuiFunctor
     *  @{
     */
		/** AddGLBackboneModel class.
				The class AddGLBackboneModel is derived from the class 
				 \link BackboneModel BackboneModel \endlink  and extents this class by overriding the creation
				method used for creating the  \link Backbone Backbone \endlink . This new primitive contains
				OpenGL implementation to generate the graphical visualization of the backbone.
				This class has the same functionality as its base class  \link AddBackboneModel AddBackboneModel \endlink .  \par
				<b>Definition:</b> BALL/MOLVIEW/GUI/FUNCTOR/glBackboneModel.h
		*/
		class AddGLBackboneModel: public AddBackboneModel
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new addGLBackboneModel.
					@return      AddGLBackboneModel new constructed addGLBackboneModel
					@see         AddBackboneModel
			*/
			AddGLBackboneModel()
				throw();

			/** Copy constructor.
					Construct new addGLBackboneModel by copying the addGLBackboneModel
					{\em add_gl_backbone_model}. Initializes the state of {\em this} addGLBackboneModel 
					to the state of {\em add_gl_backbone_model}.
					@param       add_gl_backbone_model the addGLBackboneModel to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em add_gl_backbone_model}
					@return      AddGLBackboneModel new constructed addGLBackboneModel copied from {\em add_gl_backbone_model}
					@see         AddBackboneModel
			*/
			AddGLBackboneModel(const AddGLBackboneModel& add_gl_backbone_model, bool deep = true)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addGLBackboneModel.
					Calls  \link AddBackboneModel::destroy AddBackboneModel::destroy \endlink .
					@see  AddBackboneModel
			*/
			virtual ~AddGLBackboneModel()
				throw();
			
			//@}

			protected:

			/** @name Creation method
					This method creates a primitives with OpenGL implementation used
					for generating the graphical representation of the shapes they
					represent.
					This method is the overridden method of the base class 
					 \link AddBackboneModel AddBackboneModel \endlink .
					@see  AddBackboneModel
			*/
			//@{
			/** Create a mesh.
					Create a  \link GLBackbone GLBackbone \endlink  object and returns it as  \link Backbone Backbone \endlink .
					This overridden method of the class  \link AddBackboneModel AddBackboneModel \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create a backbone object
					for the {\em CA}-atoms determined by the  \link operator() operator() \endlink  method.
					@see  GLBackbone
					@see  Backbone
					@see  AddBackboneModel
			*/
			virtual Backbone* createBackbone_();
			//@}
		};
     /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLBACKBONEMODEL_H
