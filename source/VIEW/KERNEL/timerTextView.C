// $Id:

#include <BALL/VIEW/KERNEL/timerTextView.h>

using namespace std;

namespace BALL
{
  
	namespace VIEW
	{
  
		TimerTextView::TimerTextView(QWidget *parent, const char *name)
			:	NotificationTarget<LogStreamNotifier>(),
			  QMultiLineEdit(parent, name),
				history_string_(),
				strstream_()
		{
			Log.insert(strstream_);
			Log.insertNotification(strstream_, *this);

			//			setTextFormat(PlainText);
		}

		TimerTextView::TimerTextView(const TimerTextView& view, bool deep)
			:	NotificationTarget<LogStreamNotifier>(),
			  QMultiLineEdit(),
				history_string_(view.history_string_),
				strstream_()
		{
			Log.insert(strstream_);
			Log.insertNotification(strstream_, *this);

			//			setTextFormat(PlainText);
		}

		TimerTextView::~TimerTextView()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<TimerTextView>() << endl;
			#endif 

			Log.remove(strstream_);

			destroy();
		}

		void TimerTextView::clear()
		{
		}

		void TimerTextView::destroy()
		{
			clear();
		}

		bool TimerTextView::isValid() const
		{
			return (bool)(true);
		}

		void TimerTextView::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void TimerTextView::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void TimerTextView::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

	  bool TimerTextView::onNotify(LogStreamNotifier &source)
		{
			string line;
			
			char c;
			strstream_.get(c);

			while (strstream_.gcount() > 0)
			{
				line += c;
				strstream_.get(c);
			}

			strstream_.clear();

			if (line.size() > 0)
			{
				history_string_ += QString(line.c_str());

				setText(history_string_);
				//	scrollBy(0, visibleHeight());
				repaint();

				qApp->processEvents();
			}

			return true;
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/timerTextView.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
