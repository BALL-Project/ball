#include <jupyterTab.h>

#include <jupyterWidget.h>

namespace BALL
{
	namespace VIEW
	{
		JupyterTab::JupyterTab(QWidget* parent, JupyterWidget* base)
				: HTMLView(parent),
				  base_(base)
		{
			connect(this, SIGNAL(loadFinished(bool)), this, SLOT(prepareNotebook(bool)));
		}

		void JupyterTab::prepareNotebook(bool ok)
		{
			if(!ok) return;

			// prevent multiple dashboards from being spawned
			page()->runJavaScript("e = document.getElementById('open_notebook'); if(e) e.parentElement.removeChild(e);");
		}

		QWebEngineView* JupyterTab::createWindow(QWebEnginePage::WebWindowType type)
		{
			return base_->createWindow(type);
		}
	}
}