// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.C,v 1.14 2003/03/28 19:38:50 amoll Exp $

#include <BALL/VIEW/GUI/WIDGETS/pyWidget.h>
#include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <Python.h>

#include <qscrollbar.h>
#include <qnamespace.h> 

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
}

PyWidget::~PyWidget()
	throw()
{	
}


void PyWidget::initializeWidget(MainControl& main_control)
{
	main_control.insertMenuEntry(MainControl::TOOLS, "&Restart Python", this, SLOT(startInterpreter()));
}


void PyWidget::finalizeWidget(MainControl& main_control)
{
	main_control.removeMenuEntry(MainControl::TOOLS, "&Restart Python", this, SLOT(startInterpreter()));
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


bool PyWidget::cursorUp()
{
	if (history_position_ == 0) return false;
	retrieveHistoryLine_(history_position_ - 1);
	return false;
}


bool PyWidget::cursorDown()
{
	retrieveHistoryLine_(history_position_ + 1);
	return false;
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

	String result = PyInterpreter::run(line);
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

	emit textChanged();
}


void PyWidget::keyPressEvent(QKeyEvent* e)
{
	int row, col;
	getCursorPosition(&row, &col);
	bool qt_continue = true;

	if (e->key() == Key_Left || e->key() == Key_Backspace)
	{
		qt_continue = (col > 4);
	}
	else if (e->key() == Key_Right)
	{
		setCursorPosition(row, col+1);
		qt_continue = false;
	}
	else if (e->key() == Key_Up)
	{
		qt_continue = cursorUp();
	}
	else if (e->key() == Key_Down)
	{
		qt_continue = cursorDown();
	}
	else if (e->key() == Key_Home)
	{
		setCursorPosition(row, 4);
		qt_continue = false;
	}
	else if (e->key() == Key_Return)
	{
		qt_continue = returnPressed();
	}

	if (qt_continue)
	{
		QTextEdit::keyPressEvent(e);
	}
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

} } // namespace
