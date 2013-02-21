#ifndef BALLAXYINTERFACE_H
#define BALLAXYINTERFACE_H

#ifndef BALL_VIEW_WIDGETS_HTMLVIEW_H
	#include <BALL/VIEW/WIDGETS/HTMLView.h>
#endif

#include <QtCore/QHash>

namespace BALL
{
	class AtomContainer;

	namespace VIEW
	{
		class BALL_VIEW_EXPORT BALLaxyInterfaceAction : public QObject
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

		class BALL_VIEW_EXPORT BALLaxyInterface : public HTMLView, public Embeddable
		{
			Q_OBJECT

			public:
				BALL_EMBEDDABLE(BALLaxyInterface, Embeddable)

				BALLaxyInterface(MainControl* parent = 0);

				virtual ~BALLaxyInterface();

				void setBALLaxyBaseUrl(String const& ballaxy_base);

				void registerAction(BALLaxyInterfaceAction* action);

				bool uploadToBallaxy(AtomContainer* ac);

			public slots:
				void sendToBallaxy();

			protected:
				typedef QList<QPair<QString, QString> > ParameterList;
				void contextMenuEvent(QContextMenuEvent* evt);
				QUrl ballaxy_base_;

			protected slots:

				void handleLinkClicked(const QUrl& url);
				void executeLink(const QUrl& url);
				void executePython_(const QString& action, const ParameterList& parameters);
        void handleSslErrors(QNetworkReply* reply, const QList<QSslError> &errors);

			private:
				String script_base_;
				QHash<QString, BALLaxyInterfaceAction*> action_registry_;
				QAction* context_separator_;
				QAction* context_action_;
		};
	}
}

#endif // BALLAXYINTERFACE_H
