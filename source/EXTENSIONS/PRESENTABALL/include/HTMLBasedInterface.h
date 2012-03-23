#ifndef HTMLBASEDINTERFACE_H
#define HTMLBASEDINTERFACE_H

#ifndef BALL_VIEW_WIDGETS_HTMLVIEW_H
	#include <BALL/VIEW/WIDGETS/HTMLView.h>
#endif

#include <QtCore/QHash>

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

		class BALL_VIEW_EXPORT HTMLBasedInterface : public HTMLView, public ModularWidget
		{
			Q_OBJECT
			BALL_EMBEDDABLE(HTMLBasedInterface, ModularWidget)

			public:
				HTMLBasedInterface(QWidget* parent = 0, const char* name = 0);
				virtual ~HTMLBasedInterface();

				void registerAction(HTMLInterfaceAction* action);
				virtual void onNotify(Message* message); 

			signals:
				
				void fireJSCompositeMessage(int i);
				void fireJSRepresentationMessage(int i);
			
			protected:
				typedef QList<QPair<QString, QString> > ParameterList;
				void contextMenuEvent(QContextMenuEvent* evt);
			
			protected slots:

				void exposeQObjectToJavascript();
				void handleLinkClicked(const QUrl& url);
				void executeLink(const QUrl& url);
				void executePython_(const QString& action, const ParameterList& parameters);

			private:
				String script_base_;
				QHash<QString, HTMLInterfaceAction*> action_registry_;
		};
	}
}

#endif // HTMLBASEDINTERFACE_H
