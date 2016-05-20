#include <jupyterWidget.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{

		JupyterWidget::JupyterWidget(MainControl* parent, const char* title)
			: DockWidget(parent, title)
		{
			registerThis();

			tab_view_ = new QTabWidget(this);
			BALL_ASSIGN_NAME(tab_view_);
			setGuest(*tab_view_);

			JupyterHTMLView* main_view = new JupyterHTMLView(tab_view_, this);

			tab_view_->addTab(main_view, "Dashboard");
			tab_view_->setTabsClosable(true);
		}

		JupyterWidget::~JupyterWidget()
		{ }

		JupyterWidget::JupyterHTMLView::JupyterHTMLView(QWidget* parent, JupyterWidget* base)
			: HTMLView(parent),
				base_(base)
		{ }

		QWebEngineView* JupyterWidget::JupyterHTMLView::createWindow(QWebEnginePage::WebWindowType type)
		{
			return base_->createWindow(type);
		}

		QWebEngineView* JupyterWidget::createWindow(QWebEnginePage::WebWindowType)
		{
			JupyterHTMLView *result = new JupyterHTMLView(tab_view_, this);
			
			result->load(base_url_);

			tab_view_->addTab(result, "Notebook");

			return result;
		}

		void JupyterWidget::setBaseURL(String const& url)
		{
			QWriteLocker lock(&page_lock_);
			base_url_.setUrl(url.c_str());
			reinterpret_cast<JupyterHTMLView*>(tab_view_->currentWidget())->load(base_url_.toString());
		}

		void JupyterWidget::contextMenuEvent(QContextMenuEvent*)
		{ }
	}
}
