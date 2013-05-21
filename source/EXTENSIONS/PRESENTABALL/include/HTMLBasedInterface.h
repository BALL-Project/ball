#ifndef HTMLBASEDINTERFACE_H
#define HTMLBASEDINTERFACE_H

#ifndef BALL_VIEW_WIDGETS_HTMLVIEW_H
	#include <BALL/VIEW/WIDGETS/HTMLView.h>
#endif

#include <QtCore/QSignalMapper>
#include <QtCore/QHash>

namespace BALL
{
	namespace VIEW
	{
		class PresentaBALLSettings;

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

		class HTMLBasedInterface : public HTMLView, public ModularWidget
		{
			Q_OBJECT
			BALL_EMBEDDABLE(HTMLBasedInterface, ModularWidget)

			public:
				HTMLBasedInterface(QWidget* parent = 0, const char* name = 0);
				virtual ~HTMLBasedInterface();

				void registerAction(HTMLInterfaceAction* action);
				virtual void onNotify(Message* message);

				virtual void setIndexHTML(String const& index_html);
				String const& getIndexHTML();

				virtual void restoreDefaults();

				virtual void applyPreferences();

				PresentaBALLSettings* getSettings();

			signals:
				
				void fireJSActionSignal(int actionType);
				void fireJSMessage(int i, int j);
				
			protected:
				typedef QList<QPair<QString, QString> > ParameterList;
				void contextMenuEvent(QContextMenuEvent* evt);
			
			protected slots:
				
				void test(int i);
				void exposeQObjectToJavascript();
				void handleLinkClicked(const QUrl& url);
				void executeLink(const QUrl& url);
				void executePython_(const QString& action, const ParameterList& parameters);

			private:
				QSignalMapper *signalMapper;
				String index_html_;
				String script_base_;
				QHash<QString, HTMLInterfaceAction*> action_registry_;
				PresentaBALLSettings* settings_;
		};
	}
}

#endif // HTMLBASEDINTERFACE_H
