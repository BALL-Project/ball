#ifndef JUPYTERTAB_H
#define JUPYTERTAB_H

#include <BALL/VIEW/WIDGETS/HTMLView.h>

namespace BALL
{
	namespace VIEW
	{
		class JupyterWidget;

		class JupyterTab : public HTMLView
		{
			Q_OBJECT

			public:
				JupyterTab(QWidget* parent, JupyterWidget* base);

			protected Q_SLOTS:
				void prepareNotebook(bool ok);

			protected:
				virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType type) override;

				JupyterWidget* base_;
		};
	}
}

#endif // JUPYTERTAB_H
