// $Id: pyWidget.C,v 1.6 2001/12/27 00:32:12 oliver Exp $


#include <BALL/VIEW/GUI/WIDGETS/pyWidget.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <Python.h>

#include <qscrollbar.h>

#include <BALL/VIEW/GUI/KERNEL/mainControl.h>

PyWidget::PyWidget
	(QWidget* parent, const char* name)
	: QMultiLineEdit(parent, name),
		ModularWidget(name)
{
	// register the widget with the MainControl
	ModularWidget::registerWidget(this);
}

PyWidget::PyWidget(const PyWidget& widget)
	:	QMultiLineEdit(),
		ModularWidget(widget)
{
}

PyWidget::~PyWidget()
	throw()
{	
}


void PyWidget::initializeWidget(MainControl& main_control)
{
	main_control.insertMenuEntry
		(MainControl::TOOLS, "&Restart Python", 
		 this, SLOT(startInterpreter()));
}

void PyWidget::finalizeWidget
	(MainControl& main_control)
{
	main_control.removeMenuEntry
		(MainControl::TOOLS, "&Restart Python", 
		 this, SLOT(startInterpreter()));
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
	setText(VersionInfo::getVersion());

	// print the first prompt 
	multi_line_mode_ = false;
	newPrompt_();
	history_position_ = history_.size() + 1;
}

void PyWidget::retrieveHistoryLine_(Position index)
{
	// we do not have a history - something went wrong
	if (history_.size() == 0)
	{
		return;
	}

	int row, col;
	getCursorPosition(&row, &col);
	
	if (history_position_ >= history_.size())
	{
		// store the current line (it is not from history!)
		String line(textLine(row));
		if (line.size() > 4)
		{
			current_line_.set(line, 4);
		}
		else
		{	
			current_line_ = "";
		}
	}
	
	String line = getPrompt_();
	if (index < history_.size())
	{
		line.append(history_[index]);
	}
	else 
	{
		line.append(current_line_);
	}

	// replace the line's contents
	removeLine(row);
	setAutoUpdate(false);
	insertLine(line.c_str(), row);
	setCursorPosition(numLines(), col);
	end();
	QScrollBar* sb = verticalScrollBar();
	if (sb != 0)
	{
		sb->setValue(sb->maxValue());
	}
	setAutoUpdate(true);

	// update the history position
	history_position_ = index;
}


void PyWidget::mousePressEvent(QMouseEvent* /* m */) 
{
	// we ignore the mouse events! 
	// they might place the cursor anywhere!
}

void PyWidget::cursorUp(bool /* mark */)
{
	if (history_position_ > 0)
	{
		retrieveHistoryLine_(history_position_ - 1);
	}
}

void PyWidget::cursorDown(bool /* mark */)
{
	if (history_position_ < history_.size())
	{
		retrieveHistoryLine_(history_position_ + 1);
	}
}

void PyWidget::newLine()
{
	// move the cursor to the end of the line
	end();

	// check for an empty line (respect the prompt)
	int row, col;
	getCursorPosition(&row, &col);
	if (col < 5)
	{
		if (multi_line_mode_ == true)
		{
			// in multi line mode: end of input - parse it!
			QMultiLineEdit::newLine();
			parseLine_();
		}
		else	
		{
			// return on an empty line is handled 
			// as in the interactive interpreter: do nothing and 
			// print another prompt
			QMultiLineEdit::newLine();
			removeLine(numLines() - 1);
			newPrompt_();
		}
	} 
	else 
	{	
		// parse the line
		QMultiLineEdit::newLine();	
		parseLine_();
	}
}

void PyWidget::cursorLeft(bool mark, bool /* wrap */)
{
	// prevent the cursor from deleting the 
	// prompt (">>> ")
	int row, col;
	getCursorPosition(&row, &col);
	if (col > 4)
	{
		QMultiLineEdit::cursorLeft(mark, FALSE);
	}
}

void PyWidget::backspace()
{
	// prevent the cursor from deleting the 
	// prompt (">>> ")
	int row, col;
	getCursorPosition(&row, &col);
	if (col > 4)
	{
		QMultiLineEdit::backspace();
	}
}

void PyWidget::home(bool /* mark */)
{
	// prevent the cursor from getting in
	// front of the prompt (">>> ")
	int row, col;
	getCursorPosition(&row, &col);
	setCursorPosition(row, 4);
}

void PyWidget::cursorRight(bool mark, bool /* wrap */)
{
	QMultiLineEdit::cursorRight(mark, FALSE);
}


void PyWidget::parseLine_()
{
	if (!Py_IsInitialized())
	{
		append("ERROR: no interpreter running!\n");
		return;
	}

	int line_no, row_no;
	getCursorPosition(&line_no, &row_no);
	QString line(textLine(line_no - 1));
	if (line.length() > 4)
	{
		line = line.mid(4);
	}
	if (multi_line_mode_ == true)
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
			removeLine(numLines() - 1);
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

				removeLine(numLines() - 1);
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

	removeLine(numLines() - 1);
	if (result != "")
	{
		append(result.c_str());
		removeLine(numLines() - 1);
	}

	newPrompt_();
}

void PyWidget::appendToHistory_(const char* line)
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
	setCursorPosition(numLines() - 1, 0);
	end();

	emit textChanged();
}
