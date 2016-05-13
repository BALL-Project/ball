#ifndef BALL_VIEW_WIDGETS_HTMLVIEW_H
#define BALL_VIEW_WIDGETS_HTMLVIEW_H

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
	#include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif
#include <BALL/VIEW/UIC/ui_HTMLViewErrorWidget.h>

#include <QtCore/QUrl>
#include <QtWebEngineWidgets/QWebEngineView>

namespace BALL
{
	namespace VIEW
	{
		class BALL_VIEW_EXPORT HTMLViewErrorWidget :
			public QWidget,
			public Ui_HTMLViewErrorWidget
		{
			public:
				HTMLViewErrorWidget(QWidget* parent = 0)
					: QWidget(parent),
					  Ui_HTMLViewErrorWidget()
				{
					setupUi(this);
				}
		};

		class BALL_VIEW_EXPORT HTMLView : public QWebEngineView
		{
			Q_OBJECT

			public:

				HTMLView(QWidget* parent = 0);
				virtual ~HTMLView();
		};

		class BALL_VIEW_EXPORT HTMLViewDock : public DockWidget
		{
			public:

				BALL_EMBEDDABLE(HTMLViewDock, DockWidget)

				HTMLViewDock(QWidget* parent, const char* title = 0);
				
				HTMLViewDock(HTMLView* view,  QWidget* parent, const char* title = 0);

				void setHTMLView(HTMLView* view);
				HTMLView* getHTMLView();
				const HTMLView* getHTMLView() const;

				/// Re-invoke the most recent setHTMLView() call
				void resetHTMLView(bool skip_checks);

			protected:

				void checkForIncompatibleDrivers_();

				bool skip_checks_;
				bool show_error_;
				HTMLView* html_view_;
		};
	}
}

#endif // BALL_VIEW_WIDGETS_HTMLVIEW_H
