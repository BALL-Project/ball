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

class DockWidget
  : public QDockWindow,
		public ModularWidget
{
  Q_OBJECT

  public:

	BALL_EMBEDDABLE(DockWidget)

  DockWidget(QWidget* parent, const char* title = 0);

	void setGuest(QWidget& guest);
	
	/**	Initialize the widget.
	*/
	virtual void initializeWidget(MainControl& main_control)
		throw();

	/**	Remove the widget.
			Reverse all actions performed in initializeWidget
			(remove menu entries of this GenericControl).
			This method will be called by MainGenericControl::aboutToExit.
			\param main_GenericControl the MainTrajectoryControl object to be finalized with this TrajectoryControl
	*/
	virtual void finalizeWidget(MainControl& main_control)
		throw();

	///
	virtual void fetchPreferences(INIFile& inifile)
		throw();
	
	///
	virtual void writePreferences(INIFile& inifile)
		throw();

	QVBoxLayout* getLayout() { return layout_;}

	//@}
	/** @name Public slots 
	*/ 
	//@{
	public slots:

	/// Show or hide widget (Called by menu entry in "WINDOWS")
	void callSwitchShowWidget()
		throw();

	//@} 

	virtual ~DockWidget() throw() {}

	protected:

  QLabel* caption_label_;
  QVBoxLayout* layout_;
};


} } // namespaces
#endif 
