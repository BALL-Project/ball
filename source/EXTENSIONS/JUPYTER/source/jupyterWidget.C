#include <jupyterWidget.h>

#include <jupyterServer.h>
#include <jupyterServerTab.h>
#include <jupyterTab.h>

#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{
		JupyterWidget::JupyterWidget(MainControl* parent, const char* title)
			: DockWidget(parent, title),
			  tab_view_(new QTabWidget(this)),
			  dashboard_(nullptr),
			  dashboard_url_(),
			  server_(nullptr),
			  server_tab_(nullptr),
			  page_lock_()
		{
			registerThis();

			BALL_ASSIGN_NAME(tab_view_);
			setGuest(*tab_view_);

			dashboard_ = new JupyterTab(tab_view_, this);
			tab_view_->addTab(dashboard_, "Dashboard");
			tab_view_->setTabsClosable(true);
			tab_view_->setMovable(true);

			PyInterpreter::startServer();

			connect(tab_view_, &QTabWidget::tabCloseRequested, this, &JupyterWidget::closeTab);
		}

		JupyterWidget::~JupyterWidget()
		{
			if(server_)
			{
				delete server_;
			}
		}

		void JupyterWidget::setDashboardURL(const QString& url)
		{
			QWriteLocker lock(&page_lock_);
			dashboard_url_.setUrl(url);
			dashboard_->load(dashboard_url_);
		}

		void JupyterWidget::setServer(JupyterServer* server)
		{
			if(server_)
			{
				if(server) // replace server
				{
					server_tab_->setServer(server);
				}
				else // hosted -> external
				{
					tab_view_->removeTab(tab_view_->indexOf(server_tab_));
				}
				delete server_;
			}
			else if(server) // external -> hosted
			{
				server_tab_ = new JupyterServerTab(tab_view_, server);
				tab_view_->insertTab(0, server_tab_, "Server");
				setDashboardURL(QString("http://localhost:%1?token=%2").arg(server->getPort()).arg(server->getToken()));
			}
			server_ = server;
		}

		JupyterServer* JupyterWidget::getServer()
		{
			return server_;
		}

		void JupyterWidget::closeTab(int index)
		{
			if(index == tab_view_->indexOf(dashboard_))
			{
				Log.info() << "[JupyterPlugin] Dashboard cannot be closed" << std::endl;
			}
			else if(index == tab_view_->indexOf(server_tab_))
			{
				Log.info() << "[JupyterPlugin] Server tab cannot be closed" << std::endl;
			}
			else
			{
				tab_view_->removeTab(index);
			}
		}

		void JupyterWidget::renameTab(const QString& title)
		{
			int index = tab_view_->indexOf((JupyterWidget*) sender());
			if(index != -1 && sender() != dashboard_)
			{
				tab_view_->setTabText(index, title);
			}
		}

		void JupyterWidget::contextMenuEvent(QContextMenuEvent*)
		{ }

		QWebEngineView* JupyterWidget::createWindow(QWebEnginePage::WebWindowType)
		{
			JupyterTab* view = new JupyterTab(tab_view_, this);
			tab_view_->addTab(view, "New tab");
			return view;
		}
	}
}
