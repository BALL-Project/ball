// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.C,v 1.39 2004/09/29 20:40:19 amoll Exp $
//

// This include has to be first in order to avoid collisions.
#include <Python.h>

#include <BALL/VIEW/WIDGETS/pyWidget.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DIALOGS/pythonSettings.h>
#include <BALL/VIEW/DIALOGS/pythonHotkeys.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/FORMAT/lineBasedFile.h>

#include <qscrollbar.h>
#include <qfiledialog.h>
#include <qapplication.h>

namespace BALL
{
	namespace VIEW
	{


#ifdef BALL_QT_HAS_THREADS
		RunPythonThread::RunPythonThread()
			throw()
			: QThread(),
				input(),
				output()
		{}

		void RunPythonThread::run()
		{
			output = PyInterpreter::run(input, state);
		}
#endif


		const Hotkey& Hotkey::operator = (const Hotkey& hotkey)
			throw()
		{
			action 				= hotkey.action;
			key 	 				= hotkey.key;
			button_state 	= hotkey.button_state;
			return *this;
		}


		bool Hotkey::operator == (const Hotkey& hotkey) const
			throw()
		{
			return (action 				== hotkey.action &&
							key 	 				== hotkey.key    &&
							button_state 	== hotkey.button_state);
		}

		bool Hotkey::operator == (const QKeyEvent& e) const
			throw()
		{

			return (key 	 				== e.key() 		&&
							button_state 	== e.state()	);
		}

		bool Hotkey::set(const String& data) 
			throw()
		{
			vector<String> fields;
			Size nr = data.split(fields, String('#').c_str());
			if (nr < 3)
			{
				Log.error() << "Could not parse Hotkey " << data << std::endl;
				return false;
			}
			
			try
			{
				key = (Qt::Key) fields[0].toUnsignedInt();
				button_state = (Qt::ButtonState) fields[1].toUnsignedInt();
				action = fields[2];
			}
			catch(...)
			{
				Log.error() << "Could not parse Hotkey " << data << std::endl;
				return false;
			}

			return true;
		}


		void Hotkey::get(String& data) const 
			throw()
		{
			data = String(key) + "#" + String(button_state) + "#" + action;
		}

		// ==========================================================

		PyWidgetData::PyWidgetData(QWidget* parent, const char* name)
			: QTextEdit(parent, name),
				thread_(0),
				stop_script_(false)
		{
			setWrapPolicy(QTextEdit::Anywhere);
			#ifdef BALL_QT_HAS_THREADS
			  thread_ = new RunPythonThread();
			#endif
		}

		PyWidgetData::PyWidgetData(const PyWidgetData& /*widget*/)
			:	QTextEdit(),
				stop_script_(false)
		{
		}

		PyWidgetData::~PyWidgetData()
			throw()
		{	
#ifdef BALL_QT_HAS_THREADS
			if (thread_->running())
			{
				thread_->wait(500);
				thread_->terminate();
			}
			delete thread_;
#endif
		}


		void PyWidgetData::stopInterpreter()
		{
			PyInterpreter::finalize();
		}


		void PyWidgetData::startInterpreter()
		{
			stop_script_ = false;
			// initialize the interpreter
			PyInterpreter::initialize();

			// print the PyBALL version and clear
			// the widget's contents in case of a restart
			setText((String("BALL ") + VersionInfo::getVersion()).c_str());

			// print the first prompt 
			multi_line_mode_ = false;
			newPrompt_();
			history_position_ = history_.size() + 1;
		}


		void PyWidgetData::retrieveHistoryLine_(Position index)
		{
			if (index > history_.size()) 
			{
				history_position_ = history_.size();
				return;
			}

			int row, col;
			getCursorPosition(&row, &col);

			if (index == history_.size())
			{
				history_position_ = history_.size();
				removeParagraph(row);
				insertParagraph(getPrompt_(), row);
				setCursorPosition(paragraphs()-1, col);
				QScrollBar* sb = verticalScrollBar();
				if (sb != 0)
				{
					sb->setValue(sb->maxValue());
				}
				removeParagraph(row +1);
				return;
			}

			String line = getPrompt_()+ history_[index];

			// replace the line's contents
			removeParagraph(row);
			insertParagraph(line.c_str(), row);
			setCursorPosition(row, line.size()); 
			QScrollBar* sb = verticalScrollBar();
			if (sb != 0)
			{
				sb->setValue(sb->maxValue());
			}

			// update the history position
			history_position_ = index;
			removeParagraph(row +1);
		}


		bool PyWidgetData::returnPressed()
		{
			// check for an empty line (respect the prompt)
			int row, col;
			getCursorPosition(&row, &col);
			current_line_ = getCurrentLine_();
			QTextEdit::returnPressed();
			if (col < 5)
			{
				if (multi_line_mode_)
				{
					// in multi line mode: end of input - parse it!
					parseLine_();
				}
				else	
				{
					// return on an empty line is handled 
					// as in the interactive interpreter: do nothing and 
					// print another prompt
					newPrompt_();
				}
			} 
			else 
			{	
				// parse the line
				parseLine_();
			}

			return false;
		}


		bool PyWidgetData::parseLine_(String line)
		{
			if (!Py_IsInitialized())
			{
				append("ERROR: no interpreter running!\n");
				return false;
			}

			// check for comments
			String temp(line);
			temp.trim();
			if (temp.hasPrefix("#")) return true;

			history_position_ = history_.size();

			line.trimRight();
			
			if (!multi_line_mode_)
			{
				if (line.isEmpty()) return true;
				if ((line.hasPrefix("for ") 		|| 
						 line.hasPrefix("def ") 		||
						 line.hasPrefix("class ") 	||
						 line.hasPrefix("while ")	  ||
						 line.hasPrefix("if "))
						&& line.hasSuffix(":"))
				{
						multi_line_mode_ = true;
						multi_line_text_ = line;
						multi_line_text_.append("\n");
						multi_lines_ = 1;
						appendToHistory_(line);
						newPrompt_();
						return true;
				}

				multi_lines_ = 0;
				appendToHistory_(line);
			}
			else // Multiline mode
			{
				multi_lines_ += 1;
				appendToHistory_(line);
				if (!line.isEmpty())
				{
					multi_line_text_ += line + "\n";
					newPrompt_();
					return true;
				}

				line = multi_line_text_ + "\n";
			}

			bool state = true;
			bool ok = true;
		  String result;
#ifndef BALL_QT_HAS_THREADS
			result = PyInterpreter::run(line, state);
#else
			thread_->input = line;
			thread_->start();

			while (thread_->running()) 
			{
				qApp->wakeUpGuiThread();
				qApp->processEvents(500);
				thread_->wait(50);
			}

			result = thread_->output;
			state = thread_->state;
#endif

			if (result != "") 
			{
				if (result.hasSubstring("ERROR"))
				{
					setColor(red);
					ok = false;
				}
				else
				{
					setColor(blue);
				}
				append(result.c_str());
				setColor(black);
			}

				
			if (!multi_line_mode_)
			{
				results_.push_back(state);
			}
			else
			{
				for (Position p = 0; p <= multi_lines_ -1; p++) results_.push_back(state);
			}
			
			multi_line_mode_ = false;

			newPrompt_();
			return ok;
		}

		void PyWidgetData::runString(String command)
		{
			if (!command.has('\n'))
			{
				parseLine_(command);
				return;
			}

			vector<String> lines;
			Size nr = command.split(lines, String('\n').c_str());
			for (Position p = 0; p < nr; p++)
			{
				parseLine_(lines[p]);
			}
		}
		

		void PyWidgetData::appendToHistory_(const String& line)
		{
			history_.push_back(line);
			history_position_ = history_.size();
		}

		const char* PyWidgetData::getPrompt_() const
		{
			return (multi_line_mode_ ? "... " : ">>> ");
		}

		void PyWidgetData::newPrompt_()
		{
			append(getPrompt_());
			setCursorPosition(paragraphs() - 1, 4);
		}

		bool PyWidgetData::parseLine_()
		{
			String line = current_line_.getSubstring(4);
			return parseLine_(line);
		}


		void PyWidgetData::keyPressEvent(QKeyEvent* e)
		{
			int row, col;
			getCursorPosition(&row, &col);

			if (row != paragraphs()-1 || col < 4)
			{
				setCursorPosition(paragraphs()-1, paragraphLength(paragraphs() -1) -1);
			}

			if (e->key() == Key_Left || e->key() == Key_Backspace)
			{
				if (col <= 4) return;
			}
			else if (e->key() == Key_Right)
			{
				setCursorPosition(paragraphs()-1, col+1);
				return;
			}
			else if (e->key() == Key_Up)
			{
				if (history_position_ != 0) retrieveHistoryLine_(history_position_ - 1);
				return;
			}
			else if (e->key() == Key_Down)
			{
				retrieveHistoryLine_(history_position_ + 1);
				return;
			}
			else if (e->key() == Key_Home)
			{
				setCursorPosition(row, 4);
				return;
			}
			else if (e->key() == Key_Return)
			{
				if (!returnPressed()) return;
			}
			else if (e->key() == Key_PageUp)
			{
				return;
			}
			else if (e->key() == Key_PageDown)
			{
				return;
			}

			QTextEdit::keyPressEvent(e);
		} 


		void PyWidgetData::dump(std::ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "multiline_mode : " << multi_line_mode_<< std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "multi_line_text  : " << multi_line_text_<< std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "history : "<< std::endl;
			
			for (Position i = 0; i < history_.size(); i++)
			{
				BALL_DUMP_DEPTH(s, depth);
				s << history_[i]<< std::endl;
			}

			BALL_DUMP_DEPTH(s, depth);
			s << "history_position : " << history_position_ << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "current_line : " << current_line_ << std::endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}


		String PyWidgetData::getCurrentLine_()
		{
			int row, col;
			getCursorPosition(&row, &col);
			return String(text(row).ascii());
		}


		void PyWidgetData::runFile(const String& filename)
		{
			stop_script_ = false;
			append(String("> running File " + filename + "\n").c_str());
			LineBasedFile file;
			try
			{
				file.open(filename);
			}
			catch(Exception::GeneralException e)
			{
				append(String("> Could not find file " + filename + "\n").c_str());
				newPrompt_();
				return;
			}

			while (file.readLine())
			{
				if (!parseLine_(file.getLine()))
				{
					String result_string = "> Error in Line " + String(file.getLineNumber()) + " in file " + filename + "\n";
					append(result_string.c_str());
					newPrompt_();
					return;
				}

				if (stop_script_) 
				{
					stop_script_ = false;
					((PyWidget*)parent())->setStatusbarText("Aborted script");
					append("> aborted...");
					newPrompt_();
					return;
				}
			}
			append("> finished...");
			((PyWidget*)parent())->setStatusbarText("finished script");
			newPrompt_();
		}

		void PyWidgetData::scriptDialog()
		{
			PyWidget* p = (PyWidget*) parent();
			// no throw specifier because of that #$%@* moc
			QString s = QFileDialog::getOpenFileName(
										p->getWorkingDir().c_str(),
										"Python Scripts(*.py)",
										p->getMainControl(),
										"Run Python Script",
										"Choose a file" );

		 	if (s == QString::null) return;
			p->setWorkingDirFromFilename_(s.ascii());

			runFile(s.ascii());
		}

		void PyWidgetData::exportHistory()
		{
			PyWidget* p = (PyWidget*) parent();
			QString s = QFileDialog::getOpenFileName(
										p->getWorkingDir().c_str(),
										"",
										p->getMainControl(),
										"Export History",
										"");

		 	if (s == QString::null) return;
			String filename(s.ascii());
			p->setWorkingDirFromFilename_(filename);

			File file(filename, std::ios::out);
			if (!file.isOpen()) 
			{
				append(String("> Could not export history to file " + filename + "\n").c_str());
				newPrompt_();
				return;
			}
					
			for (Position p = 0; p < history_.size(); p++)
			{
				if (results_[p]) file << history_[p] << std::endl;
			}

			file.close();
		}

		void PyWidgetData::cut()
		{
			QTextEdit::cut();
		}

		void PyWidgetData::clear()
		{
			QTextEdit::clear();
			newPrompt_();
		}

		void PyWidgetData::paste()
		{
			int row, col;
			getCursorPosition(&row, &col);

			if (row != paragraphs()-1)
			{
				setCursorPosition(paragraphs()-1, paragraphLength(paragraphs() -1) -1);
			}

			QTextEdit::paste();
		}
// ######################################################################################################

		PyWidget::PyWidget(QWidget *parent, const char *name)
			throw()
			: DockWidget(parent, name),
				text_edit_(new PyWidgetData(this))
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new PyWidget " << this << std::endl;
		#endif
			default_visible_ = false;
			setGuest(*text_edit_);
			registerWidget(this);
		}

		PyWidget::~PyWidget()
			throw()
		{}

		void PyWidget::initializeWidget(MainControl& main_control)
			throw()
		{
			main_control.insertMenuEntry(MainControl::TOOLS_PYTHON, "Restart Python", text_edit_, SLOT(startInterpreter()));
			main_control.insertMenuEntry(MainControl::TOOLS_PYTHON, "Run Python Script", text_edit_, SLOT(scriptDialog()));
			main_control.insertMenuEntry(MainControl::TOOLS_PYTHON, "Abort Python Script", text_edit_, SLOT(abortScript()));
			main_control.insertMenuEntry(MainControl::TOOLS_PYTHON, "Export History", text_edit_, SLOT(exportHistory()));

			DockWidget::initializeWidget(main_control);
		}


		void PyWidget::finalizeWidget(MainControl& main_control)
			throw()
		{
			text_edit_->abortScript();
			stopInterpreter();
			main_control.removeMenuEntry(MainControl::TOOLS_PYTHON, "Restart Python", text_edit_, SLOT(startInterpreter()));
			main_control.removeMenuEntry(MainControl::TOOLS_PYTHON, "Run Python Script", text_edit_, SLOT(scriptDialog()));
			main_control.removeMenuEntry(MainControl::TOOLS_PYTHON, "Abort Python Script", text_edit_, SLOT(abortScript()));
			main_control.removeMenuEntry(MainControl::TOOLS_PYTHON, "Export History", text_edit_, SLOT(exportHistory()));

			DockWidget::finalizeWidget(main_control);
		}


		void PyWidget::fetchPreferences(INIFile& inifile)
			throw()
		{
			DockWidget::fetchPreferences(inifile);

			for (Position p = 0; true; p++)
			{
				if (!inifile.hasEntry("PYTHON", "Hotkey" + String(p))) break;

				Hotkey hotkey;
				if (!hotkey.set(inifile.getValue("PYTHON", "Hotkey" + String(p)))) continue;

				insertHotkey(hotkey);
			}

			python_hotkeys_->setContent(hotkeys_);

			if (!inifile.hasEntry("PYTHON", "StartupScript")) return;

			text_edit_->startup_script_ =	inifile.getValue("PYTHON", "StartupScript");
			text_edit_->python_settings_->setFilename(text_edit_->startup_script_);
			if (text_edit_->startup_script_ != "") text_edit_->runFile(text_edit_->startup_script_);
		}


		void PyWidget::writePreferences(INIFile& inifile)
			throw()
		{
			inifile.appendSection("PYTHON");
			inifile.insertValue("PYTHON", "StartupScript", text_edit_->startup_script_);

			DockWidget::writePreferences(inifile);

			List<Hotkey>::iterator it = hotkeys_.begin();
			for (Position p = 0; it != hotkeys_.end(); it++)
			{
				String data;
				(*it).get(data);
				inifile.insertValue("PYTHON", "Hotkey" + String(p), data);
				p++;
			}

		}


		void PyWidget::initializePreferencesTab(Preferences &preferences)
			throw()
		{
			text_edit_->python_settings_= new PythonSettings();
			text_edit_->python_settings_->setFilename(text_edit_->startup_script_);
			preferences.insertEntry(text_edit_->python_settings_, "Python");
			python_hotkeys_ = new PythonHotkeys();
			preferences.insertEntry(python_hotkeys_, "Hotkeys");
		}

		void PyWidget::finalizePreferencesTab(Preferences &preferences)
			throw()
		{
			if (text_edit_->python_settings_ != 0)
			{
				preferences.removeEntry(text_edit_->python_settings_);
				text_edit_->python_settings_ = 0;
			}

			if (python_hotkeys_ != 0)
			{
				preferences.removeEntry(python_hotkeys_);
				python_hotkeys_ = 0;
			}
		}

		void PyWidget::applyPreferences()
			throw()
		{
			if (text_edit_->python_settings_ == 0) return;
			text_edit_->startup_script_ = text_edit_->python_settings_->getFilename();

			hotkeys_ = (python_hotkeys_->getContent());
		}

		void PyWidget::cancelPreferences()
			throw()
		{
			if (text_edit_->python_settings_ != 0)
			{
				text_edit_->python_settings_->setFilename(text_edit_->startup_script_);
			}
		}

		void PyWidget::startInterpreter()
		{
			text_edit_->startInterpreter();
		}

		void PyWidget::stopInterpreter()
		{
			text_edit_->stopInterpreter();
		}

		void PyWidgetData::abortScript()
		{
			((PyWidget*)(parent()))->setStatusbarText("Aborting Python script");
			 stop_script_ = true;
		}

		PyWidget::PyWidget(const PyWidget& p)
			throw()
		 : DockWidget(p)
		{}

		bool PyWidget::toAbortScript() throw() 
		{
			return text_edit_->stop_script_;
		}

		void PyWidget::insertHotkey(const Hotkey& hotkey) 
			throw()
		{
			List<Hotkey>::iterator it = hotkeys_.begin();
			for (; it != hotkeys_.end(); it++)
			{
				if ((*it) == hotkey) return;
			}

			hotkeys_.push_back(hotkey);
		}

		void PyWidget::removeHotkey(const Hotkey& hotkey) 
			throw()
		{
			List<Hotkey>::iterator it = hotkeys_.begin();
			for (; it != hotkeys_.end(); it++)
			{
				if ((*it) == hotkey) return;
			}

			hotkeys_.erase(it);
		}

		void PyWidget::reactTo(const QKeyEvent& e) 
			throw() 
		{
			List<Hotkey>::iterator it = hotkeys_.begin();
			for (; it != hotkeys_.end(); it++)
			{
				if ((*it) == e) 
				{
					text_edit_->runString((*it).action);
					return;
				}
			}
		}

	} // namespace VIEW
} // namespace BALL
