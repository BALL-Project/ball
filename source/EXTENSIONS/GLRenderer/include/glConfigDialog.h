#ifndef BALL_GLRENDERER_CONFIGDIALOG_H
#define BALL_GLRENDERER_CONFIGDIALOG_H

#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>

namespace Ui
{
	class OpenGLSettings;
}

namespace BALL
{
	namespace GLRenderer
	{
		class BALL_PLUGIN_EXPORT GLConfigDialog : public BALL::VIEW::ConfigDialog
		{
			Q_OBJECT

			public:
				GLConfigDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
				virtual ~GLConfigDialog();

			private:
				Ui::OpenGLSettings* ui_;
		};
	} // namespace GLRenderer
} // namespace BALL

#endif // BALL_GLRENDERER_CONFIGDIALOG_H
