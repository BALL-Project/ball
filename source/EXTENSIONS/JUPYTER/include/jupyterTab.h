#ifndef JUPYTERTAB_H
#define JUPYTERTAB_H

#include <BALL/VIEW/WIDGETS/HTMLView.h>

namespace BALL
{
	namespace VIEW
	{
		class JupyterWidget;

		/**
		 * [JupyterPlugin]
		 * Single notebook or dashboard tab for the Jupyter widget.
		 */
		class JupyterTab : public HTMLView
		{
			Q_OBJECT

			public:
				/**
				 * Creates a new notebook or dashboard tab.
				 *
				 * @param parent parent object for Qt-handled memory management
				 * @param base Jupyter widget this tab is to be shown in
				 */
				JupyterTab(QWidget* parent, JupyterWidget* base);

				~JupyterTab() override = default;

			protected Q_SLOTS:
				/**
				 * Prepares the notebook tab for display in the Jupyter widget. This mostly boils down to removing
				 * Jupyter notebook functions that are not available in the widget and to preventing multiple
				 * dashboard tabs to be opened at the same time.
				 *
				 * @param ok true if the tab has been loaded successfully (cf. QWebEngineView::loadFinished)
				 */
				void prepareNotebook(bool ok);

			protected:
				/**
				 * Adds a new notebook tab to the Jupyter widget (via JupyterWidget::createWindow) and returns a pointer
				 * to it.
				 *
				 * Automatically invoked when requesting a new window from this tab (e.g., by clicking on a notebook
				 * link in the dashboard tab).
				 *
				 * @return new notebook tab
				 */
				QWebEngineView* createWindow(QWebEnginePage::WebWindowType /*type*/) override;

				JupyterWidget* base_ {nullptr};
		};
	}
}

#endif // JUPYTERTAB_H
