// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyWidget.C,v 1.11 2003/03/27 17:57:33 amoll Exp $

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
Log.error() << "retrieveHistoryLine_" << std::endl;
	if (index >= history_.size()) return;

	int row, col;
	getCursorPosition(&row, &col);
	
	// store the current line (it is not from history!)
	String line = getCurrentLine_();
	if (line.size() > 4)
	{
		current_line_.set(line, 4);
	}
	else
	{	
		current_line_ = "";
	}
	// append ?????
	
	line = getPrompt_()+ history_[index];

	// replace the line's contents
	removeParagraph(row);
	insertParagraph(line.c_str(), row);
	setCursorPosition(row, col); 
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
Log.error() << "returnPressed" << std::endl;
	// check for an empty line (respect the prompt)
	int row, col;
	getCursorPosition(&row, &col);
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
			current_line_ = getCurrentLine_();
			QTextEdit::returnPressed();
			newPrompt_();
		}
	} 
	else 
	{	
		// parse the line
		current_line_ = getCurrentLine_();
		QTextEdit::returnPressed();	
		parseLine_();
	}

	return false;
}


void PyWidget::parseLine_()
{
Log.error() << "parseLine_" << std::endl;
	if (!Py_IsInitialized())
	{
		append("ERROR: no interpreter running!\n");
		return;
	}

	QString line = current_line_.c_str();

	if (line.length() > 4)
	{
		line = line.mid(4);
	}
	else
	{
		line = "";
	}

	if (multi_line_mode_)
	{
		if (line.isEmpty())
		{
			line = multi_line_text_.c_str();
			line.append("\n");
		}
		else
		{
			appendToHistory_(line);
			multi_line_text_.append(line);
			multi_line_text_.append("\n");
			newPrompt_();
			return;
		}
	}
	else
	{
		if (line.isEmpty()) return;

		if (((line.length() > 3) && ((line.left(3) == "for") || (line.left(3) == "def")) &&  line[3].isSpace())
				|| ((line.length() > 4) && (line.left(4) == "class") && line[4].isSpace())
				|| ((line.length() > 5) && (line.left(5) == "while") && line[5].isSpace())
				|| ((line.length() > 2) && (line.left(2) == "if") && line[2].isSpace()))
		{
			line.simplifyWhiteSpace();
			if (line.right(1) == ":")
			{
				multi_line_mode_ = true;
				multi_line_text_ = line;
				multi_line_text_.append("\n");
				appendToHistory_(line);

				newPrompt_();
				return;
			}
		}
	}

	if (line.isEmpty())
	{
		return;			
	}

	// store single lines only - lines from 
	// multi line input are already in the history!
	if (!multi_line_mode_)
	{
		appendToHistory_(line);
	}

	String result = PyInterpreter::run(line.ascii());
	multi_line_mode_ = false;

	if (result != "")
	{
		append(result.c_str());
	}

	newPrompt_();
}

void PyWidget::appendToHistory_(const QString& line)
{
Log.error() << "appendToHistory_" << std::endl;
	history_.push_back(String(line));
	history_position_ = history_.size();
}

const char* PyWidget::getPrompt_() const
{
	return (multi_line_mode_ ? "... " : ">>> ");
}

void PyWidget::newPrompt_()
{
Log.error() << "newPrompt_" << std::endl;
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
	else if (e->key() == Key_Print)
	{
		Log.error() << "print " << std::endl; 
		dump();
	}
	else if (e->key() == Key_Escape)
	{
		Log.error() << "||: " << getCurrentLine_() << "|"<< std::endl;
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
	return String(text(row));
}



} } // namespace
