// $Id: glVanDerWaalsModel.h,v 1.6 2001/07/16 14:49:28 amoll Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLVANDERWAALSMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLVANDERWAALSMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H
#	include <BALL/VIEW/GUI/PRIMITIV/glsphere.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_VANDERWAALSMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/vanDerWaalsModel.h>
#endif


namespace BALL
{
	
	namespace MOLVIEW
	{

		/** AddGLVanDerWaalsModel class.
				The class AddGLVanDerWaalsModel is derived from the class 
				\Ref{AddVanDerWaalsModel} and extents this class by overriding the creation
				methods used for creating the primitives. These new primitives contain
				OpenGL implementation to generate the graphical visualization of geometric
				shapes they present.
				This class has the same functionality as its base class \Ref{AddVanDerWaalsModel}. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/FUNCTOR/glVanDerWaalsModel.h}
		*/
		class AddGLVanDerWaalsModel: public AddVanDerWaalsModel
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new addGLVanDerWaalsModel.
					@return      AddGLVanDerWaalsModel new constructed addGLVanDerWaalsModel
					@see         AddVanDerWaalsModel
			*/
			AddGLVanDerWaalsModel()
				throw();

			/** Copy constructor.
					Constructs new addGLVanDerWaalsModel by copying the addGLVanDerWaalsModel
					{\em add_GL_van_der_waals_model}. Initializes the state of {\em this} 
					addGLVanDerWaalsModel to the state of {\em add_GL_van_der_waals_model}.
					@param       add_GL_van_der_waals_model the addGLVanDerWaalsModel to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em add_GL_van_der_waals_model}
					@return      AddGLVanDerWaalsModel new constructed addGLVanDerWaalsModel copied from {\em add_GL_van_der_waals_model}
					@see         AddVanDerWaalsModel
			*/
			AddGLVanDerWaalsModel
				(const AddGLVanDerWaalsModel& add_Gl_van_der_waals_model, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addGLVanDerWaalsModel.
					Calls \Ref{AddVanDerWaalsModel::destroy}.
					@see  AddVanDerWaalsModel
			*/
			virtual ~AddGLVanDerWaalsModel()
				throw();
			//@}


			protected:

			/** @name Creation method
					This method creates primitives with OpenGL implementation used
					for generating the graphical representation of the shapes they
					represent.
					This method is the overridden method of the base class 
					\Ref{AddVanDerWaalsModel}.
					@see  AddVanDerWaalsModel
			*/
			//@{
			/** Creates a sphere.
					Creates a \Ref{GLSphere} object and returns it as \Ref{Sphere}.
					This overridden method of the class \Ref{AddVanDerWaalsModel} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create a sphere
					for each \Ref{Atom} object.
					@see  GLSphere
					@see  Sphere
					@see  AddVanDerWaalsModel
					@see  Atom
			*/
			virtual Sphere* createSphere_();
			//@}
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/glVanDerWaalsModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLVANDERWAALSMODEL_H
