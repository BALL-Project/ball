// $Id: glAtomBondModelConnector.h,v 1.2 2000/10/05 10:51:03 oliver Exp $

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

using namespace BALL;
using namespace BALL::VIEW;
 
namespace BALL
{

	namespace MOLVIEW
	{
		
		/**
		*/
		class GLAtomBondModelConnector
			: public AtomBondModelConnector
		{
			public:
			
			/**	@name	Type definitions
			*/
			//@{
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			GLAtomBondModelConnector();

			GLAtomBondModelConnector
				(const AtomBondModelConnector& connector, bool deep = true);

			virtual ~GLAtomBondModelConnector();

			//@}

			/**	@name	Exceptions
			*/
			//@{
			//@}

			/**	@name	Asignment
			*/
			//@{
			//@}

			
			/**	@name	Accessors
			*/
			//@{
			//@}
			

			/**	@name	Debugging and Diagnostics
			*/
			//@{
			//@}

			protected:

  		private:

			virtual Line*           createLine_();
			virtual TwoColoredLine* createTwoColoredLine_();
			virtual Tube*           createTube_();
			virtual TwoColoredTube* createTwoColoredTube_();
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/glAtomBondModelConnector.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_GLATOMBONDMODELCONNECTOR_H
