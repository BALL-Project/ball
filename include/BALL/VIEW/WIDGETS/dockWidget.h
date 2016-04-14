#ifndef BALL_VIEW_WIDGETS_DOCKABLEWIDGET_H
#define BALL_VIEW_WIDGETS_DOCKABLEWIDGET_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#	include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#include <QtWidgets/QDockWidget>
#include <QtGui/QDropEvent>
#include <QtWidgets/QDockWidget>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QGridLayout>

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
		class BALL_VIEW_EXPORT DockWidget
			: public QDockWidget,
				public ModularWidget
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(DockWidget,Embeddable)

			/// Use this constructor!
			DockWidget(QWidget* parent, const char* title = 0);

			///
			virtual ~DockWidget(){}
			
			/** Insert a widget with the content of the window.
			 */
			void setGuest(QWidget& guest);

			/**	Initialize the widget.
					@see ModularWidget::initializeWidget
			*/
			virtual void initializeWidget(MainControl& main_control);

			/** Load settings from an INIFile
					@see ModularWidget::fetchPreferences
			*/
			virtual void fetchPreferences(INIFile& inifile);
			
			/** Write settings to an INIFile
					@see ModularWidget::writePreferences
			*/
			virtual void writePreferences(INIFile& inifile);

			/// For usage with Python:
			virtual void setWidgetVisible(bool state);
			
			///
			virtual void registerForHelpSystem(const QObject* widget, const String& url);

			///
			QGridLayout* getGuestLayout() { return layout_;}

			public slots:

			///
			virtual void dropEvent(QDropEvent* e);

			///
			virtual void dragEnterEvent(QDragEnterEvent* e);

			///
			virtual void showGuestContextMenu(const QPoint&) {};

			protected:
			
			// required for Python bindings, but dont use this methods:
			DockWidget(); 
			DockWidget(const DockWidget&);
			
			QWidget* 			container_;
			QWidget* 			guest_;
			QGridLayout*  layout_;
		};

  }  // namespace VIEW
} // namespace BALL
#endif 
