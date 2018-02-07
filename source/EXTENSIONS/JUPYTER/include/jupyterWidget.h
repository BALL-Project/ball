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

				JupyterWidget();
				~JupyterWidget() override = default;

				void setDashboardURL(const QString& url);
				void setServer(JupyterServer* server);
				JupyterServer* getServer() { return server_; };

				virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType /*type*/);

			public Q_SLOTS:
				void closeTab(int index);
				void renameTab(const QString& title);

			protected:
				void contextMenuEvent(QContextMenuEvent* evt) override;

				QTabWidget* tab_view_ {new QTabWidget(this)};
				JupyterTab* dashboard_ {nullptr};
				QUrl dashboard_url_ {};
				JupyterServer* server_ {nullptr};
				JupyterServerTab* server_tab_ {nullptr};

			private:
				QReadWriteLock page_lock_ {};
		};
	}
}

#endif // JUPYTERWIDGET_H
