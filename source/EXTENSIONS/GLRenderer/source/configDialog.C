#include <configDialog.h>
#include <ui_configDialog.h>

GLConfigDialog::GLConfigDialog(QWidget* parent, Qt::WindowFlags f)
	: BALL::VIEW::ConfigDialog(parent, f)
{
	ui_ = new Ui::OpenGLSettings;
	ui_->setupUi(this);
	registerWidgets_();
	setINIFileSectionName("GLRendererPlugin");
}

GLConfigDialog::~GLConfigDialog()
{
	delete ui_;
}
