#include <glPlugin.h>
#include <glRenderer.h>
#include <configDialog.h>

GLPlugin::GLPlugin()
	: is_active_(false),
	  icon_(":pluginGLRenderer.png"),
	  config_dialog_(0)
{
}

GLPlugin::~GLPlugin()
{
}

QString GLPlugin::getName() const
{
	return QString("GLRenderer");
}

QString GLPlugin::getDescription() const
{
	return QString("An OpenGL 1.x renderer.");
}

const QPixmap* GLPlugin::getIcon() const
{
	return &icon_;
}

BALL::VIEW::ConfigDialog* GLPlugin::getConfigDialog()
{
	if(!config_dialog_){
		config_dialog_ = new GLConfigDialog;
	}

	return config_dialog_;
}

BALL::VIEW::Renderer* GLPlugin::createRenderer()
{
	return new BALL::VIEW::GLRenderer;
}
