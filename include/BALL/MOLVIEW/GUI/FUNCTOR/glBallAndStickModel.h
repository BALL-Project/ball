// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glBallAndStickModel.h,v 1.11 2003/03/14 11:49:39 sturm Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLBALLANDSTICKMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLBALLANDSTICKMODEL_H

#ifndef BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H
#	include <BALL/VIEW/GUI/PRIMITIV/glsphere.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLTUBE_H
#	include <BALL/VIEW/GUI/PRIMITIV/gltube.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BALLANDSTICKMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/ballAndStickModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H
#	include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredTube.h>
#endif

namespace BALL
{

	namespace MOLVIEW
	{
    /**  \addtogroup  MolviewGuiFunctor
     *  @{
     */
		/** AddGLBallAndStickModel class.
				The class AddGLBallAndStickModel is derived from the class 
				 \link AddBallAndStickModel AddBallAndStickModel \endlink  and extents this class by overriding the creation
				methods used for creating the primitives. These new primitives contain
				OpenGL implementation to generate the graphical visualization of geometric
				shapes they present.
				This class has the same functionality as its base class  \link AddBallAndStickModel AddBallAndStickModel \endlink .
				
		*/
		class AddGLBallAndStickModel: public AddBallAndStickModel
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new addGLBallAndStickModel.
					@return      AddGLBallAndStickModel new constructed addGLBallAndStickModel
					@see         AddBallAndStickModel
			*/
			AddGLBallAndStickModel()
				throw();

			/** Copy constructor.
					Constructs new addGLBallAndStickModel by copying the addGLBallAndStickModel
					{\em add_GL_ball_and_stick_model}. Initializes the state of {\em this} 
					addGLBallAndStickModel to the state of {\em add_GL_ball_and_stick_model}.
					@param       add_GL_ball_and_stick_model the addGLBallAndStickModel to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em add_GL_ball_and_stick_model}
					@return      AddGLBallAndStickModel new addGLBallAndStickModel copied from {\em add_GL_ball_and_stick_model}
					@see         AddBallAndStickModel
			*/
			AddGLBallAndStickModel
				(const AddGLBallAndStickModel& add_GL_ball_and_stick_model, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addGLBallAndStickModel.
					Calls  \link AddBallAndStickModel::destroy AddBallAndStickModel::destroy \endlink .
					@see  AddBallAndStickModel
			*/
			virtual ~AddGLBallAndStickModel()
				throw();
			//@}

	  	protected:

			/** @name Creation methods
					This methods create primitives with OpenGL implementation used
					for generating the graphical representation of the shapes they
					represent.
					This methods are overridden methods of the base class 
					 \link AddBallAndStickModel AddBallAndStickModel \endlink .
					@see  AddBallAndStickModel
			*/
			//@{
			/** Creates a sphere.
					Creates a  \link GLSphere GLSphere \endlink  object and returns it as  \link Sphere Sphere \endlink .
					This overridden method of the class  \link AddBallAndStickModel AddBallAndStickModel \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create a sphere
					for each  \link Atom Atom \endlink  object.
					@see  GLSphere
					@see  Sphere
					@see  AddBallAndStickModel
					@see  Atom
			*/
			virtual Sphere* createSphere_();
		 
			/** Creates a one colored tube.
					Creates a  \link GLTube GLTube \endlink  object and returns it as  \link Tube Tube \endlink .
					This overridden method of the class  \link AddBallAndStickModel AddBallAndStickModel \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create one colored
					tubes for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have the same
					element types and thus the same color.
					@see  GLTube
					@see  Tube
					@see  AddBallAndStickModel
					@see  Atom
					@see  Bond
			*/
			virtual Tube* createTube_();
		 
			/** Creates a two colored tube.
					Creates a  \link GLTwoColoredTube GLTwoColoredTube \endlink  object and returns it as  \link TwoColoredTube TwoColoredTube \endlink .
					This overridden method of the class  \link AddBallAndStickModel AddBallAndStickModel \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create two colored
					tubes for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have different
					element types and thus different colors.
					@see  GLTwoColoredTube
					@see  TwoColoredTube
					@see  AddBallAndStickModel
					@see  Atom
					@see  Bond
			*/
			virtual TwoColoredTube* createTwoColoredTube_();
			//@}

			private:
		};
     /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLBALLANDSTICKMODEL_H
