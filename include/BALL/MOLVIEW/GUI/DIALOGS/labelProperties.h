// $Id:

#ifndef BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIES_H
#define BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_STRUCTURE_GEOMETRICPROPERTIES_H
#	include <BALL/STRUCTURE/geometricProperties.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLLABEL_H
#	include <BALL/VIEW/GUI/PRIMITIV/gllabel.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
# include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#endif

#ifndef BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIESDATA_H
# include <BALL/MOLVIEW/GUI/DIALOGS/labelPropertiesData.h>
#endif

//using namespace BALL;
using namespace BALL::VIEW;
//using namespace BALL::MOLVIEW;

namespace BALL
{

	namespace MOLVIEW
	{

		/**	The LabelProperties class.
				{\bf Framework:} BALL/MOLVIEW/GUI/DIALOGS\\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/DIALOGS/labelProperties.h}\\ \\
				The class LabelProperties is a dialog for appending to a selection of 
				molecular objects a label which text and color can be changed.
				The class \Ref{MolecularControl} is responsible for creating such a selection.
				If this dialog is used, it should be created with \Ref{MainControl} as parent.
				The class LabelPropertiesData contains the definition of the layout of
				this dialog and is therefore not necessary for understanding.
				@memo    LabelProperties class (BALL MOLVIEW gui dialogs framework)
				@author  $Author: aubertin $
				@version $Revision: 1.6 $
				@date    $Date: 2001/07/06 13:48:22 $
		*/
		class LabelProperties : 
			public BALL::MOLVIEW::LabelPropertiesData,
			public ModularWidget
		{
			Q_OBJECT
				
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new labelProperties.
					Calls \Ref{registerWidget}.
					@param      parent the parent widget of {\em *this} labelProperties (See documentation of QT-library for information concerning widgets)
					@param      name the name of {\em *this} labelProperties (See documentation of QT-library for information concerning widgets)
					@return     LabelProperties new constructed labelProperties
					@see        QDialog
					@see        ModularWidget
			*/
			LabelProperties(QWidget *parent = NULL, const char *name = NULL )
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} labelProperties.
			*/
			virtual ~LabelProperties()
				throw();
			//@}
			
			/**	@name	Accessors: inspectors and mutators 
			 */
			//@{
			/** Message handling method.
					Handles messages sent by other registered \Ref{ConnectionObject} objects.
					Catches only \Ref{MolecularSelectionMessage} objects.
					If such a message is catched the apply button will be enabled and labels
					can be appended onto the selection.
					@param message the pointer to the message that should be processed
					@see   applyButtonClicked
					@see   MolecularSelectionMessage
					@see   ConnectionObject
					@see   Message
		  */
			virtual void onNotify(Message *message)
				throw();
			//@}

			/**	ModularWidget methods
			*/
			//@{
			
			/** Fetches the preferences.
					Fetches the preferences (the position and the selected color of {\em *this}
					labelProperties) from the	\Ref{INIFile} {\em inifile}.
					This method will be called inside the method \Ref{show} from the class
					\Ref{MainControl}.
					@param  inifile the \Ref{INIFile} that contains the needed information for {\em *this} labelProperties
					@see    writePreferences
			*/
			virtual void fetchPreferences(INIFile &inifile)
				throw();
			
			/** Writes the preferences.
					Writes the preferences (the position and the selected color of {\em *this}
					labelProperties) to the	\Ref{INIFile} {\em inifile}.
					This method will be called inside the method \Ref{aboutToExit} from the class
					\Ref{MainControl}.
					@param  inifile the \Ref{INIFile} to be written into
					@see    fetchPreferences
			*/
			virtual void writePreferences(INIFile &inifile)
				throw();
				
			/**	Initializes the widget.
					Initializes the popup menu {\em Display} with its checkable submenu 
					{\em Label Properties} and connects it to the method \Ref{openDialog}.
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by \Ref{show} from the \Ref{MainControl} object.
					@param main_control the \Ref{MainControl} object to be initialized with {\em *this} labelProperties
					@see   openDialog
					@see   finalizeWidget
					@see   insertMenuEntry
					@see   checkMenu
					@see   show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();
			
			/**	Removes the widget.
					Removes the checkable submenu {\em Label Properties} from the popup menu
					{\em Display}	and cuts the connection to the method \Ref{openDialog}.
					This method will be called by \Ref{aboutToExit} from the \Ref{MainControl}
					object.
					@param main_control the \Ref{MainControl} object to be finalized with {\em *this} labelProperties
					@see   initializeWidget
					@see   checkMenu
					@see   removeMenuEntry
					@see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
			
			/**	Menu checking method.
					This method is called by the method \Ref{checkMenus} from the
					\Ref{MainControl} object before a popup menu is shown.
					The menus {\em Label Properties} will be checked
					if {\em *this} labelProperties is visible. It will be enabled if a selection of
					objects to be labeled is available, disabled otherwise.
					@param main_control the \Ref{MainControl} object whose menus should be checked
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenus
					@see   show
			*/
			virtual void checkMenu(MainControl& main_control)
				throw();
			//@}
			
			public slots:
				
  		/** @name Public slots
	  	*/
			//@{
				
			/** Starts the labelProperties dialog.
					Opens {\em *this} labelProperties dialog.
					Calls \Ref{show} and \Ref{raise} from \Ref{QDialog} class.
					See documentation of QT-library for information concerning QDialog widgets.
			*/
			void openDialog();
			//@}
			
			protected slots:
				
			/** @name Protected slots
			*/
			//@{
				
			/** Indicates the apply button was pressed.
					Appends a \Ref{GLLabel} with the specified text and color
					to the geometric center of the objects in the selection. 
					Sents the message \Ref{ChangedCompositeMessage}, \Ref{SceneMessage}
					and \Ref{WindowMessage} to inform other \Ref{ConnectionObject} about the change
					in the \Ref{Composite} objects in the selection.
					@see  onNotify
					@see  Composite
					@see  GeometricCenterProcessor
					@see  GLLabel
					@see  ChangedCompositeMessage
					@see  SceneMessage
					@see  WindowMessage
			*/
			virtual void applyButtonClicked();
			
			/** Opens the edit color dialog.
					Opens the dialog for editing the color in which the text of the label
					should be drawn.
					Opens a \Ref{QColorDialog} from the QT-library.
					See documentation of QT-library for information concerning QColorDialog widgets.
			 */ 
			virtual void editColor();
			//@}
			
		private:
			
			int id_;
			
			ColorRGBA custom_color_;
			List<Composite*> selection_;
		};

		//#		ifndef BALL_NO_INLINE_FUNCTIONS
		//#			include <BALL/MOLVIEW/GUI/DIALOGS/labelProperties.iC>
		//#		endif 

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_DIALOGS_LABELPROPERTIES_H
