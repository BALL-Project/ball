// $Id: glLineModel.h,v 1.4 2001/07/10 12:40:53 aubertin Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLLINEMODEL_H
#define BALL_MOLVIEW_GUI_FUNCTOR_GLLINEMODEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

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
		
		/** AddGLLineModel class.
				{\bf Framework:} BALL/MOLVIEW/GUI/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/FUNCTOR/glLineModel.h}\\ \\
				The class AddGLLineModel is derived from the class 
				\Ref{AddLineModel} and extents this class by overriding the creation
				methods used for creating the primitives. These new primitives contain
				OpenGL implementation to generate the graphical visualization of geometric
				shapes they present.
				This class has the same functionality as its base class \Ref{AddLineModel}.
				@memo    AddGLLineModel class (BALL MOLVIEW gui functor framework)
				@author  $Author: aubertin $
				@version $Revision: 1.4 $
				@date    $Date: 2001/07/10 12:40:53 $
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
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em add_GL_line_model}
					@return      AddGLLineModel new constructed addGLLineModel copied from {\em add_GL_line_model}
					@see         AddLineModel
			*/
			AddGLLineModel
				(const AddGLLineModel& add_GL_line_model, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} addGLLineModel.
					Calls \Ref{AddLineModel::destroy}.
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
					\Ref{AddLineModel}.
					@see  AddLineModel
			*/
			//@{
			/** Creates a point.
					Creates a \Ref{GLPoint} object and returns it as \Ref{Point}.
					This overridden method of the class \Ref{AddLineModel} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create a point
					for each \Ref{Atom} object whose.
					@see  GLPoint
					@see  Point
					@see  AddLineModel
					@see  Atom
			*/
			virtual Point* createPoint_();
		 
			/** Creates a one colored line.
					Creates a \Ref{GLLine} object and returns it as \Ref{Line}.
					This overridden method of the class \Ref{AddLineModel} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create one colored
					lines for \Ref{Bond} objects whose \Ref{Atom} objects have the same
					element types and thus the same color.
					@see  GLLine
					@see  Line
					@see  AddLineModel
					@see  Atom
					@see  Bond
			*/
			virtual Line* createLine_();

			/** Creates a two colored line.
					Creates a \Ref{GLTwoColoredLine} object and returns it as \Ref{TwoColoredLine}.
					This overridden method of the class \Ref{AddLineModel} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create two colored
					lines for \Ref{Bond} objects whose \Ref{Atom} objects have different
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

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/glLineModel.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_GLLINEMODEL_H
