// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id:
//

#include <BALL/VIEW/WIDGETS/helpViewer.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		HelpViewer::HelpViewer(QWidget *parent, const char *name)
			throw()
			: DockWidget(parent, name),
				default_page_("index.html"),
				browser_( new QTextBrowser(this))
		{
			Path path;
			base_dir_ = path.getDataPath() + 
									FileSystem::PATH_SEPARATOR + 
									".." + 
									FileSystem::PATH_SEPARATOR + 
									"doc" + 
									FileSystem::PATH_SEPARATOR +
									"BALLView" +
									FileSystem::PATH_SEPARATOR;

			hide();
			setGuest(*browser_);
			undock();
			resize(800, 700);
			move(20,20);
			setMinimumSize(800, 600);

  		browser_->mimeSourceFactory()->setFilePath(base_dir_.c_str());
			browser_->setSource((base_dir_ + default_page_).c_str());

			registerWidget(this);
		}

		HelpViewer::~HelpViewer()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << this << " of class HelpViewer" << endl;
			#endif 
		}

		void HelpViewer::initializeWidget(MainControl& main_control)
			throw()
		{
			DockWidget::initializeWidget(main_control);
			insertMenuEntry(MainControl::HELP, "Documentation", this, SLOT(showHelp()));
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
			browser_->setSource(default_page_);
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
			base_dir_ = dir;
			browser_->mimeSourceFactory()->setFilePath(base_dir_.c_str());
			browser_->setSource(base_dir_ + default_page_);
		}

	} // VIEW
} // namespace BALL
