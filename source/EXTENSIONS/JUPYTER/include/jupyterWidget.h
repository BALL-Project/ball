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
		class JupyterWidget
			: public DockWidget
		{
			Q_OBJECT

			public:
				BALL_EMBEDDABLE(JupyterWidget, DockWidget)

				class JupyterHTMLView : public HTMLView
				{
					public:
						JupyterHTMLView(QWidget* parent, JupyterWidget* base);

					protected:
						JupyterWidget* base_;

						virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType type);
				};

				JupyterWidget(MainControl* parent = 0, const char* title = "");

				virtual ~JupyterWidget();

				void setBaseURL(String const& url);

			protected:
				virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType /*type*/);

			protected:
				void contextMenuEvent(QContextMenuEvent* evt);

				QUrl base_url_;
				QTabWidget* tab_view_;

			private:
				QReadWriteLock page_lock_;
		};
	}
}

#endif // JUPYTERWIDGET_H
