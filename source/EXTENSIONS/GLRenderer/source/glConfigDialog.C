#include <glConfigDialog.h>
#include <glRenderer.h>

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

		void GLConfigDialog::updateGLInfo(BALL::GLRenderer::GLRenderer& renderer)
		{
			ui_->vendor_label->setText(renderer.getVendor().c_str());
			ui_->renderer_label->setText(renderer.getRenderer().c_str());
			ui_->version_label->setText(renderer.getOpenGLVersion().c_str());
			ui_->extensions_list->clear();
			for(auto& ext: renderer.getExtensions())
			{
				new QListWidgetItem(ext.c_str(), ui_->extensions_list);
			}
		}
	}
}
