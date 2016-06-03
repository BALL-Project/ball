#include <jupyterWidget.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{

		JupyterWidget::JupyterHTMLView::JupyterHTMLView(QWidget* parent, JupyterWidget* base)
				: HTMLView(parent),
				  base_(base)
		{ }

		QWebEngineView* JupyterWidget::JupyterHTMLView::createWindow(QWebEnginePage::WebWindowType type)
		{
			return base_->createWindow(type);
		}

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

			connect(tab_view_, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
		}

		JupyterWidget::~JupyterWidget()
		{ }

		void JupyterWidget::setBaseURL(String const& url)
		{
			QWriteLocker lock(&page_lock_);
			base_url_.setUrl(url.c_str());
			reinterpret_cast<JupyterHTMLView*>(tab_view_->widget(0))->load(base_url_.toString());
		}

		void JupyterWidget::closeTab(int index)
		{
			if(index)
			{
				tab_view_->removeTab(index);
			}
			else
			{
				Log.info() << "Jupyter dashboard cannot be closed" << std::endl;
			}
		}

		void JupyterWidget::renameTab(const QString& title)
		{
			int index = tab_view_->indexOf((JupyterWidget*) sender());
			if(index != -1)
			{
				tab_view_->setTabText(index, title);
			}
		}

		void JupyterWidget::contextMenuEvent(QContextMenuEvent*)
		{ }

		QWebEngineView* JupyterWidget::createWindow(QWebEnginePage::WebWindowType)
		{
			JupyterHTMLView *view = new JupyterHTMLView(tab_view_, this);
			view->load(base_url_);
			tab_view_->addTab(view, view->title());
			connect(view, SIGNAL(titleChanged(const QString&)), this, SLOT(renameTab(const QString&)));
			return view;
		}
	}
}
