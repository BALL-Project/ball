/**********************************************************************

	--- Qt Architect generated file ---

	File: DlgDisplayProperties.h
	Last generated: Mon Jan 10 18:55:27 2000

 *********************************************************************/

#ifndef DlgDisplayProperties_included
#define DlgDisplayProperties_included

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
# include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEOBJECTPROCESSOR_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeObjectProcessor.h>
#endif

#ifndef BALL_MOLVIEW_KERNEL_MOLECULARMESSAGE_H
# include <BALL/MOLVIEW/KERNEL/molecularMessage.h>
#endif

#include "DlgDisplayPropertiesData.h"

using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::MOLVIEW;

class DlgDisplayProperties : public DlgDisplayPropertiesData,
														 public ModularWidget
{
    Q_OBJECT

public:

		/**     @name Constructors and Destructors
	  */
		//@{
		
		/** Constructor for the display properties dialog.
				create a dialog for changing the appearance of selected molecular objects.
		*/
    DlgDisplayProperties(
        QWidget *parent = NULL,
        const char *name = NULL );

		/** Destructor for the display properties dialog.
		*/
    virtual ~DlgDisplayProperties();
		//@}

		/**     @name Accessors
	  */
		//@{
		
		/** register a object processor.
				This function provides a means to change the implementation of the routines,
				that create the graphical representation of the molecular structures.
				@param object_processor - a object processor
		*/
		void registerObjectProcessor(const MoleculeObjectProcessor& object_processor);

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
				If a graphical representation is changed this dialog sents a message to the other
				dialogs and widgets so that they can act upon the possibly changed state of
				some molecular objects. It also receives messages from other dialogs concerning
				the selection of some molecular objects.
				@param message - a pointer to a given message
		*/
		virtual void onNotify(Message *message);
		
		//@}

		/** @name Modular Widget related methods
		*/
		//@{

		/**     Initialize the display properties dialog.
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

		/** @name Protected slots
    */

		/** changes the precision of the selected objects.
	  */
    virtual void selectPrecision(const QString& string);

		/** changes the model of the selected objects
		*/
    virtual void selectModel(const QString& string);

		/** changes the coloring methods of the selected objects.
		*/
    virtual void selectColoringMethod(const QString& string);

		/** indicates that the apply button is pressed.
		*/
    virtual void applyButtonClicked();

		/** open the edit color dialog.
		*/ 
    virtual void editColor();


    private:

		void setComboBoxIndex_(QComboBox* combo_box, QString& item_string);

		int id_;

		QString   model_string_;
		QString   precision_string_;
		QString   coloring_method_string_;
		ColorRGBA custom_color_;

		MoleculeObjectProcessor *object_processor_;

		AtomDistanceColorCalculator distance_color_calculator_;
		bool distance_coloring_;

		List<Composite*> selection_;
};
#endif // DlgDisplayProperties_included
