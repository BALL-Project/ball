// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: logView.h,v 1.10 2004/02/23 17:26:11 anhi Exp $
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

#ifndef QAPPLICATION_H
#	include <qapplication.h>
#endif

#ifndef QSTRING_H
#	include <qstring.h>
#endif

#ifndef QTEXTEDIT_H
# include <qtextedit.h>
#endif

#ifndef BALL_CONCEPT_NOTIFICATION_h
# include <BALL/CONCEPT/notification.h>
#endif

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
#	include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** LogView class.
				The class LogView records all messages sent to the  \link BALL::LogStream Log \endlink  object and
				displays them as a text history. The class is derived from 
				<b> NotificationTarget<LogStreamNotifier></b> that provides the connection
				to the  \link BALL::LogStream Log \endlink  object. The class  QTextEdit from the 
				qt - library is responsible for the visualization of the text history.
				Use the class LogView as a widget. There are no initializations necessary.
				Just create this widget as a child widget of your application and it will
				record and show all messages sent to the \link BALL::LogStream Log \endlink object.
				\ingroup ViewWidgets
		*/
		class LogView
			: public DockWidget,
			  public NotificationTarget<LogStreamNotifier>
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(LogView,DockWidget)
		
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The contructor connects the own
					<b> stringstream</b> with the  \link BALL::LogStream Log \endlink  object. If a string is written into
					 \link BALL::LogStream Log \endlink  this will be notified and the string will be displayed
					by this logView. 
					\see         BALL::LogStream
			*/
			LogView(QWidget *parent = 0, const char *name = 0)
				throw();

			/** Copy constructor.
					The text of <b> view</b> will be copied into this logView.
			*/
			LogView(const LogView& view)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Calls  clear.
			*/
			virtual ~LogView()
				throw();

			//@}

			/**	Setup the menu entry in "Edit->Clear Logs".
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();

			/**	Remove menu entries.
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();

			protected:

			/** Overridden notify call.
					Will be called by  \link BALL::LogStream Log \endlink  whenever a string is written to it.
					That string will then be displayed.
					\param   source the notification source
					\return  bool returns always <tt>true</tt>
			*/
			virtual bool onNotify(LogStreamNotifier &source)
				throw();

			private:

			QTextEdit* text_edit_;

			std::stringstream strstream_;

			bool output_running_;
		};
  	
} } // namespaces

#endif // BALL_VIEW_WIDGETS_LOGVIEW_H
