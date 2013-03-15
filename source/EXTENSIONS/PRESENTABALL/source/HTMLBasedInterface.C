#include <HTMLBasedInterface.h>
#include <PresentaBALLSettings.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/directory.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/shortcutRegistry.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>

#include <QtCore/QUrlQuery>

namespace BALL
{
	namespace VIEW
	{

		void HTMLInterfaceAction::execute(const QList<QPair<QString, QString> >& parameters)
		{
			executeImpl_(parameters);

			emit finishedExecution();
		}

		HTMLBasedInterface::HTMLBasedInterface(QWidget* parent, const char* name)
			: HTMLView(parent),
				ModularWidget(name)
		{
			page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

			settings_ = new PresentaBALLSettings(this);
			
			//create SignalMapper for Scene actions
			signalMapper = new QSignalMapper(this);
			
			// make us available in Javascript, even after a new url has been loaded
			connect(page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(exposeQObjectToJavascript()));
			
			connect(this, SIGNAL(linkClicked(const QUrl&)), this, SLOT(handleLinkClicked(const QUrl&)));
			connect(this, SIGNAL(urlChanged(const QUrl&)), this, SLOT(executeLink(const QUrl&)));
			
			restoreDefaults();

			ModularWidget::registerWidget(this);
		}

		HTMLBasedInterface::~HTMLBasedInterface()
		{
			for(QHash<QString, HTMLInterfaceAction*>::iterator it = action_registry_.begin(); it != action_registry_.end(); ++it)
			{
				delete it.value();
			}

			ModularWidget::unregisterThis();
		}

		void HTMLBasedInterface::restoreDefaults()
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

			script_base_ = p.find("HTMLBasedInterface/scripts") + "/";
		}

		void HTMLBasedInterface::applyPreferences()
		{
			setIndexHTML(ascii(settings_->getIndexHTMLLocation()));
		}

		void HTMLBasedInterface::setIndexHTML(const String& index_html)
		{
			if (index_html != index_html_)
			{
				index_html_ = index_html;
				setUrl(QUrl::fromLocalFile(index_html_.c_str()));

				settings_->setIndexHTMLLocation(index_html_.c_str());
			}
		}

		String const& HTMLBasedInterface::getIndexHTML()
		{
			return index_html_;
		}

		void HTMLBasedInterface::registerAction(HTMLInterfaceAction* action)
		{
			if(!action)
			{
				return;
			}

			QHash<QString, HTMLInterfaceAction*>::iterator it = action_registry_.find(action->getName());

			if(it != action_registry_.end())
			{
				delete *it;
			}

			action_registry_.insert(action->getName(), action);
		}
		
		void HTMLBasedInterface::test(int i)
		{
			emit fireJSActionSignal(i);
		}
		
		void HTMLBasedInterface::exposeQObjectToJavascript()
		{
			QAction* action = 0;
			
			ShortcutRegistry& sr = MainControl::getInstance(0)->getShortcutRegistry();
			
			for(uint i = 0; i < sr.size(); i++)
			{
				action = sr[i].second;
				if (action)
				{
					connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
					signalMapper->setMapping(action, i);
					connect(signalMapper, SIGNAL(mapped(int)), this, SIGNAL(fireJSActionSignal(int)));
					#ifdef BALL_VIEW_DEBUG
					Log.info() << "Connected <" << action->text().toStdString() << "> action to JSActionSignal " << i << std::endl;
					#endif
				}
			}
			//add us (the object of HTMLBasedInterface) to JavaScript runtime of currently loaded page
			page()->mainFrame()->addToJavaScriptWindowObject(QString("mywebview"), this);
			
			// Just for testing
			//page()->mainFrame()->evaluateJavaScript("alert('Qt is good'); ");
		}

		void HTMLBasedInterface::handleLinkClicked(const QUrl& url)
		{
			if(!getMainControl()->isBusy())
			{
				setUrl(url);
			}
		}

		void HTMLBasedInterface::onNotify(Message* message)
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
							emit fireJSMessage(3, (int) dmsg->getType()); //DataMessage = 3

							#ifdef BALL_VIEW_DEBUG
							Log.info() << "DataMessage fired to JS" << std::endl;
							#endif
						}
					}
					else
					{

						emit fireJSMessage(2, (int) smsg->getType()); //SceneMessage = 2

						#ifdef BALL_VIEW_DEBUG
						Log.info() << "SceneMessage fired to JS" << std::endl;
						#endif
					}
				}
				else
				{
					emit fireJSMessage(1, (int) rmsg->getType()); // RepresentationMessage = 1

					#ifdef BALL_VIEW_DEBUG
					Log.info() << "RepresentationMessage fired to JS" << std::endl;
					#endif
				}
			}
			else
			{

				// fire a Qt signal that can be handled by the website 
				emit fireJSMessage(0, (int) cmsg->getType()); // CompositeMessage = 0

				#ifdef BALL_VIEW_DEBUG
				Log.info() << "CompositeMessage fired to JS" << std::endl;
				#endif
			}


		}
		
		void HTMLBasedInterface::contextMenuEvent(QContextMenuEvent*)
		{
		}

		void HTMLBasedInterface::executeLink(const QUrl& url)
		{
			QString action_name = QUrlQuery(url).queryItemValue("action");
			if(action_name == QString::null)
			{
				return;
			}

			QString method_type = QUrlQuery(url).queryItemValue("method");
			QString parameters  = QUrlQuery(url).queryItemValue("parameters");

			//Ideally this if should be converted into another registry
			if(method_type == "native")
			{
				QHash<QString, HTMLInterfaceAction*>::iterator it = action_registry_.find(action_name);

				if(it != action_registry_.end())
				{
					(*it)->execute(QUrlQuery(url).queryItems());
				}
			}
			else if(method_type == "" || method_type == "python")
			{
					executePython_(action_name, QUrlQuery(url).queryItems());
			}
		}

		void HTMLBasedInterface::executePython_(const QString& action, const ParameterList& parameters)
		{
#ifdef BALL_PYTHON_SUPPORT
			//Ensure, that the module search path is registered
			static bool added_module_path = false;

			if(!added_module_path)
			{
				PyInterpreter::run(String("sys.path.append(\"") + script_base_ + "\")", added_module_path);

				if(!added_module_path)
				{
					Log.error() << "Could not add module path" << std::endl;
					return;
				}
			}

			//Search the module to load
			QString load_module = "__main__";
			for(ParameterList::const_iterator it = parameters.begin(); it != parameters.end(); ++it)
			{
				if(it->first == "module")
				{
					load_module = it->second;
					break;
				}
			}

			try
			{
				PyInterpreter::execute(load_module, action, parameters);
			}
			catch(Exception::FileNotFound)
			{
				Log.error() << "Could not execute action " << action.toStdString() << "\n No such file or directory." << std::endl;
			}
#else
			Log.error() << "BALL is compiled without Python support. Action " << action.toStdString() << " could not be executed." << std::endl;
#endif
		}

		PresentaBALLSettings* HTMLBasedInterface::getSettings()
		{
			return settings_;
		}

	}
}
