#ifndef BALLAXYINTERFACE_H
#define BALLAXYINTERFACE_H

#include <BALL/VIEW/WIDGETS/HTMLView.h>

#include <QWebEngineDownloadItem>

namespace BALL
{
	class AtomContainer;

	namespace VIEW
	{
		class BALLaxyInterface : public HTMLView, public Embeddable
		{
			Q_OBJECT

			public:
				BALL_EMBEDDABLE(BALLaxyInterface, Embeddable)

				BALLaxyInterface(MainControl* parent = 0);

				virtual ~BALLaxyInterface();

				void setBALLaxyBaseUrl(const String& ballaxy_base);
				bool uploadToBallaxy(AtomContainer* ac, const String& format);

			public slots:
				void sendPDBToBallaxy();
				void sendMOL2ToBallaxy();
				void verifyDownloadRequest(QWebEngineDownloadItem* request);
				void openStructure();

			protected:
				void contextMenuEvent(QContextMenuEvent* evt);
				QUrl ballaxy_base_;

			private:
				QMenu*   context_submenu_;
				QAction* context_submenu_action_;
				QAction* context_separator_;
				QAction* context_action_pdb_;
				QAction* context_action_mol2_;
		};
	}
}

#endif // BALLAXYINTERFACE_H
