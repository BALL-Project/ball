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
			connect(this, SIGNAL(titleChanged(const QString&)), base_, SLOT(renameTab(const QString&)));
			connect(this, SIGNAL(loadFinished(bool)), this, SLOT(prepareNotebook(bool)));
		}

		void JupyterTab::prepareNotebook(bool ok)
		{
			if(!ok || page()->title() != "IPython Notebook") return;

			// prevent multiple dashboards from being spawned
			page()->runJavaScript("e = document.getElementById('ipython_notebook'); if(e) e.childNodes[0].href='#';");
			page()->runJavaScript("e = document.getElementById('open_notebook'); if(e) e.parentElement.removeChild(e);");
			// remove download options (unfortunately, the corresponding menu entry lacks an ID...)
			page()->runJavaScript("e = document.getElementById('print_preview').nextElementSibling; if(e) e.parentElement.removeChild(e);");
		}

		QWebEngineView* JupyterTab::createWindow(QWebEnginePage::WebWindowType type)
		{
			return base_->createWindow(type);
		}
	}
}
