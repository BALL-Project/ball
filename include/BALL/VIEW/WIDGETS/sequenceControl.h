// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_SEQUENCECONTROL_H
#define BALL_VIEW_WIDGETS_SEQUENCECONTROL_H

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#ifndef BALL_CONCEPT_MOLECULARINFORMATION_H
# include <BALL/CONCEPT/molecularInformation.h>
#endif

#ifndef BALL_VIEW_KERNEL_PREFERENCESENTRY
# include <BALL/VIEW/KERNEL/preferencesEntry.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
 #include <BALL/MATHS/matrix44.h>
#endif

#include <QtCore/QPoint>
#include <QtGui/QMenu>
#include <QtGui/QTreeView>
#include <QtGui/QComboBox>

#include <BALL/VIEW/UIC/ui_sequenceControl.h>


namespace BALL
{
	namespace VIEW
	{


//TODO
/**	SequenceControl is a widget to display the sequence of Composite objects. 
		This class is derived from the class DockWidget and extends it for showing and modifiying
		molecular structures. The methods checkMenu() and buildContextMenu() are overridden 
		for performing special molecular tasks.
		SequenceControl has also a QLineEdit to select Composites by BALL expressions.
		For further informations on this topic have a look at the class Selector.
		\ingroup ViewWidgets
*/
class BALL_VIEW_EXPORT SequenceControl
	: public DockWidget,
		public PreferencesEntry
{

	Q_OBJECT

	public:

  BALL_EMBEDDABLE(SequenceControl, DockWidget)

	/**	@name	Constructors and Destructor
	*/
	//@{

	/** Default Constructor.
			\param      parent the parent widget 
			\param      name the name of this widget
	*/
	SequenceControl(QWidget* parent = 0, const char* name = "SequenceControl");

	/** Destructor.
	*/
	virtual ~SequenceControl();

	//@} 
	/**	@name	Accessors: inspectors and mutators 
	*/
	//@{

	/**	Menu checking method.
			This method is called by MainControl::checkMenus before a popup menu is shown.
			The copy menu entry is only enabled for selected System objects.
			\param main_control the MainControl object whose menus should be checked
			\see   Control::checkMenu
	*/
//	virtual void checkMenu(VIEW::MainControl& main_control);

	/** Insert a Composite object into this Control.
			If <tt>name == ""</tt> than the Information visitor is used for determining
			a name for the Composite.\par
			Calls generateListViewItem_() for the Composite.
			\param   composite a pointer to the Composite to be inserted into the Control
	*/
  //void addComposite(Composite& composite, String given_name = "");

	/** Recursive removal of a Composite from the Control.
	*/
	//Size removeComposite(Composite& composite);

	/** Non-mutable inspection of the selection.
	*/
	//const std::list<Composite*>& getSelection() const;

	/** Message handling.
			Overridden method from ConnectionObject to handle incoming messages.\par
			Calls reactToMessages_().\par
			\param  message a pointer to a Message object
	*/
//	virtual void onNotify(Message *message);

	/** Build a context menu for a Composite.
			If the Composite has certain properties a context menu is created.
			This method only creates the necessary interface for derived classes.
			Override this method for creating other context menu entries, but call 
			Control::buildContextMenu at the end of your method to create the standard entries.
			Calls insertContextMenuEntry() for each context menu entry that will be created.
			\param   composite the Composite object for that a context menu should be created
			\see     onContextMenu
	*/
//	virtual void updateContextMenu(Composite& composite);

	/**	Initialize the menu entries:
				- cut 
				- copy 
				- paste 
				- delete
				- clear clipboard
			\par
			This method is called automatically	immediately before the main application 
			is started by MainControl::show.
			\param main_control the MainControl object to be initialized with this ModularWidget
	*/
	virtual void initializeWidget(MainControl& main_control);


	///
	void writePreferences(INIFile& inifile);

	///
	void fetchPreferences(INIFile& inifile);

	public slots:

	//@}
	/** @name Public slots
	*/
	//@{


	//@} 
	/** @name Protected members 
	*/
	//@{
	protected slots:



	protected:
		Ui_SequenceControlData ui_;

	///
	//	void buildContextMenu_();

	/** Set the selection of the checkboxes and the opening of the tree 
			according to the selection in the MainControl.
			\param open true means, that the item tree is opend and closed according to the changes
			\param force true means, that the item tree is opend and closed
						 also if more than 50 items are selected.
	*/
	//	void setSelection_(bool open, bool force = false);


	// only for Python Interface
	//	SequenceControl(const SequenceControl& mc);

	//	void enableUpdates_(bool state);
	//	void newSelection_(std::list<Composite*>& sel, bool selected);

	//@} 
	/** @name Menu entries ids
	*/
	//@{


};
}} // namespaces

#endif // BALL_VIEW_WIDGETS_SEQUENCECONTROL_H
