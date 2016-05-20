#include <iPythonWidget.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{

		IPythonWidget::IPythonWidget(MainControl* parent, const char* title)
			: DockWidget(parent, title)
		{
			registerThis();

			tab_view_ = new QTabWidget(this);
			BALL_ASSIGN_NAME(tab_view_);
			setGuest(*tab_view_);

			IPythonHTMLView* main_view = new IPythonHTMLView(tab_view_, this);

			tab_view_->addTab(main_view, "iPython");
			tab_view_->setTabsClosable(true);
		}

		IPythonWidget::~IPythonWidget()
		{ }

		IPythonWidget::IPythonHTMLView::IPythonHTMLView(QWidget* parent, IPythonWidget* base)
			: HTMLView(parent),
				base_(base)
		{ }

		QWebEngineView* IPythonWidget::IPythonHTMLView::createWindow(QWebEnginePage::WebWindowType type)
		{
			return base_->createWindow(type);
		}

		QWebEngineView* IPythonWidget::createWindow(QWebEnginePage::WebWindowType)
		{
			IPythonHTMLView *result = new IPythonHTMLView(tab_view_, this);
			
			result->load(ipython_url);

			tab_view_->addTab(result, "iPython");

			return result;
		}

		void IPythonWidget::setIPythonURL(String const& url)
		{
			QWriteLocker lock(&page_lock_);
			ipython_url.setUrl(url.c_str());
			reinterpret_cast<IPythonHTMLView*>(tab_view_->currentWidget())->load(ipython_url.toString());
		}

		void IPythonWidget::contextMenuEvent(QContextMenuEvent*)
		{ }
	}
}
