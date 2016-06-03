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

		class JupyterWidget : public DockWidget
		{
			Q_OBJECT

			public:
				BALL_EMBEDDABLE(JupyterWidget, DockWidget)

				JupyterWidget(MainControl* parent = 0, const char* title = "");

				virtual ~JupyterWidget();

				void setBaseURL(String const& url);
				virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType /*type*/);

			protected slots:
				void closeTab(int index);
				void renameTab(const QString& title);

			protected:
				void contextMenuEvent(QContextMenuEvent* evt);

				QUrl base_url_;
				QTabWidget* tab_view_;
				JupyterTab* dashboard_;

			private:
				QReadWriteLock page_lock_;
		};
	}
}

#endif // JUPYTERWIDGET_H
