// $Id: QTTimer.C,v 1.9 2001/02/04 16:14:27 hekl Exp $


#include <BALL/VIEW/KERNEL/QTTimer.h>
#include <BALL/COMMON/exception.h>
#include <stdio.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		QTTimer::NoValidInterval::NoValidInterval(const char* file, int line, const string& data)
			:	Exception::GeneralException(file, line, string("NoValidInterval"), string("not a valid interval: ") + data)
		{
		}

		QTTimer::QTTimer(QObject* parent, const char* name)
			:	QObject(parent, name),
			  timer_id_(0),
				interval_(100),
				timer_enabled_(false),
				timer_running_(false)
		{
		}

		QTTimer::QTTimer(const QTTimer &timer, QObject* parent, const char* name)
			:	QObject(parent, name),
			  timer_id_(0),
				interval_(timer.interval_),
				timer_enabled_(false),
				timer_running_(false)
		{
		}

		QTTimer::~QTTimer()
		{
#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<QTTimer>() << endl;
#endif 

			destroy();
		}

		void QTTimer::clear()
		{
			stopTimer();

			timer_running_ = false;
			interval_ = 100;
		}

		void QTTimer::destroy()
		{
			stopTimer();
		}

		void QTTimer::set(const QTTimer& timer)
		{
			stopTimer();

			timer_running_ = false;
			interval_ = timer.interval_;
		}

		QTTimer& QTTimer::operator =(const QTTimer& timer)
		{
			set(timer);

			return *this;
		}

		void QTTimer::get(QTTimer& timer) const
		{
			timer.set(*this);
		}

		void QTTimer::swap(QTTimer& timer)
		{
			stopTimer();
			timer.stopTimer();

			int i = interval_;
			interval_ = timer.interval_;
			timer.interval_ = i;
		}

		void QTTimer::setInterval(int interval)
		{
			if (interval <= 0)
			{
				char temp[20];

				sprintf(&temp[0], "%d", interval);
				throw NoValidInterval(__FILE__, __LINE__, temp);
			}

			interval_ = interval;
		}

		void QTTimer::timer()
		{
			return;
		}

		void QTTimer::timerEvent(QTimerEvent *timer_event)
		{
			if (timer_event->timerId() == timer_id_)
			{
				if (timer_running_ == false)
				{
					timer_running_ = true;
					timer();
					timer_running_ = false;
				}
			}
		}

		void QTTimer::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "interval: " << interval_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "timer running: " 
					 << ((timer_enabled_ == true) ? "yes" : "no") 
					 << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void QTTimer::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void QTTimer::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/QTTimer.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
