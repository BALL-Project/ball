#include <glConfigDialog.h>

namespace BALL
{
	namespace GLRenderer
	{
		GLConfigDialog::GLConfigDialog(QWidget* parent, Qt::WindowFlags f)
				: BALL::VIEW::ConfigDialog(parent, f),
				  ui_ {new Ui::OpenGLSettings}
		{
			ui_->setupUi(this);
			registerWidgets_();
			setINIFileSectionName("GLRendererPlugin");
		}
	}
}
