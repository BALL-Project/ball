// $Id: glAtomBondModelConnector.h,v 1.4 2001/05/13 14:55:24 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_GLATOMBONDMODELCONNECTOR_H
#define BALL_MOLVIEW_FUNCTOR_GLATOMBONDMODELCONNECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLLINE_H
#	include <BALL/VIEW/GUI/PRIMITIV/glline.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLTUBE_H
#	include <BALL/VIEW/GUI/PRIMITIV/gltube.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELCONNECTOR_H
#	include <BALL/MOLVIEW/FUNCTOR/atomBondModelConnector.h>
#endif

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H
#	include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredLine.h>
#endif

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H
#	include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredTube.h>
#endif

//using namespace BALL;
using namespace BALL::VIEW;
 
namespace BALL
{

	namespace MOLVIEW
	{
		
		/** GLAtomBondModelConnector class.
				{\bf Framework:} BALL/MOLVIEW/GUI/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/FUNCTOR/glAtomBondModelConnector.h}\\ \\
				The class GLAtomBondModelConnector is derived from the class 
				\Ref{AtomBondModelConnector} and extents this class by overriding the creation
				methods used for creating the primitives. These new primitives contain
				OpenGL implementation to generate the graphical visualization of geometric
				shapes they present.
				This class has the same functionality as its base class \Ref{AtomBondModelConnector}.
				@memo    GLAtomBondModelConnector class (BALL MOLVIEW gui functor framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 14:55:24 $
		*/
		class GLAtomBondModelConnector: public AtomBondModelConnector
		{
			public:
			

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glAtomBondModelConnector.
					@return      GLAtomBondModelConnector new constructed glAtomBondModelConnector
					@see         AtomBondModelConnector
			*/
			GLAtomBondModelConnector()
				throw();

			/** Copy constructor.
					Construct new glAtomBondModelConnector by copying the glAtomBondModelConnector
					{\em connector}. Initializes the state of {\em this} glAtomBondModelConnector 
					to the state of {\em connector}.
					@param       connector the glAtomBondModelConnector to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em connector}
					@return      GLAtomBondModelConnector new constructed glAtomBondModelConnector copied from {\em connector}
					@see         AtomBondModelConnector
			*/
			GLAtomBondModelConnector
				(const GLAtomBondModelConnector& connector, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glAtomBondModelConnector.
					Calls \Ref{AtomBondModelConnector::destroy}.
					@see  AtomBondModelConnector
			*/
			virtual ~GLAtomBondModelConnector()
				throw();

			//@}

			protected:

			/** @name Creation methods
					This methods create primitives with OpenGL implementation used
					for generating the graphical representation of the shapes they
					represent.
					This methods are overridden methods of the base class 
					\Ref{AtomBondModelConnector}.
					@see  AtomBondModelConnector
			*/
			//@{
			/** Create a one colored line.
					Create a \Ref{GLLine} object and returns it as \Ref{Line}.
					This overridden method of the class \Ref{AtomBondModelConnector} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create one colored
					lines for \Ref{Bond} objects whose \Ref{Atom} objects have the same
					element types and thus the same color.
					@see  GLLine
					@see  Line
					@see  AtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual Line* createLine_();

			/** Create a two colored line.
					Create a \Ref{GLTwoColoredLine} object and returns it as \Ref{TwoColoredLine}.
					This overridden method of the class \Ref{AtomBondModelConnector} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create two colored
					lines for \Ref{Bond} objects whose \Ref{Atom} objects have different
					element types and thus different colors.
					@see  GLTwoColoredLine
					@see  TwoColoredLine
					@see  AtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual TwoColoredLine* createTwoColoredLine_();

			/** Create a one colored tube.
					Create a \Ref{GLTube} object and returns it as \Ref{Tube}.
					This overridden method of the class \Ref{AtomBondModelConnector} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create one colored
					tubes for \Ref{Bond} objects whose \Ref{Atom} objects have the same
					element types and thus the same color.
					@see  GLTube
					@see  Tube
					@see  AtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual Tube* createTube_();

			/** Create a two colored tube.
					Create a \Ref{GLTwoColoredTube} object and returns it as \Ref{TwoColoredTube}.
					This overridden method of the class \Ref{AtomBondModelConnector} creates
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create two colored
					tubes for \Ref{Bond} objects whose \Ref{Atom} objects have different
					element types and thus different colors.
					@see  GLTwoColoredTube
					@see  TwoColoredTube
					@see  AtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual TwoColoredTube* createTwoColoredTube_();
			//@}
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/glAtomBondModelConnector.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_GLATOMBONDMODELCONNECTOR_H
