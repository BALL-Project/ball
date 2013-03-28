#ifndef CONFIG_DIALOG_H
#define CONFIG_DIALOG_H

#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>

namespace Ui
{
	class OpenGLSettings;
}

class GLConfigDialog : public BALL::VIEW::ConfigDialog
{
	Q_OBJECT

	public:
		GLConfigDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);
		virtual ~GLConfigDialog();

	private:
		Ui::OpenGLSettings* ui_;
};

#endif
