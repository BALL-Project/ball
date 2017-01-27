#include <PresentaBALLView.h>
#include <PresentaBALLSettings.h>

#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <QtWebChannel/QtWebChannel>

namespace BALL
{
	namespace VIEW
	{
		PresentaBALLView::PresentaBALLView(PresentaBALLSettings* settings, QWidget* parent, const char* name)
			: HTMLView(parent),
			  ModularWidget(name),
			  signal_(nullptr),
			  channel_(nullptr),
			  signalMapper_(nullptr),
			  index_html_(""),
			  settings_(settings)
		{
			// establish webchannel
			channel_ = new QWebChannel(page());
			signal_ = new PresentaBALLSignal(this);
			page()->setWebChannel(channel_);
			channel_->registerObject(QString("signals"), signal_);

			// connect actions
			QAction* action = nullptr;
			signalMapper_ = new QSignalMapper(this);
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
			ModularWidget::unregisterThis();
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
			if (cmsg)
			{
				// fire a Qt signal that can be handled by the website
				emit signal_->messageSignal(0, (int) cmsg->getType()); // CompositeMessage = 0

#ifdef BALL_VIEW_DEBUG
				Log.info() << "CompositeMessage fired to JS" << std::endl;
#endif
				return;
			}

			RepresentationMessage* rmsg = RTTI::castTo<RepresentationMessage>(*message);
			if (rmsg)
			{
				emit signal_->messageSignal(1, (int) rmsg->getType()); // RepresentationMessage = 1

				#ifdef BALL_VIEW_DEBUG
				Log.info() << "RepresentationMessage fired to JS" << std::endl;
				#endif
				return;
			}

			SceneMessage* smsg = RTTI::castTo<SceneMessage>(*message);
			if (smsg)
			{
				emit signal_->messageSignal(2, (int) smsg->getType()); //SceneMessage = 2

#ifdef BALL_VIEW_DEBUG
				Log.info() << "SceneMessage fired to JS" << std::endl;
#endif
				return;
			}

			DatasetMessage* dmsg = RTTI::castTo<DatasetMessage>(*message);
			if (dmsg)
			{
				emit signal_->messageSignal(3, (int) dmsg->getType()); //DataMessage = 3

#ifdef BALL_VIEW_DEBUG
				Log.info() << "DataMessage fired to JS" << std::endl;
#endif
			}
		}

		void PresentaBALLView::contextMenuEvent(QContextMenuEvent*)
		{ }
	}
}
