// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:
//

#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{
  

LogView::LogView(QWidget *parent, const char *name)
	throw()
	: DockWidget(parent, name),
		NotificationTarget<LogStreamNotifier>(),
		text_edit_(new QTextEdit(this)),
		strstream_(),
		output_running_(false)
{
	default_visible_ = false;
	setGuest(*text_edit_);
  registerWidget(this);
}

LogView::LogView(const LogView& view)
	throw()
	: DockWidget((QWidget*)view.getParent()),
		NotificationTarget<LogStreamNotifier>(),
		text_edit_(new QTextEdit(this)),
		strstream_(),
		output_running_(false)
{
	default_visible_ = false;
	setGuest(*text_edit_);
}

LogView::~LogView()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.error() << "Destructing object " << (void *)this 
								<< " of class " << RTTI::getName<LogView>() << endl;
	#endif 
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
		text_edit_->append(line.c_str());
		text_edit_->scrollToBottom();
	}

	output_running_ = false;
	return true;
}

void LogView::initializeWidget(MainControl& main_control)
	throw()
{
	Log.insert(strstream_);
	Log.insertNotification(strstream_, *this);
	text_edit_->setReadOnly(TRUE);
	text_edit_->setTextFormat(PlainText);

	DockWidget::initializeWidget(main_control);
	main_control.insertMenuEntry(MainControl::EDIT, "Clear Logs", text_edit_, SLOT(clear()));
}


void LogView::finalizeWidget(MainControl& main_control)
	throw()
{
	DockWidget::finalizeWidget(main_control);
	main_control.removeMenuEntry(MainControl::EDIT, "Clear Logs", text_edit_, SLOT(clear()));
	Log.remove(strstream_);
}

} } // namespaces
