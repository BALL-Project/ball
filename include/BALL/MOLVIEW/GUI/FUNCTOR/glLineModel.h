// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glLineModel.h,v 1.9 2003/03/03 08:17:30 sturm Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLLINEMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLLINEMODEL_H

#ifndef BALL_VIEW_GUI_PRIMITIV_GLLINE_H
#	include <BALL/VIEW/GUI/PRIMITIV/glline.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLPOINT_H
#	include <BALL/VIEW/GUI/PRIMITIV/glpoint.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_LINEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/lineModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H
#	include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredLine.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
    /** \ingroup MolviewGuiFunctor
     *  @{
     */		
		/** AddGLLineModel class.
				The class AddGLLineModel is derived from the class 
				 \link AddLineModel AddLineModel \endlink  and extents this class by overriding the creation
				methods used for creating the primitives. These new primitives contain
				OpenGL implementation to generate the graphical visualization of geometric
				shapes they present.
				This class has the same functionality as its base class  \link AddLineModel AddLineModel \endlink .  \par
				<b>Definition:</b> BALL/MOLVIEW/GUI/FUNCTOR/glLineModel.h
		*/
		class AddGLLineModel: public AddLineModel
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new addGLLineModel.
					@return      AddGLLineModel new constructed addGLLineModel
					@see         AddLineModel
			*/
			AddGLLineModel()
				throw();

			/** Copy constructor.
					Constructs new addGLLineModel by copying the addGLLineModel
					{\em add_GL_line_model}. Initializes the state of {\em this} 
					addGLLineModel to the state of {\em add_GL_line_model}.
					@param       add_GL_line_model the addGLLineModel to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em add_GL_line_model}
					@return      AddGLLineModel new constructed addGLLineModel copied from {\em add_GL_line_model}
					@see         AddLineModel
			*/
			AddGLLineModel(const AddGLLineModel& add_GL_line_model, bool deep = true)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addGLLineModel.
					Calls  \link AddLineModel::destroy AddLineModel::destroy \endlink .
					@see  AddLineModel
			*/
			virtual ~AddGLLineModel()
				throw();

			//@}
			
			protected:

			/** @name Creation methods
					This methods create primitives with OpenGL implementation used
					for generating the graphical representation of the shapes they
					represent.
					This methods are overridden methods of the base class 
					 \link AddLineModel AddLineModel \endlink .
					@see  AddLineModel
			*/
			//@{
			
			/** Creates a point.
					Creates a  \link GLPoint GLPoint \endlink  object and returns it as  \link Point Point \endlink .
					This overridden method of the class  \link AddLineModel AddLineModel \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create a point
					for each  \link Atom Atom \endlink  object whose.
					@see  GLPoint
					@see  Point
					@see  AddLineModel
					@see  Atom
			*/
			virtual Point* createPoint_();
		 
			/** Creates a one colored line.
					Creates a  \link GLLine GLLine \endlink  object and returns it as  \link Line Line \endlink .
					This overridden method of the class  \link AddLineModel AddLineModel \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create one colored
					lines for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have the same
					element types and thus the same color.
					@see  GLLine
					@see  Line
					@see  AddLineModel
					@see  Atom
					@see  Bond
			*/
			virtual Line* createLine_();

			/** Creates a two colored line.
					Creates a  \link GLTwoColoredLine GLTwoColoredLine \endlink  object and returns it as  \link TwoColoredLine TwoColoredLine \endlink .
					This overridden method of the class  \link AddLineModel AddLineModel \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create two colored
					lines for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have different
					element types and thus different colors.
					@see  GLTwoColoredLine
					@see  TwoColoredLine
					@see  AddLineModel
					@see  Atom
					@see  Bond
			*/
			virtual TwoColoredLine* createTwoColoredLine_();

			//@}
		};
     /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLLINEMODEL_H
