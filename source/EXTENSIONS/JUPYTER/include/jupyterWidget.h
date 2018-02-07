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

		/**
		 * [JupyterPlugin]
		 * Main widget of the Jupyter plugin, showing dashboard, notebooks, and the server log (if any).
		 */
		class JupyterWidget : public DockWidget
		{
			Q_OBJECT

			public:
				BALL_EMBEDDABLE(JupyterWidget, DockWidget)

				JupyterWidget();
				~JupyterWidget() override = default;

				/**
				 * Sets the dashboard URL and reloads the dashboard tab.
				 *
				 * @param url dashboard URL
				 */
				void setDashboardURL(const QString& url);

				/**
				 * Sets or replaces the Jupyter server for this plugin (including the server tab). Previous servers will
				 * be stopped and destroyed automatically.
				 *
				 * @param server new server to be used; pass nullptr for externally hosted servers
				 */
				void setServer(JupyterServer* server);

				JupyterServer* getServer() { return server_; };

				/**
				 * Adds a new notebook tab to this widget and returns a pointer to it.
				 *
				 * @return new notebook tab
				 */
				virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType /*type*/);

			public Q_SLOTS:
				/**
				 * Remove the notebook tab with the given index. Dashboard and server tabs cannot be removed.
				 *
				 * @param index valid notebook tab index
				 */
				void closeTab(int index);

				/**
				 * Renames the notebook tab with the given index. Dashboard and server tabs cannot be renamed.
				 *
				 * @param title new notebook tab title
				 */
				void renameTab(const QString& title);

			protected:
				/**
				 * Does nothing.
				 *
				 * @param evt
				 */
				void contextMenuEvent(QContextMenuEvent* /*evt*/) override;

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
