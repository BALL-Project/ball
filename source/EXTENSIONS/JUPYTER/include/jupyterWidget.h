#ifndef JUPYTERWIDGET_H
#define JUPYTERWIDGET_H

#include <BALL/VIEW/WIDGETS/HTMLView.h>

#include <QtCore/QReadWriteLock>
#include <QtWidgets/QTabWidget>

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

				void setDashboardURL(const QString& url);
				void setServer(JupyterServer* server);
				JupyterServer* getServer();

				virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType /*type*/);

			public slots:
				void closeTab(int index);
				void renameTab(const QString& title);

			protected:
				void contextMenuEvent(QContextMenuEvent* evt) override;

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
