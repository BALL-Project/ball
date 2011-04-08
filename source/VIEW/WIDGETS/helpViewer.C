// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/helpViewer.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/lineBasedFile.h>

#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtCore/QEvent>
#include <QtGui/QTextCursor>
#include <QtGui/QToolBar>

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
			setOpenExternalLinks(true);
//   			connect(this, SIGNAL(backwardAvailable(bool)), this, SLOT(setBackwardAvailable(bool)));
//   			connect(this, SIGNAL(forwardAvailable(bool)), this, SLOT(setForwardAvailable(bool)));
		}

		/*
		QMenu* MyTextBrowser::createMenu(const QPoint&)
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
*/


		HelpViewer::HelpViewer(QWidget *parent, const char *name)
			: DockWidget(parent, name),
				project_("BALLView"),
				default_page_("index.html"),
				browser_( new MyTextBrowser(this)),
				whats_this_mode_(false),
				ignore_event_(false),
				whats_this_(true),
				whats_action_(0)
		{
			Path p;
			String dir = p.find("../doc/BALLView/");

			setBaseDirectory(dir);

			hide();
			setGuest(*browser_);
			registerWidget(this);
			browser_->setReadOnly(true);
		}

		HelpViewer::~HelpViewer()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << this << " of class HelpViewer" << endl;
			#endif 

			qApp->removeEventFilter(this);
		}

		void HelpViewer::initializeWidget(MainControl& main_control)
		{
			DockWidget::initializeWidget(main_control);
			QAction* action = insertMenuEntry(MainControl::HELP, tr((project_ + " Documentation").c_str()), this, SLOT(showHelp()),
			                                  "Shortcut|MainControl|Documentation|Help", QKeySequence(), 
																				tr(""), UIOperationMode::MODE_ADVANCED);
			setIcon(action, "actions/help-hint", true);

			if (action)
				registerForHelpSystem(action, "tips.html#help");

			if (whats_this_)
			{
				whats_action_ = insertMenuEntry(MainControl::HELP, tr("Whats this?"), this, SLOT(enterWhatsThisMode()),
				                                "Shortcut|MainControl|Documentation|WhatsThis", QKeySequence(),
																				tr("Show help for clicked widget, exit this mode with right mouse button."),
																				UIOperationMode::MODE_ADVANCED);
				registerForHelpSystem(whats_action_, "tips.html#help");
			}

 			qApp->installEventFilter(this);
		}

		void HelpViewer::showHelp()
		{
			showHelp(default_page_);
		}

		void HelpViewer::showHelp(const String& URL)
		{
			showHelp(URL, "");
		}

		void HelpViewer::showHelp(const String& org_url, String entry)
		{
			String url = org_url;
			String fragment;

			if (url.has('#'))
			{
				url = url.before("#");
				fragment = org_url.after("#");
			}

			QUrl qurl = QUrl::fromLocalFile((base_dir_ + url).c_str());
			if (fragment != "") qurl.setFragment(fragment.c_str());
 			browser_->setSource(qurl);

 			QTextCursor ct = browser_->textCursor();
 			if (!ct.atStart()) 
 			{
 				ct.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
				browser_->setTextCursor(ct);
 			}

			if (entry != "") browser_->find(entry.c_str(), QTextDocument::FindCaseSensitively);

			if (!isVisible())
			{
				show();
				setFloating(true);
				showMaximized();
			}
		}

		void HelpViewer::onNotify(Message *message)
		{
#ifdef BALL_VIEW_DEBUG
			Log.error() << "HelpViewer" << this  << "onNotify " << message << std::endl;
#endif

			if (RTTI::isKindOf<RegisterHelpSystemMessage>(*message)) 
			{
				RegisterHelpSystemMessage* msg = RTTI::castTo<RegisterHelpSystemMessage>(*message);
				if (msg->isRegister())
				{
					registerForHelpSystem(msg->getObject(), msg->getURL());
				}
				else
				{
					unregisterForHelpSystem(msg->getObject());
				}

				return;
			}

			if (!RTTI::isKindOf<ShowHelpMessage>(*message)) return;

			ShowHelpMessage* msg = RTTI::castTo<ShowHelpMessage>(*message);
			bool classname = false;
			String project = msg->getProject();
			String url = msg->getURL();

			if (project.hasSuffix(" class"))
			{
				classname = true;
				project = project.before(" ");
			}

			if (project != project_) return;

			if (classname)
			{
				showDocumentationFor(url, msg->getEntry());
				return;
			}

			showHelp(url, msg->getEntry());
		}

		void HelpViewer::showDocumentationFor(const String& classname, const String& member)
		{
			if (classes_to_files_.size() == 0) collectClasses_();

			String classn = classname;
			if (!classes_to_files_.has(classn))
			{
				classn = String("T") + classn;
				if (!classes_to_files_.has(classn)) return;
			}

			showHelp(classes_to_files_[classn], member);
		}

		void HelpViewer::collectClasses_()
		{
			classes_to_files_.clear();
			
			Path p;
			String filename = p.find("../doc/classes");

			if (filename == "")
			{
				Log.error() << "Could not load the file \"classes\" to parse class list for the documentation!"
										<< std::endl;

				return;
			}

			try
			{
				LineBasedFile file(filename);
				vector<String> fields;
				while (file.readLine())
				{
					const String& line = file.getLine();
					line.split(fields, " ");
					classes_to_files_[fields[0]] = fields[1];
				}
			}
			catch(...)
			{
				Log.error() << "Could not parse the file \"classes\" containing the class list for the documentation!"
										<< std::endl;
			}
		}

		void HelpViewer::setDefaultPage(const String& url)
		{
			default_page_ = url;
			QUrl qurl = QUrl::fromLocalFile((base_dir_ + url).c_str());
			browser_->setSource(qurl);
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

			QUrl qurl = QUrl::fromLocalFile((base_dir_ + default_page_).c_str());
			browser_->setSource(qurl);
		}

		void HelpViewer::enterWhatsThisMode()
		{
			qApp->setOverrideCursor(Qt::WhatsThisCursor);
			whats_this_mode_ = true;
		}

		
		void HelpViewer::exitWhatsThisMode()
		{
			if (!whats_this_mode_) return;

			whats_this_mode_ = false;
			qApp->restoreOverrideCursor();
		}

		bool HelpViewer::showDocumentationForObject()
		{
			QPoint point = QCursor::pos();
			QWidget* widget = qApp->widgetAt(point);

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
				if (RTTI::isKindOf<QMenu>(*widget))
				{
					ignore_event_ = true;

					// nothing happens if we dont have a docu entry
					QAction* id = getMainControl()->getLastHighLightedMenuEntry();

					if (docu_entries_.has(id))
					{
						showHelp(docu_entries_[id]);
						exitWhatsThisMode();
						return true;
					}
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
			// Show Documentation if F1 is pressed
			/////////////////////////////////////////////
			if (e->type() == QEvent::KeyPress)
			{
				QKeyEvent* ke = (QKeyEvent*) e;
				if (ke->key() != Qt::Key_F1 ||
 						ke->modifiers() != Qt::NoModifier)
				{
					return false;
				}

				if (ke->key() == Qt::Key_Escape) 
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
			if (me->button() != Qt::LeftButton)
			{
				exitWhatsThisMode();
				return true;
			}

			if (me->button() != Qt::LeftButton) return false;
		
			QPoint point = QCursor::pos();
			QWidget* widget = qApp->widgetAt(point);
			QMenu* menu = 0;
			QMenuBar* menu_bar = 0;
			
			if (widget != 0) 
			{
				menu     = dynamic_cast<QMenu*>(widget);
				menu_bar = dynamic_cast<QMenuBar*>(widget);
			}

			if (menu_bar != 0) return false;

			bool shown_docu = showDocumentationForObject();

			if (menu != 0)
			{
				if (shown_docu)
				{
					menu->hide();
				}
				else
				{
					return false;
				}
			}

			return true;
		}

		void HelpViewer::registerForHelpSystem(const QObject* object, const String& docu_entry)
		{
			docu_entries_[object] = docu_entry;
		}

		void HelpViewer::unregisterForHelpSystem(const QObject* object)
		{
			docu_entries_.erase(object);
		}

		bool HelpViewer::showHelpFor(const QObject* object)
		{
			if (object == 0) return false;

			HashMap<const QObject*, String>::Iterator to_find;

			QObject* object2 = (QObject*) object;

			QWidget* widget = dynamic_cast<QWidget*>(object2);
			if (widget && widget->parent() != 0)
			{
				QToolBar* tb = dynamic_cast<QToolBar*>(widget->parent());
				if (tb != 0)
				{
					QList<QAction *> acs = widget->actions();
					if (acs.size() == 1)
					{
						to_find = docu_entries_.find(*acs.begin());
						if (to_find != docu_entries_.end())
						{
							showHelp((*to_find).second);
							return true;
						}
					}
				}
			}

			while (object2 != 0)
			{ 
				to_find = docu_entries_.find(object2);
				if (to_find != docu_entries_.end()) break;


				object2 = object2->parent();
			}

			if (object2 == 0) 
			{
				setStatusbarText(tr("No documentation for this widget available!"), true);
				return false;
			}

			showHelp((*to_find).second);

			return true;
		}

		bool HelpViewer::hasHelpFor(const QObject* object) const
		{
			return docu_entries_.has(object);
		}

		String HelpViewer::getHelpEntryFor(const QObject* widget) const
		{
			if (!docu_entries_.has(widget)) return false;
			
			return docu_entries_[widget];
		}

	} // VIEW
} // namespace BALL
