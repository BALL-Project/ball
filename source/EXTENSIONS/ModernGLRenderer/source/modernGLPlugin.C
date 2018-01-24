#include <GL/glew.h>

#include <modernGLPlugin.h>
#include <modernGLConfigDialog.h>


ModernGLPlugin::ModernGLPlugin()
	: is_active_(false),
	  icon_(":pluginModernGLRenderer.png"),
	  config_dialog_(0),
	  renderer_(0)
{
}

ModernGLPlugin::~ModernGLPlugin()
{
}

QString ModernGLPlugin::getName() const
{
	//GL_RENDERER​
	return QString("ModernGLRenderer");
}

QString ModernGLPlugin::getDescription() const
{
	return QString("An OpenGL 2.x renderer.");
}

QString ModernGLPlugin::getVersion() const
{
	QString version = QString("undefined");

	if(renderer_ && renderer_->isInitialized())
	{
		version = QString(reinterpret_cast< char const * >(glGetString(GL_VERSION)));
	}

	return version;
}

QString ModernGLPlugin::getVendor() const
{
	QString vendor = QString("undefined");

	if(renderer_ && renderer_->isInitialized())
	{
		vendor = QString(reinterpret_cast< char const * >(glGetString(GL_VENDOR)));
	}

	return vendor;
}

QString ModernGLPlugin::getGraphicsCard() const
{
	QString renderer = QString("undefined");

	if(renderer_ && renderer_->isInitialized())
	{
		renderer = QString(reinterpret_cast< char const * >(glGetString(GL_RENDERER)));
	}

	return renderer;
}

const QPixmap* ModernGLPlugin::getIcon() const
{
	return &icon_;
}

BALL::VIEW::ConfigDialog* ModernGLPlugin::getConfigDialog()
{
	if(!config_dialog_)
	{
		config_dialog_ = new ModernGLConfigDialog;
	}

	updateLabels();

	return config_dialog_;
}

void ModernGLPlugin::updateLabels()
{
	config_dialog_->setDescription(getDescription());
	config_dialog_->setGraphicsCard(getGraphicsCard());
	config_dialog_->setVersion(getVersion());
	config_dialog_->setVendor(getVendor());
}

BALL::VIEW::Renderer* ModernGLPlugin::createRenderer()
{
	renderer_ = new ModernGLRenderer;
	getConfigDialog();
	config_dialog_->setRenderer(renderer_);

	connect(renderer_, SIGNAL(initialized()),
	        this, SLOT(updateLabels()));

	return renderer_;
}
