// $Id: QTTimer.C,v 1.6 2000/06/06 13:19:05 oliver Exp $


#include <BALL/VIEW/KERNEL/QTTimer.h>
#include <BALL/COMMON/exception.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		QTTimer::QTTimer()
			:	timer_id_(0),
				intervall_(100),
				timer_enabled_(false),
				timer_running_(false)
		{
		}

		QTTimer::QTTimer(const QTTimer& timer, bool /* deep */)
			:	timer_id_(0),
				intervall_(timer.intervall_),
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
			intervall_ = 100;
		}

		void QTTimer::destroy()
		{
			clear();
		}

		void QTTimer::set(const QTTimer& timer, bool /* deep */)
		{
			stopTimer();

			timer_running_ = false;
			intervall_ = timer.intervall_;
		}

		QTTimer& QTTimer::operator =(const QTTimer& timer)
		{
			set(timer);

			return *this;
		}

		void QTTimer::get(QTTimer& timer, bool deep) const
		{
			timer.set(*this, deep);
		}

		void QTTimer::swap(QTTimer& timer)
		{
			stopTimer();
			timer.stopTimer();

			int i = intervall_;
			intervall_ = timer.intervall_;
			timer.intervall_ = i;
		}

		void QTTimer::setIntervall(int intervall)
		{
			BALL_PRECONDITION
				(intervall >= 0,
				 BALL_VIEW_TIMER_ERROR_HANDLER
				 (QTTimer::ERROR__INTERVALL_CANNOT_BE_LOWER_THAN_ZERO));

			intervall_ = intervall;
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

		bool QTTimer::isValid() const
		{
			return true;
		}

		void QTTimer::dump(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "intervall: " << intervall_ << endl;

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
