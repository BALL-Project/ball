// $Id: 

#ifndef BALL_VIEW_KERNEL_TIMERTEXTVIEW_H
#define BALL_VIEW_KERNEL_TIMERTEXTVIEW_H

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

#ifndef QTEXTVIEW_H
#	include <qtextview.h>
#endif

#include <qmultilineedit.h>

#ifndef BALL_CONCEPT_NOTIFICATION_h
# include <BALL/CONCEPT/notification.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		class TimerTextView
			: public NotificationTarget<LogStreamNotifier>,
			  public QMultiLineEdit
		{
			public:

			/**	@name	 Constructors and Destructors	
			*/
			//@{

			/** default constructor. creates an instance of TimerTextView.
			*/
			TimerTextView(QWidget *parent = 0, const char *name = 0);

			TimerTextView(const TimerTextView& view, bool deep = true);

			/** default destructor. destroys an instance of TimerTextView.
			*/
			virtual ~TimerTextView();

			/** clear methode. resets the timer used to control the TimerTextView
			*/
			virtual void clear();

			/** destroy methode. resets the timer used to control the TimerTextView
			*/
			virtual void destroy();
			//@}

			/**	@name	Exceptions
			*/
			//@{
			
			/** Composite Exception. returned if the creation of a received object failed.
			*/
			//@}

			/**	@name	Accessors
			*/
			//@{

			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			/** returns true if the underlying timer is valid.
		  */
			virtual bool isValid() const;

			/** dumps the states.
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/	
			//@{

			/** Not yet implemented.
			*/
			virtual void read(std::istream& s);

			/** Not yet implemented.
			*/
			virtual void write(std::ostream& s) const;
			//@}

			protected:

			/** overridden notify call.
			*/
			virtual bool onNotify(LogStreamNotifier &source);


			private:

			/** private methodes used for reacting to client requests.
			*/


			/** private storage variables.
			*/
			QString history_string_;

			std::strstream strstream_;
		};



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/timerTextView.iC>
#		endif

	}// namespace VIEW
		
}// namespace BALL

#endif // BALL_VIEW_KERNEL_TIMERTEXTVIEW_H
