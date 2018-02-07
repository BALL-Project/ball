#include <BALL/VIEW/WIDGETS/HTMLPage.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

#ifdef BALL_PYTHON_SUPPORT
#	include <BALL/PYTHON/pyInterpreter.h>
#endif

#include <QUrlQuery>

using std::string;

namespace BALL
{
	namespace VIEW
	{
		HTMLPage::HTMLPage(QObject* parent, bool ignore_ssl_errors) :
			QWebEnginePage(parent),
			ignore_ssl_errors_(ignore_ssl_errors)
		{}

		HTMLPage::HTMLPage(QWebEngineProfile* profile, QObject* parent, bool ignore_ssl_errors) :
			QWebEnginePage(profile, parent),
			ignore_ssl_errors_(ignore_ssl_errors)
		{}

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
			auto action_name = QUrlQuery(url).queryItemValue("action");
			if (action_name == QString::null)
			{
				return;
			}

#ifdef BALL_PYTHON_SUPPORT
			string load_module = "__main__";
			PyKernel::KeyValArgs args;
			for (const auto& pair: QUrlQuery(url).queryItems())
			{
				if (pair.first == "module") load_module = pair.second.toStdString();

				if (pair.first == "action" || pair.first == "module") continue;

				args[pair.first.toStdString()] = pair.second.toStdString();
			}

			if (!PyInterpreter::execute(load_module, action_name.toStdString(), args))
			{
				Log.error() << "Could not execute action " << action_name.toStdString() << " from module "
							<< load_module << " \n";
			}
#else
			Log.error() << "BALL has been compiled without Python support. Action " << action_name.toStdString()
						<< " cannot be executed." << std::endl;
#endif
		}
	}
}
