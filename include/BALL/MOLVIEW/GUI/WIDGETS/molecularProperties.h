// $Id: molecularProperties.h,v 1.7 2001/07/16 14:49:28 amoll Exp $

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


//using namespace BALL;
using namespace BALL::VIEW;


namespace BALL
{

	namespace MOLVIEW
	{

		/**	The MolecularProperties class.
				The class MolecularProperties is a widget that converts certain \Ref{Message}
				objects to other \Ref{Message} objects.
				This class is necessary to convert new \Ref{Composite} objects opened with either
				\Ref{openPDBFile} or \Ref{openHINFile} to \Ref{Composite} objects
				that have certain molecular properties. Further it converts the geometric selection
				sent by \Ref{Scene} to a molecular selection whose objects can be given a new
				graphical representation by the \Ref{DisplayProperties} dialog.
				See \Ref{onNotify} for information concerning the conversion mechanism. \\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/WIDGETS/molecularProperties.h}
		*/
		class MolecularProperties: public QWidget, public ModularWidget
		{
		public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new molecularProperties.
					Calls \Ref{registerWidget}.
					@param      parent the parent widget of {\em *this} molecularProperties (See documentation of QT-library for information concerning widgets)
					@param      name the name of {\em *this} molecularProperties (See documentation of QT-library for information concerning widgets)
					@return     MolecularProperties new constructed molecularProperties
					@see        QWidget
					@see        ModularWidget
			*/
			MolecularProperties(QWidget* parent = 0, const char* name = 0)
				throw();
			
			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} molecularProperties.
			*/
			virtual ~MolecularProperties()
				throw();
			//@}
			
			/**	@name	Accessors: inspectors and mutators 
			 */
			//@{
			/** Message handling method.
					Handles messages sent by other registered \Ref{ConnectionObject} objects.
					Converts \Ref{NewCompositeMessage} to \Ref{NewMolecularMessage} if the
					retrieved \Ref{Composite} object is kind of \Ref{AtomContainer} and
					applies molecular properties to it (like \Ref{normalize_names} and
					\Ref{build_bonds}).\\
					Converts \Ref{GeometricObjectSelectionMessage} to \Ref{NewMolecularMessage}
					if every \Ref{Composite} object in the selection has an ancestor that is
					an \Ref{AtomContainer}. These found ancestors are put into a new selection
					that is sent with the \Ref{NewMolecularMessage}.
					@param message the pointer to the message that should be processed
					@see   Message
					@see   NewCompositeMessage
					@see   NewMolecularMessage
					@see   GeometricObjectSelectionMessage
					@see   AtomContainer
					@see   ConnectionObject
		  */
			void onNotify(Message *message)
				throw();
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
