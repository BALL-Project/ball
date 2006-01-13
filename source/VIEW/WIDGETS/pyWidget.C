// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.C,v 1.49.2.2 2006/01/13 16:20:51 amoll Exp $
//

// This include has to be first in order to avoid collisions.
#include <Python.h>

#include <BALL/VIEW/WIDGETS/pyWidget.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DIALOGS/pythonSettings.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/FORMAT/lineBasedFile.h>

#include <qscrollbar.h>
#include <QFileDialog>
#include <qapplication.h>
#include <QKeyEvent>
#include <QDropEvent>
#include <QTextCursor>

// currently doesnt work right
//#undef BALL_QT_HAS_THREADS

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
							button_state 	== e.modifiers()	);
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
				button_state = (Qt::KeyboardModifiers) fields[1].toUnsignedInt();
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

		void MyLineEdit::keyPressEvent(QKeyEvent* e) 
		{ 
			if (e->key() == Qt::Key_Up ||
			    e->key() == Qt::Key_Down)
			{
				pw_->keyPressed(e);
				return;
			}

			QLineEdit::keyPressEvent(e);
		}

		//
// ######################################################################################################

		PyWidget::PyWidget(QWidget *parent, const char *name)
			throw()
			: DockWidget(parent, name),
				text_edit_(new QTextEdit(this)),
				line_edit_(new QLineEdit(this)),
				thread_(0),
				working_dir_(""),
				valid_(false),
				started_startup_script_(false),
				stop_script_(false)
		{
		#ifdef BALL_VIEW_DEBUG
			Log.error() << "new PyWidget " << this << std::endl;
		#endif
			default_visible_ = false;
			setGuest(*text_edit_);
			registerWidget(this);
			text_edit_->setReadOnly(true);

//    			QGridLayout* lay = new QGridLayout();
//   			((QGridLayout*)layout())->addLayout(lay, 2, 0);
//   			line_edit_ = new QLineEdit(this);
//   			lay->addWidget(text_edit_,0, 0, 1, -1);
//   			lay->addWidget(line_edit_,1, 0, 1, -1);
		}

		PyWidget::~PyWidget()
			throw()
		{}

		void PyWidget::initializeWidget(MainControl& main_control)
			throw()
		{
//   			insertMenuEntry(MainControl::TOOLS_PYTHON, "Restart Python", text_edit_, SLOT(startInterpreter()));

			DockWidget::initializeWidget(main_control);
			registerForHelpSystem(this, "pythonInterpreter.html");

			QAction* id1 = insertMenuEntry(MainControl::TOOLS_PYTHON, "Run Python Script", this , SLOT(scriptDialog()));
			QAction* id2 = insertMenuEntry(MainControl::TOOLS_PYTHON, "Abort Python Script", text_edit_, SLOT(abortScript()));
			QAction* id3 = insertMenuEntry(MainControl::TOOLS_PYTHON, "Export History", text_edit_, SLOT(exportHistory()));

			startInterpreter();

			valid_ = PyInterpreter::isValid();
	
			if (!valid_)
			{
				id1->setEnabled(false);
				id2->setEnabled(false);
				id3->setEnabled(false);
				appendText("No Python support available:");
				runString("import BALL");
				setEnabled(false);
				setStatusbarText("No Python support available! (See PyWidget)", true);
				return;
			}
		}


		void PyWidget::finalizeWidget(MainControl& main_control)
			throw()
		{
			abortScript();
			PyInterpreter::finalize();

			DockWidget::finalizeWidget(main_control);
		}


		void PyWidget::initializePreferencesTab(Preferences &preferences)
			throw()
		{
			python_settings_= new PythonSettings();
			preferences.insertEntry(python_settings_);
			preferences_ = &preferences;
		}

		void PyWidget::finalizePreferencesTab(Preferences &preferences)
			throw()
		{
			if (python_settings_ != 0)
			{
				preferences.removeEntry(python_settings_);
				python_settings_ = 0;
			}
		}

		void PyWidget::applyPreferences()
			throw()
		{
			DockWidget::applyPreferences();

			if (python_settings_ == 0)
			{
				return;	
			}

 			hotkeys_ = (python_settings_->getContent());

			QMenu* menu = getMainControl()->initPopupMenu(MainControl::USER);
			menu->clear();

			List<Hotkey>::Iterator it = hotkeys_.begin();
			for (; it != hotkeys_.end(); it++)
			{
				String entry = (*it).action;
				if (entry.size() > 35)
				{ 
					entry.truncate(32);
					entry += "...";
				}

 				QAction* action = insertMenuEntry(MainControl::USER, entry.c_str(), this, SLOT(hotkeyItem()));
				action->setData((*it).action.c_str());
				setMenuHint("Perform a user defined action per Python interface");
				setMenuHelp("pythonInterpreter.html#create_hotkeys");
			}

			getMainControl()->insertPopupMenuSeparator(MainControl::USER);
 			insertMenuEntry(MainControl::USER, "Modify", this, SLOT(modifyHotkeys()));
			setMenuHint("Manage user defined Python commands");
			setMenuHelp("pythonInterpreter.html#create_hotkeys");

			/////////////////////////////////////////
			// startup script
			/////////////////////////////////////////
			if (started_startup_script_ || !isValid())
			{
				return;
			}

			started_startup_script_ = true;

			String startup = getDataPath() + "startup.py";
			if (!runFile(startup))
			{
				Log.error() << "Could not find startup script. Please set the correct path to the data path!" << std::endl;
				Log.error() << "To do so set the environment variable BALL_DATA_PATH or BALLVIEW_DATA_PATH." << std::endl;
			}
			
			String user_startup = python_settings_->getFilename();
			if (user_startup == "") 
			{
				return;	
			}

			runFile(user_startup);
		}

		void PyWidget::modifyHotkeys()
		{
 			preferences_->showEntry(python_settings_->widget_stack->widget(1));
			preferences_->show();
		}

		void PyWidget::hotkeyItem()
		{
			if (sender() == 0) return;

			QAction* action = dynamic_cast<QAction*>(sender());

			if (action == 0) return;

			runString(ascii(action->data().toString()));
		}

		void PyWidget::abortScript()
		{
			 setStatusbarText("Aborting Python script");
			 stop_script_ = true;
		}

		PyWidget::PyWidget(const PyWidget& p)
			throw()
		 : DockWidget(p)
		{}

		bool PyWidget::toAbortScript() throw() 
		{
			return stop_script_;
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
			// doesnt work, no idea yet why:
			/*
			if (getMainControl()->compositesAreLocked() ||
					getMainControl()->getPrimitiveManager().updateRunning())
			{
				return;
			}
			*/

			List<Hotkey>::iterator it = hotkeys_.begin();
			for (; it != hotkeys_.end(); it++)
			{
				if ((*it) == e) 
				{
					runString((*it).action);
					return;
				}
			}
		}
		
		bool PyWidget::runAgain()
		{
			return runFile(last_script_);
		}

		void PyWidget::setError_(bool state)
		{
			QColor color;
			if (state)
			{
				color = Qt::red;
			}
			else
			{
				color = Qt::black;
			}

			text_edit_->setTextColor(color);
		}

		void PyWidget::appendText(const String& text)
		{
 			text_edit_->setUpdatesEnabled(false);
 			QTextCursor ct = text_edit_->textCursor();
 			if (!ct.atEnd()) 
 			{
 				ct.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
				text_edit_->setTextCursor(ct);
 			}
					
 			text_edit_->insertPlainText(text.c_str());
 			ct.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
 			text_edit_->setTextCursor(ct);
 			text_edit_->ensureCursorVisible();
 			text_edit_->setUpdatesEnabled(true);
		}

		void PyWidget::contentsDragEnterEvent(QDragEnterEvent * e)
		{
//   			e->accept(Q3TextDrag::canDecode(e));
		}

		void PyWidget::contentsDropEvent(QDropEvent *e)
		{
//   			VIEW::processDropEvent(e);
		}

		void PyWidget::exportHistory()
		{
			QString s = QFileDialog::getSaveFileName(
										0, "Export History",
										getWorkingDir().c_str(), "");

		 	if (s == QString::null) return;
			String filename(ascii(s));
			setWorkingDirFromFilename_(filename);

			File file(filename, std::ios::out);
			if (!file.isOpen()) 
			{
				appendText(String("> Could not export history to file " + filename + "\n").c_str());
				newPrompt_();
				return;
			}
					
			for (Position p = 0; p < history_.size(); p++)
			{
				if (results_[p]) file << history_[p] << std::endl;
			}

			file.close();
		}


		void PyWidget::clear()
			throw()
		{
			text_edit_->clear();
			newPrompt_();
		}

		void PyWidget::paste()
		{
			line_edit_->paste();
		}

		void PyWidget::scriptDialog()
		{
			if (working_dir_ == "") working_dir_ = getWorkingDir();

			QString s = QFileDialog::getOpenFileName(
										0, "Choose a Python script",
										working_dir_.c_str(),
										"Python Scripts(*.py)");

		 	if (s == QString::null) return;
			setWorkingDirFromFilename_(ascii(s));
			working_dir_ = getWorkingDir();

			runFile(ascii(s));
		}

		String PyWidget::getCurrentLine() const
		{
			return String(ascii(line_edit_->text()));
		}

		bool PyWidget::runFile(const String& filename)
			throw()
		{
			last_script_ = filename;
			stop_script_ = false;
			appendText(String("> executing script from " + filename + "\n").c_str());
			LineBasedFile file;
			try
			{
				file.open(filename);
			}
			catch	(...)
			{
				appendText(String("> Could not find file " + filename + "\n").c_str());
				newPrompt_();
				return false;
			}

			while (file.readLine())
			{
				// Call parse line with 'silent = true' to make sure we do not get prompts
				// for each line read.
				if (!parseLine_(file.getLine(), true))
				{
					String result_string = "> Error in line " + String(file.getLineNumber()) + " of file " + filename + "\n";
					appendText(result_string.c_str());
					newPrompt_();
					return false;
				}

				if (stop_script_) 
				{
					stop_script_ = false;
 					setStatusbarText("Aborted script");
					appendText("> aborted...");
					newPrompt_();
					return false;
				}
			}
			appendText("> Finished.");
			setStatusbarText("Finished script.");
			newPrompt_();
			return true;
		}

		void PyWidget::dump(std::ostream& s, Size depth) const
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
			s << "current_line : " << getCurrentLine() << std::endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void PyWidget::newPrompt_()
		{
			appendText(getPrompt_());
//   			setCursorPosition(paragraphs() - 1, 4);
		}

		bool PyWidget::parseLine_()
		{
			return parseLine_(getCurrentLine());
		}

		void PyWidget::keyPressed(QKeyEvent* e)
		{
			if (e->key() == Qt::Key_Up)
			{
				if (history_position_ != 0) retrieveHistoryLine_(history_position_ - 1);
				return;
			}
			else if (e->key() == Qt::Key_Down)
			{
				retrieveHistoryLine_(history_position_ + 1);
				return;
			}
			/*
			else if (e->key() == Qt::Key_Home)
			{
				setCursorPosition(row, 4);
				return;
			}
			else if (e->key() == Qt::Key_Return)
			{
				if (!returnPressed()) return;
			}
			else if (e->key() == Qt::Key_PageUp)
			{
				return;
			}
			else if (e->key() == Qt::Key_PageDown)
			{
				return;
			}
*/
		} 

		bool PyWidget::parseLine_(String line, bool silent)
		{
			appendText(line);

			if (!Py_IsInitialized())
			{
				appendText("ERROR: no interpreter running!\n");
				return false;
			}

			// check for comments
			String temp(line);
			temp.trim();
			if (temp.hasPrefix("#")) return true;

			history_position_ = history_.size();

			line.trimRight();

			if (line == "quit" || line == "quit()")
			{
				stop_script_ = true;
				PyInterpreter::finalize();
				getMainControl()->quit();
				return true;
			}

			if (line.hasPrefix("run("))
			{
				// This code is probably the worst I've seen in a long, long time...
				// Needs to be replaced by something integrated with Py language concepts
				// in the future (e.g. calling run with a variable argument won't work!) -- OK -- 11/2006
				vector<String> tokens;
				Size nr = line.split(tokens, String("(\"')").c_str());
				if (nr < 2)		
				{
					return false;
				}
				PyInterpreter::runFile(tokens[1]);
				appendToHistory_(line);
				
				return true;
			}
			
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
						if (!silent)
						{
							newPrompt_();
						}
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
					if (!silent)
					{
						newPrompt_();	
					}
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
					setError_(true);
					ok = false;
				}
				else
				{
					setError_(false);
				}
				appendText(result.c_str());
				setError_(false);
			}

				
			if (!multi_line_mode_)
			{
				results_.push_back(state);
			}
			else
			{
				for (Position p = 0; p <= multi_lines_ -1; p++)
				{
					results_.push_back(state);
				}
			}
			
			multi_line_mode_ = false;

			if (!silent)
			{
				newPrompt_();
			}
			return ok;
		}

		bool PyWidget::runString(String command)
		{
			if (!command.has('\n'))
			{
				return parseLine_(command);
			}

			vector<String> lines;
			Size nr = command.split(lines, String('\n').c_str());
			for (Position p = 0; p < nr; p++)
			{
				if (!parseLine_(lines[p])) return false;
			}

			return true;
		}
		

		void PyWidget::appendToHistory_(const String& line)
		{
			history_.push_back(line);
			history_position_ = history_.size();
		}

		const char* PyWidget::getPrompt_() const
		{
			return (multi_line_mode_ ? "... " : ">>> ");
		}

		void PyWidget::retrieveHistoryLine_(Position index)
		{
			if (index > history_.size()) 
			{
				history_position_ = history_.size();
				return;
			}
			
			line_edit_->setText(history_[index].c_str());
		}

		bool PyWidget::returnPressed()
		{
			if (multi_line_mode_) parseLine_(ascii(line_edit_->text()));
			else 									newPrompt_();

			return true;
		}

		void PyWidget::startInterpreter()
		{
			stop_script_ = false;
			// initialize the interpreter
 			PyInterpreter::initialize();

			if (!PyInterpreter::isValid())
			{
				return;
			}

			// print the PyBALL version and clear
			// the widget's contents in case of a restart
			text_edit_->clear();
			appendText((String("BALL ") + VersionInfo::getVersion()).c_str());

			// print the first prompt 
			multi_line_mode_ = false;
			newPrompt_();
			history_position_ = history_.size() + 1;
		}


	} // namespace VIEW
} // namespace BALL
