// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.C,v 1.16 2003/04/01 00:50:27 amoll Exp $

#include <BALL/VIEW/GUI/WIDGETS/pyWidget.h>
#include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#include <BALL/VIEW/GUI/DIALOGS/pythonSettings.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/VIEW/GUI/DIALOGS/preferences.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <Python.h>

#include <qscrollbar.h>
#include <qfiledialog.h>

namespace BALL
{
	namespace VIEW
	{

PyWidget::PyWidget(QWidget* parent, const char* name)
	: QTextEdit(parent, name),
		ModularWidget(name)
{
	// register the widget with the MainControl
	ModularWidget::registerWidget(this);
}

PyWidget::PyWidget(const PyWidget& widget)
	:	QTextEdit(),
		ModularWidget(widget)
{
	ModularWidget::registerWidget(this);
}

PyWidget::~PyWidget()
	throw()
{	
}


void PyWidget::initializeWidget(MainControl& main_control)
{
	main_control.insertMenuEntry(MainControl::TOOLS, "&Restart Python", this, SLOT(startInterpreter()));
	main_control.insertMenuEntry(MainControl::TOOLS, "&Run Python Script", this, SLOT(scriptDialog()));
}


void PyWidget::finalizeWidget(MainControl& main_control)
{
	main_control.removeMenuEntry(MainControl::TOOLS, "&Restart Python", this, SLOT(startInterpreter()));
	main_control.removeMenuEntry(MainControl::TOOLS, "&Run Python Script", this, SLOT(scriptDialog()));
}


void PyWidget::stopInterpreter()
{
	PyInterpreter::finalize();
}


void PyWidget::startInterpreter()
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


void PyWidget::retrieveHistoryLine_(Position index)
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
		setCursorPosition(row, col); 
		QScrollBar* sb = verticalScrollBar();
		if (sb != 0)
		{
			sb->setValue(sb->maxValue());
		}
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
}


void PyWidget::mousePressEvent(QMouseEvent* /* m */) 
{
	// we ignore the mouse events! 
	// they might place the cursor anywhere!
}


bool PyWidget::returnPressed()
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


void PyWidget::parseLine_()
{
	if (!Py_IsInitialized())
	{
		append("ERROR: no interpreter running!\n");
		return;
	}

	history_position_ = history_.size();

	String line = current_line_.getSubstring(4);
	line.trimRight();
	
	if (!multi_line_mode_)
	{
		if (line.isEmpty()) return;
		if ((line.hasPrefix("for ") 		|| 
				 line.hasPrefix("def ") 		||
				 line.hasPrefix("class ") 	||
				 line.hasPrefix("while ")	||
				 line.hasPrefix("if "))
				&& line.hasSuffix(":"))
		{
				multi_line_mode_ = true;
				multi_line_text_ = line;
				multi_line_text_.append("\n");
				appendToHistory_(line);
				newPrompt_();
				return;
		}

		appendToHistory_(line);
	}
	else // Multiline mode
	{
		if (!line.isEmpty())
		{
			appendToHistory_(line);
			multi_line_text_ += line + "\n";
			newPrompt_();
			return;
		}

		line = multi_line_text_ + "\n";
	}

	bool state;
	String result = PyInterpreter::run(line, state);
	multi_line_mode_ = false;

	if (result != "") append(result.c_str());

	newPrompt_();
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

void PyWidget::newPrompt_()
{
	append(getPrompt_());
	setCursorPosition(lines() - 1, 4);
}


void PyWidget::keyPressEvent(QKeyEvent* e)
{
	int row, col;
	getCursorPosition(&row, &col);

	if (e->key() == Key_Left || e->key() == Key_Backspace)
	{
		if (col <= 4) return;
	}
	else if (e->key() == Key_Right)
	{
		setCursorPosition(row, col+1);
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

	QTextEdit::keyPressEvent(e);
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
	s << "current_line : " << current_line_ << std::endl;

	BALL_DUMP_STREAM_SUFFIX(s);
}


String PyWidget::getCurrentLine_()
{
	int row, col;
	getCursorPosition(&row, &col);
	return String(text(row).ascii());
}


void PyWidget::runFile(const String& filename)
{
	append(String("> running File " + filename + "\n").c_str());
	bool result;
	String result_string;
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
		result_string = PyInterpreter::run(file.getLine(), result);
		if (!result)
		{
			result_string += "> Error in Line " + String(file.getLineNumber()) + " in file " + filename + "\n";
			append(result_string.c_str());
			newPrompt_();
			return;
		}

		append(result_string.c_str());
	}
	append("> finished...");
	newPrompt_();
}

void PyWidget::scriptDialog()
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


void PyWidget::fetchPreferences(INIFile& inifile)
	throw()
{
	if (!inifile.hasEntry("PYTHON", "StartupScript")) return;

	startup_script_ =	inifile.getValue("PYTHON", "StartupScript");
	python_settings_->setFilename(startup_script_);
	runFile(startup_script_);
}


void PyWidget::writePreferences(INIFile& inifile)
	throw()
{
	inifile.appendSection("PYTHON");
	inifile.insertValue("PYTHON", "StartupScript", startup_script_);
}


void PyWidget::initializePreferencesTab(Preferences &preferences)
	throw()
{
	python_settings_= new PythonSettings(this);
	python_settings_->setFilename(startup_script_);
	CHECK_PTR(python_settings_);

	preferences.insertTab(python_settings_, "Python");
}


void PyWidget::finalizePreferencesTab(Preferences &preferences)
	throw()
{
	if (python_settings_ != 0)
	{
		preferences.removeTab(python_settings_);

		delete python_settings_;
		python_settings_ = 0;
	}
}


void PyWidget::applyPreferences(Preferences & /* preferences */)
	throw()
{
	if (python_settings_ == 0) return;
	startup_script_ = python_settings_->getFilename();
}


void PyWidget::cancelPreferences(Preferences&)
	throw()
{
	if (python_settings_ != 0)
	{
		python_settings_->setFilename(startup_script_);
	}
}


} } // namespace
