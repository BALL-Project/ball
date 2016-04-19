#include <BALL/VIEW/WIDGETS/HTMLView.h>

namespace BALL
{
	namespace VIEW
	{

		HTMLView::HTMLView(QWidget* parent)
			: QWebEngineView(parent)
		{
			setAttribute(Qt::WA_DontCreateNativeAncestors);
		}

		HTMLView::~HTMLView()
		{
		}

		HTMLViewDock::HTMLViewDock(HTMLView* view, QWidget* parent, const char* title)
			: DockWidget(parent, title),
			  html_view_(0)
		{
			setHTMLView(view);
		}

		HTMLViewDock::HTMLViewDock(QWidget* parent, const char* title)
			: DockWidget(parent, title),
			  html_view_(0)
		{
			setHTMLView(new HTMLView);
		}

		void HTMLViewDock::setHTMLView(HTMLView* html_view)
		{
			if(!html_view || html_view == html_view_)
			{
				return;
			}

			delete html_view_;

			html_view_ = html_view;

			setWidget(html_view_);
		}

	}
}
