#include <PresentaBALLPage.h>

#include <BALL/SYSTEM/path.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <QUrlQuery>
#include <QtWebEngineWidgets/QWebEnginePage>

namespace BALL
{
	namespace VIEW
	{
		void HTMLInterfaceAction::execute(const QList<QPair<QString, QString> >& parameters)
		{
			executeImpl_(parameters);
			emit finishedExecution();
		}

		PresentaBALLPage::PresentaBALLPage(QObject* parent)
				: QWebEnginePage(parent)
		{
			init();
		}

		PresentaBALLPage::PresentaBALLPage(QWebEngineProfile* profile, QObject* parent)
				: QWebEnginePage(profile, parent)
		{
			init();
		}

		PresentaBALLPage::~PresentaBALLPage()
		{
			for(QHash<QString, HTMLInterfaceAction*>::iterator it = action_registry_.begin(); it != action_registry_.end(); ++it)
			{
				delete it.value();
			}
		}

		void PresentaBALLPage::init()
		{
			Path p;
			script_base_ = p.find("HTMLBasedInterface/scripts") + "/";
		}

		bool PresentaBALLPage::acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame)
		{
			if(getMainControl()->isBusy())
			{
				return false;
			}

			executeLink(url);
			return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
		}

		void PresentaBALLPage::executeLink(const QUrl& url)
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

		void PresentaBALLPage::executePython_(const QString& action, const ParameterList& parameters)
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
	}
}
