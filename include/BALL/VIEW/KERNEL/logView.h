// $Id: 

#ifndef BALL_VIEW_KERNEL_LOGVIEW_H
#define BALL_VIEW_KERNEL_LOGVIEW_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <strstream>

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

		/** LogView class.
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/logView.h}\\
				{\bf Concept:} notification design pattern\\ \\
				The class LogView records all messages sent to the \Ref{Log} object and
				displays them as a text history. The class is derived from 
				{\em NotificationTarget<LogStreamNotifier>} that provides the connection
				to the \Ref{Log} object. The class \Ref{QMultiLineEdit} from the 
				qt - library is responsible for the visualization of the text history.
				Use the class LogView as a widget. There are no initializations necessary.
				Just create this widget as a child widgets of your application and it will
				record and show all messages sent to the \Ref{Log} object.
				@memo    LogView class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 13:47:47 $
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
					{\em strstream} with the \Ref{Log} object. If a string is written into
					\Ref{Log} {\em *this} will be notified and the string will be displayed
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
					Remove the connection between the \Ref{Log} object and {\em *this} logView.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~LogView()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} logView.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Storers
			*/	
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent logView data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
					@param       s input stream from where to restore the internal state of {\em *this} logView
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent logView data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
  				@param       s output stream to where to store the internal state of {\em *this} logView
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}

			protected:

			/** Overridden notify call.
					Will be called by \Ref{Log} whenever a string is written to it.
					That string will be added to the history string of {\em *this} logView
					and then displayed.
					@param   source the notification source
					@return  bool returns always {\tt true}
			*/
			virtual bool onNotify(LogStreamNotifier &source)
				throw();


			private:

			/** private storage variables.
			*/
			QString history_string_;

			std::strstream strstream_;
		};



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/logView.iC>
#		endif

	}// namespace VIEW
		
}// namespace BALL

#endif // BALL_VIEW_KERNEL_LOGVIEW_H
