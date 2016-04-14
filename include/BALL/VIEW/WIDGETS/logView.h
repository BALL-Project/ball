// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_LOGVIEW_H
#define BALL_VIEW_WIDGETS_LOGVIEW_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifdef BALL_HAS_SSTREAM
# include <sstream>
#else
# include <strstream>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
#	include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <QtWidgets/QTextBrowser>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDropEvent>

namespace BALL
{
	namespace VIEW
	{
		class DragLogView
			: public QTextBrowser
		{
			Q_OBJECT

			public:

			DragLogView(QWidget* parent);

			public slots:
			virtual void contentsDragEnterEvent(QDragEnterEvent* e);
			virtual void contentsDragLeaveEvent(QDragLeaveEvent* e);
			virtual void contentsDropEvent(QDropEvent* e);
			virtual void setSource(const QUrl& name);
		};

		/** LogView class.
				The class LogView records all messages sent to the  \link BALL::LogStream Log \endlink  object and
				displays them as a text history. The class is derived from 
				<b> LogStreamNotifier </b> that provides the connection
				to the  \link BALL::LogStream Log \endlink  object. The class  QTextEdit from the 
				qt - library is responsible for the visualization of the text history.
				Use the class LogView as a widget. There are no initializations necessary.
				Just create this widget as a child widget of your application and it will
				record and show all messages sent to the \link BALL::LogStream Log \endlink object.
				The LogView supports URLs, that are included into the LogStream.
				The links must only be one line long.
				If such a link is clicked, it's document is shown in the HelpViewer.
				This can be used to show the corresponding section in the BALLView
				documentation for an occuring problem.
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT LogView
			: public DockWidget,
			  public LogStreamNotifier
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(LogView,DockWidget)
		
			/** Default Constructor.
					The contructor connects the own
					<b> stringstream</b> with the  \link BALL::LogStream Log \endlink  object. If a string is written into
					 \link BALL::LogStream Log \endlink  this will be notified and the string will be displayed
					by this logView. 
					\see         BALL::LogStream
			*/
			LogView(QWidget *parent = 0, const char *name = 0);

			/** Copy constructor.
				 	Only for Python Interface
					The text of <b> view</b> will be copied into this logView.
			*/
			LogView(const LogView& view);

			/** Destructor.
					Calls  clear.
			*/
			virtual ~LogView();

			/**	Setup the menu entry in "Edit->Clear Logs".
			*/
			virtual void initializeWidget(MainControl& main_control);

			/**	Remove menu entries.
			*/
			virtual void finalizeWidget(MainControl& main_control);

			// output a string
			void logString(const String& text);

			public slots:

			virtual void showGuestContextMenu(const QPoint&);
			
			/// Event filter logstream
			bool eventFilter(QObject*, QEvent*);

			protected:

			/** Overridden notify call.
					Will be called by  \link BALL::LogStream Log \endlink  whenever a string is written to it.
					That string will then be displayed.
					\param   source the notification source
					\return  bool returns always <tt>true</tt>
			*/
			void logNotify();

			private:

			QTextEdit* text_edit_;
		};
  	
} } // namespaces

#endif // BALL_VIEW_WIDGETS_LOGVIEW_H
