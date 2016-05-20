#ifndef IPYTHONINTERFACE_H
#define IPYTHONINTERFACE_H

#ifndef BALL_VIEW_WIDGETS_HTMLVIEW_H
	#include <BALL/VIEW/WIDGETS/HTMLView.h>
#endif

#include <QtWidgets/QTabWidget>
#include <QReadWriteLock>

namespace BALL
{
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

						virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType type);
				};

				IPythonWidget(MainControl* parent = 0, const char* title = "");

				virtual ~IPythonWidget();

				void setIPythonURL(String const& url);

			protected:
				virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType /*type*/);

			protected:
				void contextMenuEvent(QContextMenuEvent* evt);
				QUrl ipython_url;

				QTabWidget* tab_view_;

			private:
				QReadWriteLock page_lock_;
		};
	}
}

#endif // IPYTHONINTERFACE_H
