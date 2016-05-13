#include <BALL/VIEW/WIDGETS/HTMLPage.h>

#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <QUrlQuery>

namespace BALL
{
	namespace VIEW
	{
		void HTMLInterfaceAction::execute(const QList<QPair<QString, QString> >& parameters)
		{
			executeImpl_(parameters);
			emit finishedExecution();
		}

		HTMLPage::HTMLPage(QObject* parent, bool ignore_ssl_errors)
				: QWebEnginePage(parent),
				  ignore_ssl_errors_(ignore_ssl_errors),
				  action_registry_()
		{}

		HTMLPage::HTMLPage(QWebEngineProfile* profile, QObject* parent, bool ignore_ssl_errors)
				: QWebEnginePage(profile, parent),
				  ignore_ssl_errors_(ignore_ssl_errors),
				  action_registry_()
		{}

		HTMLPage::~HTMLPage()
		{
			for (QHash<QString, HTMLInterfaceAction*>::iterator it = action_registry_.begin(); it != action_registry_.end(); ++it)
			{
				delete it.value();
			}
		}

		bool HTMLPage::acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame)
		{
			if (getMainControl()->isBusy())
			{
				return false;
			}

			executeLink(url);
			return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
		}

		bool HTMLPage::certificateError(const QWebEngineCertificateError&)
		{
			return ignore_ssl_errors_;
		}

		void HTMLPage::javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString&)
		{
			Q_UNUSED(level)
			Q_UNUSED(message)
			Q_UNUSED(lineNumber)

#ifdef BALL_VIEW_DEBUG
			switch (level)
			{
				case InfoMessageLevel:
					Log.info() << lineNumber << ": " << message.toStdString() << std::endl;
					break;
				case WarningMessageLevel:
					Log.warn() << lineNumber << ": " << message.toStdString() << std::endl;
					break;
				case ErrorMessageLevel:
					Log.error() << lineNumber << ": " << message.toStdString() << std::endl;
					break;
			}
#endif
		}

		void HTMLPage::executeLink(const QUrl& url)
		{
			QString action_name = QUrlQuery(url).queryItemValue("action");
			if (action_name == QString::null)
			{
				return;
			}

			QString method_type = QUrlQuery(url).queryItemValue("method");
			QString parameters  = QUrlQuery(url).queryItemValue("parameters");

			//Ideally this if should be converted into another registry
			if (method_type == "native")
			{
				QHash<QString, HTMLInterfaceAction*>::iterator it = action_registry_.find(action_name);

				if (it != action_registry_.end())
				{
					(*it)->execute(QUrlQuery(url).queryItems());
				}
			}
			else if (method_type == "" || method_type == "python")
			{
				executePython_(action_name, QUrlQuery(url).queryItems());
			}
		}

		void HTMLPage::executePython_(const QString& action, const ParameterList& parameters)
		{
#ifdef BALL_PYTHON_SUPPORT
			//Search the module to load
			QString load_module = "__main__";
			for (ParameterList::const_iterator it = parameters.begin(); it != parameters.end(); ++it)
			{
				if (it->first == "module")
				{
					load_module = it->second;
					break;
				}
			}

			try
			{
				PyInterpreter::execute(load_module, action, parameters);
			}
			catch (Exception::FileNotFound)
			{
				Log.error() << "Could not execute action " << action.toStdString() << "\n No such file or directory." << std::endl;
			}
#else
			Log.error() << "BALL is compiled without Python support. Action " << action.toStdString() << " could not be executed." << std::endl;
#endif
		}
	}
}
