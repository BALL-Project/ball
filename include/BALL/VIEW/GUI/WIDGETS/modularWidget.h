// $Id: modularWidget.h,v 1.3 2000/09/23 14:15:07 hekl Exp $

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
#define BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_EMBEDDABLE_H
#	include <BALL/CONCEPT/embeddable.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_MAINCONTROL_H
#	include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONNECTIONOBJECT_H
# include <BALL/VIEW/KERNEL/connectionObject.h>
#endif

using namespace BALL;

/**	The base class for all modular widgets.
		BALLVIEW provides a simple mechanism for Rapid 
		Application Prototyping based on {\em modular widgets}.
		Using the modular widgets and the \Ref{MainControl} class
		(or more precisely, classes derived from MainControl) 
		applications can be developed in a plug&play style.
		The most important of a visualization application are	
		implemented as classes derived from ModularWidget.
		Inserting an instance of one of these widgets 
		autoamtically creates the required menus in the menubar 
		of the main window, sets up the required connections,
		and registers the class instance for use from the embedded
		Python interpreter (if desired).\\
		When implementing classes derived from ModularWidget and you
		want to give access to the class (from Python), please remember
		to include the \REF{BALL_EMBEDDABLE} macro in the public section
		of your class declaration. Also make sure that the QT class you
		derive from (e.g. QWidget) is the {\em first} base class and
		ModularWidget second.
		@see MainControl
		@see Embeddable
		@see PyInterpreter
		@see PyWidget
*/
class ModularWidget
  : public Embeddable,
		public ConnectionObject
{
  public:
	
	BALL_EMBEDDABLE(ModularWidget)

	BALL_CREATE(ModularWidget)

	/**	@name Constructors and Destructors
	*/
	//@{
	/**	Default constructor.
	*/
	ModularWidget(const char* name = "<ModularWidget>");
		
	/**	Copy constructor.
	*/
	ModularWidget(const ModularWidget& widget);
		
	/**	Destructor.
			The destructor calls finalizeWidget.
	*/
	virtual ~ModularWidget();
	//@}

	/**	@name Accessors
	*/
	//@{
	/**	Register the widget with the MainControl.
	*/
	static void registerWidget(ModularWidget* mwidget);

	/**	Initialize the widget.
			This method is called automatically
			immediately before the main application 
			is started. It should add the widget's 
			menu entries and connections (if required).
	*/
	virtual void initializeWidget(MainControl& main_control);
		
	/**	Remove the widget.
			This method is called by the widget's destructor.
			It should reverse all actions performed in 
			initializeWidget (remove menu entries and connections).
	*/
	virtual void finalizeWidget(MainControl& main_control);

	/**	Update all menu entry states.
			This method is called just before a popup menu
			is shown (via the QT signal aboutToShow()).
			It should be used to update the state of 
			menu entries (e.g. disable or enable entries).
	*/
	virtual void checkMenu(MainControl& main_control);
	//@}

}; 
#endif // BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
