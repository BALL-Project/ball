// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:
//

#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <QtGui/QTextCursor>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		DragLogView::DragLogView(QWidget* parent)
			: QTextEdit(parent)
		{
		}


		void DragLogView::contentsDragEnterEvent(QDragEnterEvent* e)
		{
			if (e->mimeData()->hasUrls()) e->acceptProposedAction();
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
 			text_edit_->setLineWrapMode(QTextEdit::WidgetWidth);
			text_edit_->setAcceptRichText(false);
			text_edit_->setReadOnly(true);
			resize(300, 100);
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
 				setUpdatesEnabled(false);
 				QTextCursor ct = text_edit_->textCursor();
 				if (!ct.atEnd()) 
 				{
 					ct.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
					text_edit_->setTextCursor(ct);
 				}
					
 				text_edit_->insertPlainText(line.c_str());
 				ct.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
 				text_edit_->setTextCursor(ct);
 				text_edit_->ensureCursorVisible();
 				setUpdatesEnabled(true);
			}

			output_running_ = false;
			return;
		}

		void LogView::initializeWidget(MainControl& main_control)
			throw()
		{
			registerAt(Log);
			text_edit_->setReadOnly(true);

			DockWidget::initializeWidget(main_control);
			insertMenuEntry(MainControl::EDIT, "Clear Logs", text_edit_, SLOT(clear()));

			registerForHelpSystem(this, "logView.html");

			setMinimumSize(10, 10);
		}


		void LogView::finalizeWidget(MainControl& main_control)
			throw()
		{
			DockWidget::finalizeWidget(main_control);
			LogStreamNotifier::unregister();
		}

		void LogView::showGuestContextMenu(const QPoint& pos)
		{
			QMenu* menu = text_edit_->createStandardContextMenu();
			menu->exec(mapToGlobal(pos));
			delete menu;
		}

	} // VIEW
} // namespace BALL
