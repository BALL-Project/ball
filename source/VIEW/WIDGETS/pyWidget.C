// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.C,v 1.25 2004/04/19 16:51:12 amoll Exp $
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
#include <qfiledialog.h>

namespace BALL
{
	namespace VIEW
	{

		PyWidgetData::PyWidgetData(QWidget* parent, const char* name)
			: QTextEdit(parent, name),
				stop_script_(false)
		{
			setWrapPolicy(QTextEdit::Anywhere);
		}

		PyWidgetData::PyWidgetData(const PyWidgetData& /*widget*/)
			:	QTextEdit(),
				stop_script_(false)
		{
		}

		PyWidgetData::~PyWidgetData()
			throw()
		{	
		}


		void PyWidgetData::stopInterpreter()
		{
			PyInterpreter::finalize();
		}


		void PyWidgetData::startInterpreter()
		{
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
			if (col < 5)
			{
				if (multi_line_mode_)
				{
					// in multi line mode: end of input - parse it!
					QTextEdit::returnPressed();
					parseLine_();
				}
				else	
				{
					// return on an empty line is handled 
					// as in the interactive interpreter: do nothing and 
					// print another prompt
					QTextEdit::returnPressed();
					newPrompt_();
				}
			} 
			else 
			{	
				// parse the line
				QTextEdit::returnPressed();	
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

			if (stop_script_) 
			{
				stop_script_ = false;
				return true;
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

			bool ok = true;
			bool state;
			String result = PyInterpreter::run(line, state);
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
			stop_script_ = false;
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
			}
			append("> finished...");
			newPrompt_();
		}

		void PyWidgetData::scriptDialog()
		{
			// no throw specifier because of that #$%@* moc
			QFileDialog *fd = new QFileDialog(this, "Run Python Script", true);
			fd->setMode(QFileDialog::ExistingFile);
			fd->addFilter("Python Scripts(*.py)");
			fd->setSelectedFilter(1);

			fd->setCaption("Run Python Script");
			fd->setViewMode(QFileDialog::Detail);
			fd->setGeometry(300, 150, 400, 400);

			int result_dialog = fd->exec();
			if (!result_dialog == QDialog::Accepted) return;

			String filename(fd->selectedFile().ascii());

			runFile(filename);
		}

		void PyWidgetData::exportHistory()
		{
			QFileDialog *fd = new QFileDialog(this, "Export History", true);
			fd->setMode(QFileDialog::AnyFile);
			fd->addFilter("Python Scripts(*.py)");
			fd->setSelectedFilter(1);

			fd->setCaption("Export History");
			fd->setViewMode(QFileDialog::Detail);
			fd->setGeometry(300, 150, 400, 400);

			int result_dialog = fd->exec();
			if (!result_dialog == QDialog::Accepted) return;

			String filename(fd->selectedFile().ascii());

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
			main_control.removeMenuEntry(MainControl::TOOLS_PYTHON, "Restart Python", text_edit_, SLOT(startInterpreter()));
			main_control.removeMenuEntry(MainControl::TOOLS_PYTHON, "Run Python Script", text_edit_, SLOT(scriptDialog()));
			main_control.removeMenuEntry(MainControl::TOOLS_PYTHON, "Export History", text_edit_, SLOT(exportHistory()));

			DockWidget::finalizeWidget(main_control);
		}


		void PyWidget::fetchPreferences(INIFile& inifile)
			throw()
		{
			DockWidget::fetchPreferences(inifile);

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
		}


		void PyWidget::initializePreferencesTab(Preferences &preferences)
			throw()
		{
			text_edit_->python_settings_= new PythonSettings(this);
			text_edit_->python_settings_->setFilename(text_edit_->startup_script_);
			preferences.insertPage(text_edit_->python_settings_, "Python");
		}

		void PyWidget::finalizePreferencesTab(Preferences &preferences)
			throw()
		{
			if (text_edit_->python_settings_ != 0)
			{
				preferences.removePage(text_edit_->python_settings_);

				delete text_edit_->python_settings_;
				text_edit_->python_settings_ = 0;
			}
		}

		void PyWidget::applyPreferences(Preferences & /* preferences */)
			throw()
		{
			if (text_edit_->python_settings_ == 0) return;
			text_edit_->startup_script_ = text_edit_->python_settings_->getFilename();
		}

		void PyWidget::cancelPreferences(Preferences&)
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

	} // namespace VIEW
} // namespace BALL
