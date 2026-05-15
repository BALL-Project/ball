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
		{
			connectCertificateErrorHandler_();
		}

		HTMLPage::HTMLPage(QWebEngineProfile* profile, QObject* parent, bool ignore_ssl_errors) :
			QWebEnginePage(profile, parent),
			ignore_ssl_errors_(ignore_ssl_errors)
		{
			connectCertificateErrorHandler_();
		}

		void HTMLPage::connectCertificateErrorHandler_()
		{
			// Qt 6: certificateError is a signal on QWebEnginePage, not a
			// virtual hook. Wire it to onCertificateError() so the
			// ignore_ssl_errors_ flag is honoured (Qt 5 parity).
			connect(this, &QWebEnginePage::certificateError,
			        this, &HTMLPage::onCertificateError);
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

		void HTMLPage::onCertificateError(const QWebEngineCertificateError& error)
		{
			// Qt 6 signal-based API: accept/reject is communicated by calling
			// the corresponding method on the error object, not via a return
			// value. The signal hands us a const-ref; the accept/reject
			// methods are non-const, so we strip const here (the object is
			// owned by Qt and lives for the duration of the slot call).
			QWebEngineCertificateError& mutable_error =
				const_cast<QWebEngineCertificateError&>(error);
			if (ignore_ssl_errors_)
			{
				mutable_error.acceptCertificate();
			}
			else
			{
				mutable_error.rejectCertificate();
			}
		}

		void HTMLPage::javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString&)
		{
			Q_UNUSED(level)
			Q_UNUSED(message)
			Q_UNUSED(lineNumber)

#ifdef BALL_VIEW_DEBUG
			auto msg = QString("[HTMLPage/JSConsole] Line %1: %2").arg(lineNumber).arg(message);
			switch (level)
			{
				case InfoMessageLevel:
					Log.info() << msg.toStdString() << std::endl;
					break;
				case WarningMessageLevel:
					Log.warn() << msg.toStdString() << std::endl;
					break;
				case ErrorMessageLevel:
					Log.error() << msg.toStdString() << std::endl;
					break;
			}
#endif
		}

		void HTMLPage::executeLink(const QUrl& url)
		{
			auto action_name = QUrlQuery(url).queryItemValue("action");
			if (action_name == QString())
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
