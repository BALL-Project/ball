/* TRANSLATOR BALL::VIEW::DragLogView

		Necessary for lupdate.
*/


// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#include <QtGui/QTextCursor>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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


		void DragLogView::setSource(const QUrl& /* name */)
		{ }


		LogView::LogView(QWidget *parent, const char *name)
			: DockWidget(parent, name),
				LogStreamNotifier(),
				text_edit_(new DragLogView(this))
		{
			default_visible_ = false;

			// UFG-24 (rc4 validation) — wrap text_edit_ in a container
			// with a small toolbar strip so the user has a
			// discoverable "Copy logs" action without diving into
			// menus or per-line right-click. The container becomes
			// the DockWidget's guest, replacing the bare text_edit_.
			QWidget* container = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(container);
			col->setContentsMargins(4, 2, 4, 2);
			col->setSpacing(2);

			QHBoxLayout* toolbar = new QHBoxLayout();
			toolbar->setContentsMargins(0, 0, 0, 0);
			toolbar->setSpacing(6);
			QPushButton* copy_btn = new QPushButton(tr("Copy logs"), container);
			copy_btn->setToolTip(tr("Copy all log lines to the clipboard."));
			QPushButton* clear_btn = new QPushButton(tr("Clear"), container);
			clear_btn->setToolTip(tr("Clear the log view (does not stop further logging)."));
			toolbar->addWidget(copy_btn);
			toolbar->addWidget(clear_btn);
			toolbar->addStretch(1);
			col->addLayout(toolbar);
			col->addWidget(text_edit_, /*stretch=*/1);

			connect(copy_btn, &QPushButton::clicked, this, [this]() {
				if (QApplication::clipboard())
					QApplication::clipboard()->setText(text_edit_->toPlainText());
			});
			connect(clear_btn, &QPushButton::clicked, text_edit_, &QTextEdit::clear);

			setGuest(*container);
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
 			if (!ct.atEnd()) {
				ct.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
				text_edit_->setTextCursor(ct);
			}

			text_edit_->insertPlainText(text.c_str());

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
			// UFG-24 — second discoverability path (alongside the
			// Copy logs button in the LogView toolbar).
			insertMenuEntry(MainControl::EDIT, tr("Copy Logs to Clipboard"), this,
			                SLOT(copyLogsToClipboard()),
			                "Shortcut|MainControl|Edit|CopyLogs", QKeySequence(), tr(""),
											UIOperationMode::MODE_ADVANCED);
		}

		void LogView::copyLogsToClipboard()
		{
			if (text_edit_ && QApplication::clipboard())
				QApplication::clipboard()->setText(text_edit_->toPlainText());
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
