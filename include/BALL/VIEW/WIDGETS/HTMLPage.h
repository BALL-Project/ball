#ifndef BALL_VIEW_WIDGETS_HTMLPAGE_H
#define BALL_VIEW_WIDGETS_HTMLPAGE_H

#include <BALL/DATATYPE/string.h>

#include <QtCore/QHash>
#include <QtWebEngineWidgets/QWebEnginePage>

namespace BALL
{
	namespace VIEW
	{
		class HTMLInterfaceAction : public QObject
		{
			Q_OBJECT

			public:
				virtual QString getName() const = 0;

			public slots:

				void execute(const QList<QPair<QString, QString> >& parameters);

			protected:
				virtual void executeImpl_(const QList<QPair<QString, QString> >& parameters) = 0;

			signals:
				void finishedExecution();
		};

		class HTMLPage: public QWebEnginePage
		{
			Q_OBJECT

			public:
				HTMLPage(QObject* parent = 0, bool ignore_ssl_errors = false);
				HTMLPage(QWebEngineProfile* profile, QObject* parent = 0, bool ignore_ssl_errors = false);
				virtual ~HTMLPage();

			protected:
				typedef QList<QPair<QString, QString> > ParameterList;
				virtual bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame);
				virtual bool certificateError(const QWebEngineCertificateError& /* certificateError */);
				virtual void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString& /* sourceID */);
				virtual void executeLink(const QUrl& url);
				virtual void executePython_(const QString& action, const ParameterList& parameters);

			private:
				bool ignore_ssl_errors_;
				QHash<QString, HTMLInterfaceAction*> action_registry_;
		};
	}
}
#endif // BALL_VIEW_WIDGETS_HTMLPAGE_H
