// $Id: glBallAndStickModel.h,v 1.4 2001/07/10 12:38:05 aubertin Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLBALLANDSTICKMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLBALLANDSTICKMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

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

		/** AddGLBallAndStickModel class.
				{\bf Framework:} BALL/MOLVIEW/GUI/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/FUNCTOR/glBallAndStickModel.h}\\ \\
				The class AddGLBallAndStickModel is derived from the class 
				\Ref{AddBallAndStickModel} and extents this class by overriding the creation
				methods used for creating the primitives. These new primitives contain
				OpenGL implementation to generate the graphical visualization of geometric
				shapes they present.
				This class has the same functionality as its base class \Ref{AddBallAndStickModel}.
				@memo    AddGLBallAndStickModel class (BALL MOLVIEW gui functor framework)
				@author  $Author: aubertin $
				@version $Revision: 1.4 $
				@date    $Date: 2001/07/10 12:38:05 $
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
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em add_GL_ball_and_stick_model}
					@return      AddGLBallAndStickModel new constructed addGLBallAndStickModel copied from {\em add_GL_ball_and_stick_model}
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
					Calls \Ref{AddBallAndStickModel::destroy}.
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
					\Ref{AddBallAndStickModel}.
					@see  AddBallAndStickModel
			*/
			//@{
			/** Creates a sphere.
					Creates a \Ref{GLSphere} object and returns it as \Ref{Sphere}.
					This overridden method of the class \Ref{AddBallAndStickModel} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create a sphere
					for each \Ref{Atom} object.
					@see  GLSphere
					@see  Sphere
					@see  AddBallAndStickModel
					@see  Atom
			*/
			virtual Sphere* createSphere_();
		 
			/** Creates a one colored tube.
					Creates a \Ref{GLTube} object and returns it as \Ref{Tube}.
					This overridden method of the class \Ref{AddBallAndStickModel} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create one colored
					tubes for \Ref{Bond} objects whose \Ref{Atom} objects have the same
					element types and thus the same color.
					@see  GLTube
					@see  Tube
					@see  AddBallAndStickModel
					@see  Atom
					@see  Bond
			*/
			virtual Tube* createTube_();
		 
			/** Creates a two colored tube.
					Creates a \Ref{GLTwoColoredTube} object and returns it as \Ref{TwoColoredTube}.
					This overridden method of the class \Ref{AddBallAndStickModel} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create two colored
					tubes for \Ref{Bond} objects whose \Ref{Atom} objects have different
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

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/glBallAndStickModel.iC>
#			endif
		
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLBALLANDSTICKMODEL_H
