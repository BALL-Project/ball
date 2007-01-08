// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:
//

#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <qdragobject.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		DragLogView::DragLogView(QWidget* parent)
			: QTextEdit(parent)
		{
		}


		void DragLogView::contentsDragEnterEvent(QDragEnterEvent * e)
		{
			e->accept(QTextDrag::canDecode(e));
			setReadOnly(false);
		}

		void DragLogView::contentsDragLeaveEvent(QDragLeaveEvent*)
		{
			setReadOnly(true);
		}


		void DragLogView::contentsDropEvent(QDropEvent *e)
		{
			VIEW::processDropEvent(e);
			setReadOnly(true);
		}


		LogView::LogView(QWidget *parent, const char *name)
			throw()
			: DockWidget(parent, name),
				LogStreamNotifier(),
				text_edit_(new DragLogView(this)),
				output_running_(false)
		{
			default_visible_ = false;
			setGuest(*text_edit_);
			registerWidget(this);
		}

		LogView::LogView(const LogView& view)
			throw()
			: DockWidget((QWidget*)view.getParent()),
				LogStreamNotifier(),
				text_edit_(new DragLogView(this)),
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
										<< " of class LogView" << endl;
			#endif 
		}

		void LogView::logNotify() 
		{
			if (output_running_) return;
			output_running_ = true;
			char c;
			stream_.get(c);

			String line;
			while (stream_.gcount() > 0)
			{
				line += c;
				stream_.get(c);
			}

			stream_.clear();

			if (line.size() > 0)
			{
				text_edit_->append(line.c_str());
				text_edit_->scrollToBottom();
			}

			output_running_ = false;
			return;
		}

		void LogView::initializeWidget(MainControl& main_control)
			throw()
		{
			registerAt(Log);
			text_edit_->setReadOnly(true);
			text_edit_->setTextFormat(PlainText);

			DockWidget::initializeWidget(main_control);
			insertMenuEntry(MainControl::EDIT, "Clear Logs", text_edit_, SLOT(clear()));

			registerWidgetForHelpSystem(this, "logView.html");

			setMinimumSize(10, 10);
		}


		void LogView::finalizeWidget(MainControl& main_control)
			throw()
		{
			DockWidget::finalizeWidget(main_control);
			LogStreamNotifier::unregister();
		}

	} // VIEW
} // namespace BALL
