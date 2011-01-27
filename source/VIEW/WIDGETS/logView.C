/* TRANSLATOR BALL::VIEW::DragLogView

		Necessary for lupdate.
*/


// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <QtGui/QTextCursor>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		DragLogView::DragLogView(QWidget* parent)
			: QTextBrowser(parent)
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


		void DragLogView::setSource(const QUrl& name)
		{
			MainControl* mc = getMainControl();
			ShowHelpMessage* msg = new ShowHelpMessage(ascii(name.toString()));
			if (mc) mc->sendMessage(*msg);
		}


		LogView::LogView(QWidget *parent, const char *name)
			: DockWidget(parent, name),
				LogStreamNotifier(),
				text_edit_(new DragLogView(this))
		{
			default_visible_ = false;
			setGuest(*text_edit_);
 			text_edit_->setLineWrapMode(QTextEdit::WidgetWidth);
			text_edit_->setAcceptRichText(false);
			text_edit_->setReadOnly(true);
			registerWidget(this);
 			qApp->installEventFilter(this);
		}

		LogView::LogView(const LogView& view)
			: DockWidget((QWidget*)view.getParent()),
				LogStreamNotifier(),
				text_edit_(new DragLogView(this))
		{
			default_visible_ = false;
			setGuest(*text_edit_);
		}

		LogView::~LogView()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class LogView" << endl;
			#endif 
		}

		void LogView::logNotify() 
		{
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
				LogEvent* su = new LogEvent;
				su->setMessage(line);
				su->setShowOnlyInLogView(true);
				qApp->postEvent(getMainControl(), su);  // Qt will delete it when done
			}
		}

		void LogView::logString(const String& text)
		{
 			setUpdatesEnabled(false);
 			QTextCursor ct = text_edit_->textCursor();
 			if (!ct.atEnd()) 
 			{
 				ct.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
				text_edit_->setTextCursor(ct);
 			}
					
			if (!text.hasSubstring("href"))
			{
				text_edit_->insertPlainText(text.c_str());
			}
			else
			{
				text_edit_->insertHtml(text.c_str());
				text_edit_->insertHtml("<br>");
			}

 			ct.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
 			text_edit_->setTextCursor(ct);
 			text_edit_->ensureCursorVisible();
 			setUpdatesEnabled(true);
		}

		void LogView::initializeWidget(MainControl& main_control)
		{
			registerAt(Log);
			text_edit_->setReadOnly(true);

			DockWidget::initializeWidget(main_control);
			insertMenuEntry(MainControl::EDIT, tr("Clear Logs"), text_edit_, SLOT(clear()),
			                "Shortcut|MainControl|Edit|ClearLogs", QKeySequence(), tr(""),
											UIOperationMode::MODE_ADVANCED);

			registerForHelpSystem(this, "logView.html");
		}


		void LogView::finalizeWidget(MainControl& main_control)
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

		bool LogView::eventFilter(QObject*, QEvent* e) 
		{
			if (e->type() == (QEvent::Type)LOG_EVENT)
			{
				LogEvent* so = dynamic_cast<LogEvent*>(e);
				logString(so->getMessage());
				if (!so->showOnlyInLogView()) getMainControl()->setStatusbarText(so->getMessage(), so->isImportant());
				return true;
			}

			return false;
		}
			
	} // VIEW
} // namespace BALL
