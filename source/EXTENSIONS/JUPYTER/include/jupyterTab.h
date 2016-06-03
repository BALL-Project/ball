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

			protected slots:
				void prepareNotebook(bool ok);

			protected:
				virtual QWebEngineView* createWindow(QWebEnginePage::WebWindowType type);

				JupyterWidget* base_;
		};
	}
}

#endif // JUPYTERTAB_H
