// $Id: molecularProperties.h,v 1.3 2000/11/05 14:38:19 hekl Exp $

#ifndef BALL_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H
#define BALL_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <iostream>
#include <qwidget.h>

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
#	include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_LOGVIEW_H
#	include <BALL/VIEW/KERNEL/logView.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
#	include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARINFORMATION_H
#	include <BALL/MOLVIEW/FUNCTOR/molecularInformation.h>
#endif


using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;


namespace BALL
{

	namespace MOLVIEW
	{

		/**
		 */
		class MolecularProperties
			: public QWidget,
			  public ModularWidget
		{
		public:
			
			/**	@name	Enums
			 */
			//@{
			//@}
			
			/**	@name	Type Definitions
			 */
			
			/**	@name	Constructors and Destructors
			 */
			//@{
			
			MolecularProperties(QWidget* parent = 0, const char* name = 0);
			
			virtual ~MolecularProperties();
			//@}
			
			/**	@name Exceptions
			 */
			//@{
			//@}
			
			/**	@name	Accessors
			 */
			//@{
			void onNotify(Message *message);
			//@}

			
			/**	ModularWidget methods.
			*/
			//@{
			/**
			*/
			virtual void initializeWidget(MainControl& main_control);
		
			/**
			*/
			virtual void finalizeWidget(MainControl& main_control);
		
			/**
			*/
			virtual void checkMenu(MainControl& main_control);
			//@}

		private:
			
			FragmentDB fragment_db_;  			
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/WIDGETS/molecularProperties.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIE_MOLVIEW_GUI_WIDGETS_MOLECULARPROPERTIES_H_
