// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:
//

#include <BALL/VIEW/WIDGETS/helpViewer.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/SYSTEM/path.h>

#include <qpopupmenu.h>
#include <qcursor.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		MyTextBrowser::MyTextBrowser(QWidget* parent, const char*)
			: QTextBrowser(parent),
				forward_(false),
				backward_(false)
		{
			connect(this, SIGNAL(backwardAvailable(bool)), this, SLOT(setBackwardAvailable(bool)));
			connect(this, SIGNAL(forwardAvailable(bool)), this, SLOT(setForwardAvailable(bool)));
		}

		QPopupMenu* MyTextBrowser::createPopupMenu(const QPoint&)
		{
			QPopupMenu* cm = new QPopupMenu(this);
			cm->insertItem("Home", this, SLOT(home()));

			Index back 		= cm->insertItem("Back", this, SLOT(backward()));
			cm->setItemEnabled(back, backward_);

			Index forward = cm->insertItem("Forward", this, SLOT(forward()));
			cm->setItemEnabled(forward, forward_);

			cm->insertSeparator();
			Index copy_a = cm->insertItem("Copy", this, SLOT(copy()));
			cm->setItemEnabled(copy_a, hasSelectedText());

			return cm;
		}

		void MyTextBrowser::setBackwardAvailable(bool b)
		{
			backward_ = b;
		}

		void MyTextBrowser::setForwardAvailable(bool b)
		{
			forward_ = b;
		}


		HelpViewer::HelpViewer(QWidget *parent, const char *name)
			throw()
			: DockWidget(parent, name),
				default_page_("index.html"),
				browser_( new MyTextBrowser(this)),
				whats_this_mode_(false),
				ignore_event_(false)
		{
			String dir = getDataPath();
			dir += 	String("..") + 
							FileSystem::PATH_SEPARATOR + 
							"doc" + 
							FileSystem::PATH_SEPARATOR +
							"BALLView" +
							FileSystem::PATH_SEPARATOR;

			setBaseDirectory(dir);

			hide();
			setGuest(*browser_);
			undock();
			resize(800, 700);
			move(20,20);
			setMinimumSize(800, 600);

			registerWidget(this);
		}

		HelpViewer::~HelpViewer()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << this << " of class HelpViewer" << endl;
			#endif 

			qApp->setGlobalMouseTracking(FALSE);
			qApp->removeEventFilter(this);
		}

		void HelpViewer::initializeWidget(MainControl& main_control)
			throw()
		{
			DockWidget::initializeWidget(main_control);
			insertMenuEntry(MainControl::HELP, "Documentation", this, SLOT(showHelp()), 0, 0);
			insertMenuEntry(MainControl::HELP, "Whats this?", this, SLOT(enterWhatsThisMode()));	

 			qApp->installEventFilter(this);
			qApp->setGlobalMouseTracking(TRUE);
		}

		void HelpViewer::showHelp()
		{
			showHelp(default_page_);
		}

		void HelpViewer::showHelp(const String& url)
		{
			browser_->setSource((base_dir_ + url).c_str());
			show();
		}

		void HelpViewer::onNotify(Message *message)
			throw()
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "HelpViewer" << this  << "onNotify " << message << std::endl;
#endif

			if (RTTI::isKindOf<RegisterHelpSystemMessage>(*message)) 
			{
				RegisterHelpSystemMessage* msg = RTTI::castTo<RegisterHelpSystemMessage>(*message);
				if (msg->getWidget() != 0)
				{
					if (msg->isRegister())
					{
						registerWidgetForHelpSystem(msg->getWidget(), msg->getURL());
					}
					else
					{
						unregisterWidgetForHelpSystem(msg->getWidget());
					}
				}

				else if (msg->getMenuEntry() != -1)
				{
					if (msg->isRegister())
					{
						registerMenuEntryForHelpSystem(msg->getMenuEntry(), msg->getURL());
					}
					else
					{
						unregisterMenuEntryForHelpSystem(msg->getMenuEntry());
					}
				}

				return;
			}

			if (!RTTI::isKindOf<ShowHelpMessage>(*message)) 
			{
				return;
			}

			ShowHelpMessage* msg = RTTI::castTo<ShowHelpMessage>(*message);
			showHelp(msg->getURL());
		}

		void HelpViewer::setDefaultPage(const String& url)
		{
			default_page_ = url;
			browser_->setSource(default_page_.c_str());
		}

		const String& HelpViewer::getDefaultPage() const
		{
			return default_page_;
		}
		
		const String& HelpViewer::getBaseDirectory() const
		{
			return base_dir_;
		}

		void HelpViewer::setBaseDirectory(const String& dir)
		{
			if (dir == "") return;

			base_dir_ = dir;

			browser_->mimeSourceFactory()->setFilePath(base_dir_.c_str());
			browser_->setSource((base_dir_ + default_page_).c_str());
		}

		void HelpViewer::enterWhatsThisMode()
		{
			qApp->setOverrideCursor(Qt::WhatsThisCursor);
			whats_this_mode_ = true;
		}

		
		void HelpViewer::exitWhatsThisMode()
		{
			whats_this_mode_ = false;
			qApp->restoreOverrideCursor();
		}

		bool HelpViewer::showDocumentationForObject()
		{
			QPoint point = QCursor::pos();
			QWidget* widget = qApp->widgetAt(point, true);

			if (widget == 0) return false;
			
			/////////////////////////////////////////////
			// show help for widget
			/////////////////////////////////////////////
			if (showHelpFor(widget)) 
			{
				exitWhatsThisMode();
				return true;
			}

			/////////////////////////////////////////////
			// show help for menu entry 
			/////////////////////////////////////////////
			
			// catch block is needed on windows, 
			// otherwise we get a uncaught exception, no idea why
			// maybe the library has a bug under windows
			try
			{
				if (RTTI::isKindOf<QPopupMenu>(*widget))
				{
					ignore_event_ = true;

					// nothing happens if we dont have a docu entry
					Index id = getMainControl()->getLastHighLightedMenuEntry();
					if (docu_for_menu_entry_.has(id))
					{
						showHelp(docu_for_menu_entry_[id]);
						exitWhatsThisMode();
					}

					return true;
				}
			}
			catch(...)
			{
			}

			return false;
		}
	
		bool HelpViewer::eventFilter(QObject*, QEvent* e) 
		{
			/////////////////////////////////////////////
			// Prevent opening a menu entry when obtaining whats this info for a menu entry
			/////////////////////////////////////////////
			if (ignore_event_)
			{
				if (e->type() == QEvent::MouseButtonRelease)
				{
					ignore_event_ = false;
					getMainControl()->menuBar()->hide();
					getMainControl()->menuBar()->show();
					return true;
				}

				return false;
			}
			
			/////////////////////////////////////////////
			// Show Documentation if Shift-F1 is pressed
			/////////////////////////////////////////////
			if (e->type() == QEvent::KeyPress)
			{
				QKeyEvent* ke = (QKeyEvent*) e;
				if (ke->key() != Qt::Key_F1 ||
						ke->state() != Qt::ShiftButton)
				{
					return false;
				}

				if (ke->key() == Key_Escape) 
				{
					if (whats_this_mode_)
					{
						exitWhatsThisMode();
						return true;
					}
				}

				showDocumentationForObject();
				return true;
			}

			/////////////////////////////////////////////
			// now react only in whats this mode and if a mouse button is pressed
			/////////////////////////////////////////////
			if (!whats_this_mode_ ||
					e->type() != QEvent::MouseButtonPress)
			{
				return false;
			}

			/////////////////////////////////////////////
			// exit whats this mode with right mouse click
			/////////////////////////////////////////////
			QMouseEvent* me = (QMouseEvent*) e;
			if (me->button() == Qt::RightButton)
			{
				exitWhatsThisMode();
				return true;
			}

			if (me->button() != Qt::LeftButton) return false;
		
			return showDocumentationForObject();
		}

		void HelpViewer::registerWidgetForHelpSystem(const QWidget* widget, const String& docu_entry)
		{
			docu_for_widget_[widget] = docu_entry;
		}

		void HelpViewer::registerMenuEntryForHelpSystem(Index entry, const String& docu_entry)
		{
			docu_for_menu_entry_[entry] = docu_entry;
		}

		void HelpViewer::unregisterWidgetForHelpSystem(const QWidget* widget)
		{
			docu_for_widget_.erase(widget);
		}

		void HelpViewer::unregisterMenuEntryForHelpSystem(Index id)
		{
			docu_for_menu_entry_.erase(id);
		}

		bool HelpViewer::showHelpFor(const QWidget* widget)
		{
			HashMap<const QWidget*, String>::Iterator to_find;
			to_find = docu_for_widget_.find(widget);

			QWidget* widget2 = (QWidget*) widget;

			while (to_find == docu_for_widget_.end() && widget2 != 0)
			{
				if (widget2->parent() == 0) return false;

				widget2 = dynamic_cast<QWidget*>(widget2->parent());

				if (widget2->parent() == 0) return false;

				to_find = docu_for_widget_.find(widget2);
			}

			if (widget2 == 0) return false;

			showHelp(docu_for_widget_[widget2]);

			return true;
		}

		bool HelpViewer::hasHelpFor(const QWidget* widget) const
		{
			return docu_for_widget_.has(widget);
		}

		bool HelpViewer::hasHelpFor(Index id) const
		{
			return docu_for_menu_entry_.has(id);
		}

		String HelpViewer::getHelpEntryFor(const QWidget* widget) const
		{
			if (!docu_for_widget_.has(widget)) return false;
			
			return docu_for_widget_[widget];
		}

		String HelpViewer::getHelpEntryFor(Index id) const
		{
			if (!docu_for_menu_entry_.has(id)) return false;

			return docu_for_menu_entry_[id];
		}
					
	} // VIEW
} // namespace BALL
