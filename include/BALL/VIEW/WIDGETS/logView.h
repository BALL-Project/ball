// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: logView.h,v 1.4 2003/09/11 09:54:26 amoll Exp $
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

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#	include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** LogView class.
				The class LogView records all messages sent to the  \link Log Log \endlink  object and
				displays them as a text history. The class is derived from 
				<b> NotificationTarget<LogStreamNotifier></b> that provides the connection
				to the  \link Log Log \endlink  object. The class  \link QTextEdit QTextEdit \endlink  from the 
				qt - library is responsible for the visualization of the text history.
				Use the class LogView as a widget. There are no initializations necessary.
				Just create this widget as a child widgets of your application and it will
				record and show all messages sent to the  \link Log Log \endlink  object.
				\ingroup ViewWidgets
		*/
		class LogView
			: public QTextEdit,
			  public NotificationTarget<LogStreamNotifier>,
				public ModularWidget
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(LogView)
		
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new logView.
					The text of this logView is empty. The contructor connects the own
					<b> stringstream</b> with the  \link Log Log \endlink  object. If a string is written into
					 \link Log Log \endlink  this will be notified and the string will be displayed
					by this logView. 
					\return      LogView new constructed logView
					\see         Log
			*/
			LogView(QWidget *parent = 0, const char *name = 0)
				throw();

			/** Copy constructor.
					Construct new logView by copying the logView <b> view</b>.
					The text of <b> view</b> will be copied into this logView.
					\param       view the logView to be copied
					\return      LogView new constructed logView copied from <b> view</b>
					\see         LogView
			*/
			LogView(const LogView& view)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Remove the connection between the  \link Log Log \endlink  object and this logView.
					Calls  \link clear clear \endlink .
			*/
			virtual ~LogView()
				throw();

			//@}
			/**	@name	Storers
			*/	
			//@{

			/**	Setup the menu entries.
					PyWidget creates an entry in Tools|Restart Python and connects
					the entry to startInterpreter().
			*/
			virtual void initializeWidget(MainControl& main_control);

			/**	Remove menu entries.
			*/
			virtual void finalizeWidget(MainControl& main_control);
				
			///
			virtual void fetchPreferences(INIFile& inifile)
				throw();
			
			///
			virtual void writePreferences(INIFile& inifile)
				throw();

		
			public slots:

			///	Show or hide widget (Called by menu entry in "WINDOWS")
			void switchShowWidget()
				throw();

			protected:

			/** Overridden notify call.
					Will be called by  \link Log Log \endlink  whenever a string is written to it.
					That string will be added to the history string of this logView
					and then displayed.
					\param   source the notification source
					\return  bool returns always <tt>true</tt>
			*/
			virtual bool onNotify(LogStreamNotifier &source)
				throw();

			private:

			QString history_string_;

			std::stringstream strstream_;

			bool output_running_;
		};
  	
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_LOGVIEW_H
