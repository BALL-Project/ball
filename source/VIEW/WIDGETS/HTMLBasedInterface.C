#include <BALL/VIEW/WIDGETS/HTMLBasedInterface.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{

		void HTMLInterfaceAction::execute(const QList<QPair<QString, QString> >& parameters)
		{
			executeImpl_(parameters);

			emit finishedExecution();
		}

		HTMLBasedInterface::HTMLBasedInterface(QWidget* parent)
			: HTMLView(parent)
		{
			page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

			connect(this, SIGNAL(linkClicked(const QUrl&)), this, SLOT(handleLinkClicked(const QUrl&)));
			connect(this, SIGNAL(urlChanged(const QUrl&)), this, SLOT(executeLink(const QUrl&)));

			Path p;
			String s = p.find("HTMLBasedInterface/html");

			setUrl(QUrl::fromLocalFile(QString(s.c_str()) + "/index.html"));

			script_base_ = p.find("HTMLBasedInterface/scripts") + "/";
		}

		HTMLBasedInterface::~HTMLBasedInterface()
		{
			for(QHash<QString, HTMLInterfaceAction*>::iterator it = action_registry_.begin(); it != action_registry_.end(); ++it)
			{
				delete it.value();
			}
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

		void HTMLBasedInterface::handleLinkClicked(const QUrl& url)
		{
			if(!getMainControl()->isBusy())
			{
				setUrl(url);
			}
		}

		void HTMLBasedInterface::contextMenuEvent(QContextMenuEvent*)
		{
		}

		void HTMLBasedInterface::executeLink(const QUrl& url)
		{
			QString action_name = url.queryItemValue("action");
			if(action_name == QString::null)
			{
				return;
			}

			QString method_type = url.queryItemValue("method");
			QString parameters  = url.queryItemValue("parameters");

			//Ideally this if should be converted into another registry
			if(method_type == "native")
			{
				QHash<QString, HTMLInterfaceAction*>::iterator it = action_registry_.find(action_name);

				if(it != action_registry_.end())
				{
					(*it)->execute(url.queryItems());
				}
			}
			else if(method_type == "" || method_type == "python")
			{
					executePython_(action_name, url.queryItems());
			}
		}

		void HTMLBasedInterface::executePython_(const QString& action, const ParameterList& parameters)
		{
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
				Log.error() << "Could not execute action " << action.toAscii().data() << "\n No such file or directory." << std::endl;
			}
		}

	}
}
