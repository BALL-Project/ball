#ifndef IPYTHONINTERFACE_H
#define IPYTHONINTERFACE_H

#ifndef BALL_VIEW_WIDGETS_HTMLVIEW_H
	#include <BALL/VIEW/WIDGETS/HTMLView.h>
#endif

#include <QtWidgets/QTabWidget>
#include <QtCore/QHash>
#include <QtNetwork/QNetworkReply>
#include <QReadWriteLock>

namespace BALL
{
	class AtomContainer;

	namespace VIEW
	{
		class IPythonWidget 
			: public DockWidget
		{
			Q_OBJECT

			public:
				BALL_EMBEDDABLE(IPythonWidget, DockWidget)

				class IPythonHTMLView : public HTMLView
				{
					public:
						IPythonHTMLView(QWidget* parent, IPythonWidget* base);

					protected:
						IPythonWidget* base_;

						virtual QWebView* createWindow(QWebPage::WebWindowType type);
				};

				IPythonWidget(MainControl* parent = 0, const char* title = "");

				virtual ~IPythonWidget();

				void setIPythonURL(String const& url);

			protected:
				virtual QWebView* createWindow(QWebPage::WebWindowType type);

			public slots:
//				void handleDownload(QNetworkReply* request);
				void loadFinished(bool ok);

			protected:
				void contextMenuEvent(QContextMenuEvent* evt);
				QUrl ipython_url;

				QTabWidget* tab_view_;

			protected slots:

				void handleLinkClicked(const QUrl& url);
        void handleSslErrors(QNetworkReply* reply, const QList<QSslError> &errors);

			private:
				QReadWriteLock page_lock_;
		};
	}
}

#endif // IPYTHONINTERFACE_H
