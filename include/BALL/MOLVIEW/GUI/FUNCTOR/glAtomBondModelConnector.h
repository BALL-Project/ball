// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glAtomBondModelConnector.h,v 1.10 2003/03/03 08:17:29 sturm Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_GLATOMBONDMODELCONNECTOR_H
#define BALL_MOLVIEW_FUNCTOR_GLATOMBONDMODELCONNECTOR_H

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

namespace BALL
{
	using VIEW::GLLine;
	using VIEW::GLTube;

	namespace MOLVIEW
	{
    /** \ingroup MolviewGuiFunctor
     *  @{
     */		
		/** GLAtomBondModelConnector class.
				The class GLAtomBondModelConnector is derived from the class 
				 \link AtomBondModelConnector AtomBondModelConnector \endlink  and extents this class by overriding the creation
				methods used for creating the primitives. These new primitives contain
				OpenGL implementation to generate the graphical visualization of geometric
				shapes they present.
				This class has the same functionality as its base class  \link AtomBondModelConnector AtomBondModelConnector \endlink .  \par
				<b>Definition:</b> BALL/MOLVIEW/GUI/FUNCTOR/glAtomBondModelConnector.h
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
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em connector}
					@return      GLAtomBondModelConnector new constructed glAtomBondModelConnector copied from {\em connector}
					@see         AtomBondModelConnector
			*/
			GLAtomBondModelConnector(const GLAtomBondModelConnector& connector, bool deep = true)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glAtomBondModelConnector.
					Calls  \link AtomBondModelConnector::destroy AtomBondModelConnector::destroy \endlink .
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
					 \link AtomBondModelConnector AtomBondModelConnector \endlink .
					@see  AtomBondModelConnector
			*/
			//@{
			/** Create a one colored line.
					Create a  \link GLLine GLLine \endlink  object and returns it as  \link Line Line \endlink .
					This overridden method of the class  \link AtomBondModelConnector AtomBondModelConnector \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create one colored
					lines for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have the same
					element types and thus the same color.
					@see  GLLine
					@see  Line
					@see  AtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual Line* createLine_();

			/** Create a two colored line.
					Create a  \link GLTwoColoredLine GLTwoColoredLine \endlink  object and returns it as  \link TwoColoredLine TwoColoredLine \endlink .
					This overridden method of the class  \link AtomBondModelConnector AtomBondModelConnector \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create two colored
					lines for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have different
					element types and thus different colors.
					@see  GLTwoColoredLine
					@see  TwoColoredLine
					@see  AtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual TwoColoredLine* createTwoColoredLine_();

			/** Create a one colored tube.
					Create a  \link GLTube GLTube \endlink  object and returns it as  \link Tube Tube \endlink .
					This overridden method of the class  \link AtomBondModelConnector AtomBondModelConnector \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create one colored
					tubes for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have the same
					element types and thus the same color.
					@see  GLTube
					@see  Tube
					@see  AtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual Tube* createTube_();

			/** Create a two colored tube.
					Create a  \link GLTwoColoredTube GLTwoColoredTube \endlink  object and returns it as  \link TwoColoredTube TwoColoredTube \endlink .
					This overridden method of the class  \link AtomBondModelConnector AtomBondModelConnector \endlink  creates
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create two colored
					tubes for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have different
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

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/MOLVIEW/GUI/FUNCTOR/glAtomBondModelConnector.iC>
#	endif
     /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_GLATOMBONDMODELCONNECTOR_H
