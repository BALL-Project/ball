#ifndef BALL_VIEW_WIDGETS_DOCKABLEWIDGET_H
#define BALL_VIEW_WIDGETS_DOCKABLEWIDGET_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#	include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <qdockwindow.h>
#include <qlayout.h>

class QLabel;

namespace BALL
{
	namespace VIEW
	{

/** Class for ModularWidget, which can be docked in the MainApplication window.
 		Insert the widget with the main content as member with setGuest().
*/
class DockWidget
  : public QDockWindow,
		public ModularWidget
{
  Q_OBJECT

  public:

	BALL_EMBEDDABLE(DockWidget)

  DockWidget(QWidget* parent, const char* title = 0);

	/** Insert a widget with the content of the window.
	 */
	void setGuest(QWidget& guest);
	
	/**	Initialize the widget.
	 		@see ModularWidget::initializeWidget
	*/
	virtual void initializeWidget(MainControl& main_control)
		throw();

	/**	Remove the widget.
	 		@see ModularWidget::finalizeWidget
	*/
	virtual void finalizeWidget(MainControl& main_control)
		throw();

	/** Load settings from an INIFile
	 		@see ModularWidget::fetchPreferences
	*/
	virtual void fetchPreferences(INIFile& inifile)
		throw();
	
	/** Write settings to an INIFile
	 		@see ModularWidget::writePreferences
	*/
	virtual void writePreferences(INIFile& inifile)
		throw();

	//@}
	/** @name Public slots 
	*/ 
	//@{
	public slots:

	/** Show or hide widget (Called by menu entry in "WINDOWS")
			If the ModularWidget is not also a QWidget, this method does nothing
	*/
	virtual void switchShowWidget()
		throw();
		
	/** Set the MenuEntry in "WINDOWS" after closing a DockWindow with the Close button.
	 		Connected to the signal visibilityChanged().
	*/
	void setWindowsMenuEntry(bool state);

	//@} 

	virtual ~DockWidget() throw() {}

	protected:

	QWidget* guest_;
  QLabel* caption_label_;
  QVBoxLayout* layout_;
};


} } // namespaces
#endif 
