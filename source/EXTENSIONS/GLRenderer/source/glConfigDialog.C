#include <glConfigDialog.h>
#include <glRenderer.h>

namespace BALL
{
	namespace GLRenderer
	{
		GLConfigDialog::GLConfigDialog(QWidget* parent, Qt::WindowFlags f) :
			BALL::VIEW::ConfigDialog(parent, f),
			ui_ {new Ui::OpenGLSettings},
			renderers_ {}
		{
			ui_->setupUi(this);
			registerWidgets_();
			setINIFileSectionName("GLRendererPlugin");
		}

		void GLConfigDialog::registerRenderer(GLRenderer &renderer)
		{
			renderers_.insert(&renderer);
		}

		void GLConfigDialog::unregisterRenderer(BALL::GLRenderer::GLRenderer &renderer)
		{
			renderers_.erase(&renderer);
		}

		void GLConfigDialog::storeValues()
		{
			BALL::VIEW::ConfigDialog::storeValues();
			for(auto renderer: renderers_)
			{
				renderer->setSmoothLines(ui_->smooth_lines_->checkState() == Qt::CheckState::Checked);
				renderer->setPreviewMode(ui_->use_preview->checkState() == Qt::CheckState::Checked);
			}
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
