// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: 

#ifndef BALL_VIEW_KERNEL_LOGVIEW_H
#define BALL_VIEW_KERNEL_LOGVIEW_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <sstream>

#ifndef QAPPLICATION_H
#	include <qapplication.h>
#endif

#ifndef QSTRING_H
#	include <qstring.h>
#endif

#ifndef QMULTILINEEDIT_H
# include <qmultilineedit.h>
#endif

#ifndef BALL_CONCEPT_NOTIFICATION_h
# include <BALL/CONCEPT/notification.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{
    /** \ingroup ViewKernelLogView
     *  @{
     */
		/** LogView class.
				The class LogView records all messages sent to the  \link Log Log \endlink  object and
				displays them as a text history. The class is derived from 
				{\em NotificationTarget<LogStreamNotifier>} that provides the connection
				to the  \link Log Log \endlink  object. The class  \link QMultiLineEdit QMultiLineEdit \endlink  from the 
				qt - library is responsible for the visualization of the text history.
				Use the class LogView as a widget. There are no initializations necessary.
				Just create this widget as a child widgets of your application and it will
				record and show all messages sent to the  \link Log Log \endlink  object.
				<b>Definition:</b> BALL/VIEW/KERNEL/logView.h
		*/
		class LogView
			: public NotificationTarget<LogStreamNotifier>,
			  public QMultiLineEdit
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new logView.
					The text of {\em *this} logView is empty. The contructor connects the own
					{\em stringstream} with the  \link Log Log \endlink  object. If a string is written into
					 \link Log Log \endlink  {\em *this} will be notified and the string will be displayed
					by {\em *this} logView. 
					@return      LogView new constructed logView
					@see         Log
			*/
			LogView(QWidget *parent = 0, const char *name = 0)
				throw();

			/** Copy constructor.
					Construct new logView by copying the logView {\em view}.
					The text of {\em view} will be copied into {\em *this} logView.
					@param       view the logView to be copied
					@return      LogView new constructed logView copied from {\em view}
					@see         LogView
			*/
			LogView(const LogView& view)
				throw();

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} logView.
					Remove the connection between the  \link Log Log \endlink  object and {\em *this} logView.
					Calls  \link clear clear \endlink .
			*/
			virtual ~LogView()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw();

			//@}

			/**	@name	Storers
			*/	

			protected:

			/** Overridden notify call.
					Will be called by  \link Log Log \endlink  whenever a string is written to it.
					That string will be added to the history string of {\em *this} logView
					and then displayed.
					@param   source the notification source
					@return  bool returns always <tt>true</tt>
			*/
			virtual bool onNotify(LogStreamNotifier &source)
				throw();


			private:

			/** private storage variables.
			*/
			QString history_string_;

			std::stringstream strstream_;
		};
  /** @} */	
	}// namespace VIEW
		
}// namespace BALL

#endif // BALL_VIEW_KERNEL_LOGVIEW_H
