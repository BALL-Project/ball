// $Id:

#include <BALL/VIEW/KERNEL/logView.h>

using namespace std;

namespace BALL
{
  
	namespace VIEW
	{
  
		LogView::LogView(QWidget *parent, const char *name)
			:	NotificationTarget<LogStreamNotifier>(),
			  QMultiLineEdit(parent, name),
				history_string_(),
				strstream_()
		{
			Log.insert(strstream_);
			Log.insertNotification(strstream_, *this);
			setReadOnly(TRUE);
			setAutoUpdate(TRUE);
		}

		LogView::LogView(const LogView& view, bool deep)
			:	NotificationTarget<LogStreamNotifier>(),
			  QMultiLineEdit(),
				history_string_(view.history_string_),
				strstream_()
		{
			Log.insert(strstream_);
			Log.insertNotification(strstream_, *this);
			setReadOnly(TRUE);
			setAutoUpdate(TRUE);
		}

		LogView::~LogView()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<LogView>() << endl;
			#endif 

			Log.remove(strstream_);

			destroy();
		}

		void LogView::clear()
		{
		}

		void LogView::destroy()
		{
			clear();
		}

		bool LogView::isValid() const
		{
			return (bool)(true);
		}

		void LogView::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void LogView::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void LogView::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

	  bool LogView::onNotify(LogStreamNotifier &source)
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
				setCursorPosition(numLines(), 0);
				repaint();

				qApp->processEvents();
			}

			return true;
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/logView.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL
