/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgLabelProperties.h
	Last generated: Sun Jun 4 13:50:59 2000

 *********************************************************************/

#ifndef DlgLabelProperties_included
#define DlgLabelProperties_included

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

#include "DlgLabelPropertiesData.h"

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;

class DlgLabelProperties : public DlgLabelPropertiesData,
													 public ModularWidget
{
    Q_OBJECT

 public:

		/** @name Constructors and Destructors
		*/
		//@{

		/** Constructor for the label property dialog.
				create a dialog for adding labels to the selected objects.
		*/
    DlgLabelProperties(
        QWidget *parent = NULL,
        const char *name = NULL );

		/** Destructor.
		*/
    virtual ~DlgLabelProperties();
		//@}

		/** @name Accessors
		*/
		//@{

		/** set the preferences.
				This dialog stores its preferences in an unique inifile, which must be already open
				when given to this function.
				@param inifile - the inifile
		*/
		void setPreferences(INIFile& inifile) const;

		/** get the preferences.
				Reads the preferences from the given inifile. As in the function above this inifile
				must be open when given to this function.
				@param inifile - the inifile
		*/
		void getPreferences(const INIFile& inifile);

		/** receive and process messages sent to this dialog.
				Receives the selected objects to which a label would be attached.
				@param message - a pointer to a given message
		*/
		virtual void onNotify(Message *message);

		/**     Initialize the label properties dialog.
						This method is called automatically
						immediately before the main application 
						is started. It adds the dialog's 
						menu entries and connections.
		*/
		virtual void initializeWidget(MainControl& main_control);
		
		/**     Remove the dialog.
						This method is called by the dialog's destructor.
						It reverses all actions performed in 
						initializeWidget (remove menu entries and connections).
		*/
		virtual void finalizeWidget(MainControl& main_control);
		
		/**     Update all menu entry states.
						This method is called just before a popup menu
						is shown (via the QT signal aboutToShow()).
						It should be used to update the state of 
						menu entries (e.g. disable or enable entries).
		*/
		virtual void checkMenu(MainControl& main_control);
		//@}

 public slots:
			
		/** @name Public slots
		*/
		//@{

		/** open the dialog.
	  */
  	void openDialog();
	  //@}

 protected slots:

		/** @name Public slots
		*/
		//@{

		/** indicates that the apply button is pressed.
		*/
    virtual void applyButtonClicked();
	
    /** open the edit color dialog.
		*/ 
    virtual void editColor();
	  //@}

 private:

		int id_;

		ColorRGBA custom_color_;
		List<Composite*> selection_;
};
#endif // DlgLabelProperties_included
