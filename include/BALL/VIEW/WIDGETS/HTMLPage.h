#ifndef BALL_VIEW_WIDGETS_HTMLPAGE_H
#define BALL_VIEW_WIDGETS_HTMLPAGE_H

#include <BALL/COMMON/global.h>

#include <QtCore/QHash>
// Qt 6 moved QWebEnginePage from QtWebEngineWidgets into QtWebEngineCore.
#include <QtWebEngineCore/QWebEnginePage>

namespace BALL
{
	namespace VIEW
	{
		class BALL_VIEW_EXPORT HTMLPage: public QWebEnginePage
		{
			Q_OBJECT

			public:
				HTMLPage(QObject* parent = 0, bool ignore_ssl_errors = false);
				HTMLPage(QWebEngineProfile* profile, QObject* parent = 0, bool ignore_ssl_errors = false);
				~HTMLPage() override = default;

			protected:
				bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame) override;
				// Qt 6: QWebEnginePage::certificateError is now a signal, not a
				// virtual hook. Keep the slot/non-virtual handler for the
				// signal-connection path used by callers; no 'override'.
				bool certificateError(const QWebEngineCertificateError& /* certificateError */);
				void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString& /* sourceID */) override;

				virtual void executeLink(const QUrl& url);

			private:
				bool ignore_ssl_errors_;
		};
	}
}
#endif // BALL_VIEW_WIDGETS_HTMLPAGE_H
