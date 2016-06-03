#include <jupyterWidget.h>

#include <jupyterTab.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{
		JupyterWidget::JupyterWidget(MainControl* parent, const char* title)
			: DockWidget(parent, title),
			  base_url_(),
			  tab_view_(new QTabWidget(this)),
			  dashboard_(0)
		{
			registerThis();

			BALL_ASSIGN_NAME(tab_view_);
			setGuest(*tab_view_);

			dashboard_ = new JupyterTab(tab_view_, this);
			tab_view_->addTab(dashboard_, "Home");
			tab_view_->setTabsClosable(true);
			tab_view_->setMovable(true);

			connect(tab_view_, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
		}

		JupyterWidget::~JupyterWidget()
		{ }

		void JupyterWidget::setBaseURL(String const& url)
		{
			QWriteLocker lock(&page_lock_);
			base_url_.setUrl(url.c_str());
			dashboard_->load(base_url_.toString());
		}

		void JupyterWidget::closeTab(int index)
		{
			if(index != tab_view_->indexOf(dashboard_))
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
			JupyterTab* view = new JupyterTab(tab_view_, this);
			tab_view_->addTab(view, view->title());
			connect(view, SIGNAL(titleChanged(const QString&)), this, SLOT(renameTab(const QString&)));
			return view;
		}
	}
}
