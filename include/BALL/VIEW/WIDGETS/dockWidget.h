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
				This class is very generic, to understand its concept better, have a look at the
				implementation in LogView.
				\ingroup ViewWidgets
		*/
		class BALL_EXPORT DockWidget
			: public QDockWindow,
				public ModularWidget
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(DockWidget,Embeddable)

			// required for Python bindings, but dont use this method
			DockWidget();

			// required for Python bindings, but dont use this method
			DockWidget(const DockWidget&);

			/// Use this constructor!
			DockWidget(QWidget* parent, const char* title = 0);

			///
			virtual ~DockWidget() throw() {}
			
			//@{
			/** Insert a widget with the content of the window.
			 */
			void setGuest(QWidget& guest);

			///
			QVBoxLayout* getLayout()
				throw() {return layout_;}
			
			/**	Initialize the widget.
					@see ModularWidget::initializeWidget
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();

			/**	Remove the widget.
					@see ModularWidget::finalizeWidget
			*/
			virtual void finalizeWidget(MainControl& main_control) throw();

			/** Load settings from an INIFile
					@see ModularWidget::fetchPreferences
			*/
			virtual void fetchPreferences(INIFile& inifile)	throw();
			
			/** Write settings to an INIFile
					@see ModularWidget::writePreferences
			*/
			virtual void writePreferences(INIFile& inifile) throw();

			/** Apply Preferences
					@see ModularWidget::applyPreferences
			*/
			virtual void applyPreferences(Preferences & /* preferences */) throw();				

			//@}
			/** @name Public slots 
			*/ 
			//@{
			public slots:

			/** Show or hide widget (Called by menu entry in "WINDOWS")
			*/
			virtual void switchShowWidget() throw();
				
			/** Set the MenuEntry in "WINDOWS" after closing a DockWindow with the Close button.
					Connected to the signal visibilityChanged().
			*/
			void setWindowsMenuEntry(bool state);

			//@} 

			protected:

			QWidget* 			guest_;
			QLabel* 			caption_label_;
			QVBoxLayout* 	layout_;
		};

  }  // namespace VIEW
} // namespace BALL
#endif 
