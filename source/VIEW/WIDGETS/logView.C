// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:
//

#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/COMMON/exception.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <qpopupmenu.h>
#include <qmenubar.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{
  
		LogView::LogView(QWidget *parent, const char *name)
			throw()
			: QTextEdit(parent, name),
			 	NotificationTarget<LogStreamNotifier>(),
				ModularWidget(name),
				history_string_(),
				strstream_(),
				output_running_(false)
		{
			registerWidget(this);
		}

		LogView::LogView(const LogView& view)
			throw()
			: QTextEdit(),
			 	NotificationTarget<LogStreamNotifier>(),
				ModularWidget(),
				history_string_(view.history_string_),
				strstream_(),
				output_running_(false)
		{
			registerWidget(this);
		}

		LogView::~LogView()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<LogView>() << endl;
			#endif 

			//destroy();
		}
		
	  bool LogView::onNotify(LogStreamNotifier& /* source */)
			throw()
		{
			if (output_running_) return false;
			output_running_ = true;
			char c;
			strstream_.get(c);

			String line;
			while (strstream_.gcount() > 0)
			{
				line += c;
				strstream_.get(c);
			}

			strstream_.clear();

			if (line.size() > 0)
			{
				append(line.c_str());
				scrollToBottom();
			}

			output_running_ = false;
			return true;
		}

		void LogView::initializeWidget(MainControl& main_control)
		{
			Log.insert(strstream_);
			Log.insertNotification(strstream_, *this);
			setReadOnly(TRUE);
			setTextFormat(PlainText);

			window_menu_entry_id_ = 
				main_control.insertMenuEntry(MainControl::WINDOWS, "LogView", this, SLOT(switchShowWidget()));
			main_control.insertMenuEntry(MainControl::TOOLS, "Clear Logs", this, SLOT(clear()));
			getMainControl()->menuBar()->setItemChecked(window_menu_entry_id_, true);
		}


		void LogView::finalizeWidget(MainControl& main_control)
		{
			main_control.removeMenuEntry(MainControl::WINDOWS, "LogView", this, SLOT(switchShowWidget()));
			main_control.removeMenuEntry(MainControl::TOOLS, "Clear Logs", this, SLOT(clear()));
			Log.remove(strstream_);
		}

		void LogView::switchShowWidget()
			throw()
		{
			QMenuBar* menu = getMainControl()->menuBar();
			if (menu->isItemChecked(window_menu_entry_id_))
			{
				hide();
				menu->setItemChecked(window_menu_entry_id_, false);
			}
			else
			{
				show();
				menu->setItemChecked(window_menu_entry_id_, true);
			}
		}

		void LogView::writePreferences(INIFile& inifile)
			throw()
		{
			inifile.insertValue("WINDOWS", "LogView::on", 
				String(getMainControl()->menuBar()->isItemChecked(window_menu_entry_id_)));
		}

		void LogView::fetchPreferences(INIFile & inifile)
			throw()
		{
			if (!inifile.hasEntry("WINDOWS", "LogView::on")) return;
			if (inifile.getValue("WINDOWS", "LogView::on").toUnsignedInt() == 0) 
			{
				switchShowWidget();
			}
		}

	} // namespace VIEW
} // namespace BALL
