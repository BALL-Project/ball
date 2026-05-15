#ifndef BALL_VIEW_WIDGETS_HTMLPAGE_H
#define BALL_VIEW_WIDGETS_HTMLPAGE_H

#include <BALL/COMMON/global.h>

#include <QtCore/QHash>
// Qt 6 moved QWebEnginePage from QtWebEngineWidgets into QtWebEngineCore.
#include <QtWebEngineCore/QWebEnginePage>
#include <QtWebEngineCore/QWebEngineCertificateError>

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
				void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString& /* sourceID */) override;

				virtual void executeLink(const QUrl& url);

			private Q_SLOTS:
				// Qt 6: QWebEnginePage::certificateError is now a SIGNAL on the
				// page (not a virtual hook). To honour ignore_ssl_errors_ we
				// connect this slot in the constructor and call
				// acceptCertificate()/rejectCertificate() on the error object.
				void onCertificateError(const QWebEngineCertificateError& error);

			private:
				// Wire the certificateError signal -> onCertificateError slot.
				// Shared by both constructors.
				void connectCertificateErrorHandler_();

				bool ignore_ssl_errors_;
		};
	}
}
#endif // BALL_VIEW_WIDGETS_HTMLPAGE_H
