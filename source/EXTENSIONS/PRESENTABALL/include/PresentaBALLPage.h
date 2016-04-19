#ifndef PRESENTABALLPAGE_H
#define PRESENTABALLPAGE_H

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

		class PresentaBALLPage: public QWebEnginePage
		{
			Q_OBJECT

			public:
				PresentaBALLPage(QObject* parent = 0);
				PresentaBALLPage(QWebEngineProfile* profile, QObject* parent = 0);
				virtual ~PresentaBALLPage();

			protected:
				typedef QList<QPair<QString, QString> > ParameterList;
				virtual void init();
				virtual bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame);
				virtual void executeLink(const QUrl& url);
				virtual void executePython_(const QString& action, const ParameterList& parameters);

			private:
				String script_base_;
				QHash<QString, HTMLInterfaceAction*> action_registry_;
		};
	}
}
#endif // PRESENTABALLPAGE_H
