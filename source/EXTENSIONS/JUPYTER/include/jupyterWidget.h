#ifndef JUPYTERWIDGET_H
#define JUPYTERWIDGET_H

#ifndef BALL_VIEW_WIDGETS_HTMLVIEW_H
	#include <BALL/VIEW/WIDGETS/HTMLView.h>
#endif

#include <QtWidgets/QTabWidget>
#include <QReadWriteLock>

namespace BALL
{
	namespace VIEW
	{
		class JupyterTab;
		class JupyterServer;
		class JupyterServerTab;

		class JupyterWidget : public DockWidget
		{
			Q_OBJECT

			public:
				BALL_EMBEDDABLE(JupyterWidget, DockWidget)

				JupyterWidget(MainControl* parent = nullptr, const char* title = "");
				virtual ~JupyterWidget();

				void setDashboardURL(String const& url);
				void setServer(JupyterServer* server);
				JupyterServer* getServer();

				virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType /*type*/);

			protected slots:
				void closeTab(int index);
				void renameTab(const QString& title);

			protected:
				void contextMenuEvent(QContextMenuEvent* evt);

				QTabWidget* tab_view_;
				JupyterTab* dashboard_;
				QUrl dashboard_url_;
				JupyterServer* server_;
				JupyterServerTab* server_tab_;

			private:
				QReadWriteLock page_lock_;
		};
	}
}

#endif // JUPYTERWIDGET_H
