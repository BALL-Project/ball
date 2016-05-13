#include <PresentaBALLView.h>
#include <PresentaBALLSettings.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/shortcutRegistry.h>

#include <QtWebChannel/QtWebChannel>
#include <QtCore/QUrlQuery>
#include <QThread>

namespace BALL
{
	namespace VIEW
	{
		PresentaBALLView::PresentaBALLView(QWidget* parent, const char* name)
			: HTMLView(parent),
			  ModularWidget(name)
		{
			// establish webchannel
			settings_ = new PresentaBALLSettings(this);
			channel_ = new QWebChannel(page());
			signal_ = new PresentaBALLSignal;
			page()->setWebChannel(channel_);
			channel_->registerObject(QString("signals"), signal_);
			restoreDefaults();

			// connect actions
			QAction* action = 0;
			signalMapper_ = new QSignalMapper;
			ShortcutRegistry& sr = MainControl::getInstance(0)->getShortcutRegistry();
			for (uint i = 0; i < sr.size(); i++)
			{
				action = sr[i].second;
				if (action)
				{
					connect(action, SIGNAL(triggered()), signalMapper_, SLOT(map()));
					signalMapper_->setMapping(action, i);
					connect(signalMapper_, SIGNAL(mapped(int)), signal_, SIGNAL(actionSignal(int)));
#ifdef BALL_VIEW_DEBUG
					Log.info() << "Connected <" << action->text().toStdString() << "> action to JSActionSignal " << i << std::endl;
#endif
				}
			}

			ModularWidget::registerWidget(this);
		}

		PresentaBALLView::~PresentaBALLView()
		{
			delete channel_;
			delete signal_;
			delete signalMapper_;
			ModularWidget::unregisterThis();
		}

		void PresentaBALLView::restoreDefaults()
		{
			Path p;
			String s;
			
			//set the webpage language according to the language set in preferences
			String home_dir = Directory::getUserHomeDir();
			INIFile f(home_dir + FileSystem::PATH_SEPARATOR + ".BALLView");
			f.read();

			if (f.hasEntry("GENERAL", "language")) 
			{
				QString str = f.getValue("GENERAL", "language").c_str();

				if (!(str == "Default") && str == "de_DE")
				{
					s = p.find("HTMLBasedInterface/html_de");
				}
				else
				{
					//default = english
					s = p.find("HTMLBasedInterface/html_eng");
				}
			 }

			if (!s.isEmpty())
			{
				setIndexHTML((s + "/index.html").c_str());
			}
			else
			{
				Log.error() << "No html directory set!" << std::endl;
			}
		}

		void PresentaBALLView::applyPreferences()
		{
			setIndexHTML(ascii(settings_->getIndexHTMLLocation()));
		}

		void PresentaBALLView::setIndexHTML(const String& index_html)
		{
			if (index_html != index_html_)
			{
				index_html_ = index_html;
				setUrl(QUrl::fromLocalFile(index_html_.c_str()));

				settings_->setIndexHTMLLocation(index_html_.c_str());
			}
		}

		String const& PresentaBALLView::getIndexHTML()
		{
			return index_html_;
		}

		void PresentaBALLView::onNotify(Message* message)
		{
			//Try to cast message to the type, that you want to handle
			CompositeMessage* cmsg = RTTI::castTo<CompositeMessage>(*message);
			RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);
			SceneMessage* smsg = RTTI::castTo<SceneMessage>(*message);
			DatasetMessage* dmsg = RTTI::castTo<DatasetMessage>(*message);			

			if (cmsg == 0)
			{
				if (rmsg == 0)
				{
					if (smsg == 0)
					{
						if (dmsg == 0)
						{
							return;
						}
						else
						{
							emit signal_->messageSignal(3, (int) dmsg->getType()); //DataMessage = 3

							#ifdef BALL_VIEW_DEBUG
							Log.info() << "DataMessage fired to JS" << std::endl;
							#endif
						}
					}
					else
					{
						emit signal_->messageSignal(2, (int) smsg->getType()); //SceneMessage = 2

						#ifdef BALL_VIEW_DEBUG
						Log.info() << "SceneMessage fired to JS" << std::endl;
						#endif
					}
				}
				else
				{
					emit signal_->messageSignal(1, (int) rmsg->getType()); // RepresentationMessage = 1

					#ifdef BALL_VIEW_DEBUG
					Log.info() << "RepresentationMessage fired to JS" << std::endl;
					#endif
				}
			}
			else
			{
				// fire a Qt signal that can be handled by the website
				emit signal_->messageSignal(0, (int) cmsg->getType()); // CompositeMessage = 0

				#ifdef BALL_VIEW_DEBUG
				Log.info() << "CompositeMessage fired to JS" << std::endl;
				#endif
			}
		}

		void PresentaBALLView::contextMenuEvent(QContextMenuEvent*)
		{
		}

		PresentaBALLSettings* PresentaBALLView::getSettings()
		{
			return settings_;
		}
	}
}
