#include <BALL/VIEW/KERNEL/MODES/interactionMode.h>

#include <BALL/VIEW/WIDGETS/scene.h>

#include <QtWidgets/QDesktopWidget>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QApplication>

#define  ZOOM_FACTOR      7.

namespace BALL
{
	namespace VIEW
	{
		InteractionMode::InteractionMode(Scene* scene)
			: scene_(scene),
			  desktop_size_(qApp->desktop()->size()),
			  mouse_sensitivity_(5.0f),
			  mouse_wheel_sensitivity_(5.0f),
			  main_action_(0)
		{
		}

		InteractionMode::~InteractionMode()
		{
		}

		void InteractionMode::modeChangeSlot_()
		{
			emit requestModeChange(this);
		}

		void InteractionMode::addToolBarEntries(QToolBar* tb)
		{
			tb->addAction(main_action_);
		}

		void InteractionMode::keyPressEvent(QKeyEvent*)
		{
		}

		void InteractionMode::keyReleaseEvent(QKeyEvent*)
		{
		}

		void InteractionMode::mouseDoubleClickEvent(QMouseEvent*)
		{
		}

		void InteractionMode::mouseMoveEvent(QMouseEvent* evt)
		{
			mouse_pos_new_ = evt->globalPos();

			mouseMoveEventImpl_(evt);

			mouse_pos_old_ = mouse_pos_new_;
		}

		void InteractionMode::mousePressEvent(QMouseEvent* evt)
		{
			mouse_pos_new_ = evt->globalPos();

			mousePressEventImpl_(evt);

			mouse_pos_old_ = mouse_pos_new_;
		}

		void InteractionMode::mouseReleaseEvent(QMouseEvent* evt)
		{
			mouse_pos_new_ = evt->globalPos();

			mouseReleaseEventImpl_(evt);

			mouse_pos_old_ = mouse_pos_new_;
		}

		void InteractionMode::wheelEvent(QWheelEvent* evt)
		{
			scene_->setInfo("");

			evt->accept();

			mouse_pos_new_.setY(mouse_pos_old_.y() + (evt->delta() / 120. * mouse_wheel_sensitivity_));
			zoomSystem_();
		}

		void InteractionMode::mouseMoveEventImpl_(QMouseEvent*)
		{
		}

		void InteractionMode::mousePressEventImpl_(QMouseEvent*)
		{
		}

		void InteractionMode::mouseReleaseEventImpl_(QMouseEvent*)
		{
		}

		float InteractionMode::getXDiff_() const
		{
			float delta_x = mouse_pos_new_.x() - mouse_pos_old_.x();
			delta_x /= desktop_size_.width();
			delta_x *= mouse_sensitivity_;
			return delta_x;
		}

		float InteractionMode::getYDiff_() const
		{
			float delta_y = mouse_pos_new_.y() - mouse_pos_old_.y();
			delta_y /= desktop_size_.height();
			delta_y *= mouse_sensitivity_;
			return delta_y;
		}

		void InteractionMode::zoomSystem_()
		{
			float y = getYDiff_() * ZOOM_FACTOR;

			if (scene_->getStage()->getCamera().getProjectionMode() == Camera::PERSPECTIVE)
			{
				scene_->move(Vector3(0, 0, y));
			}
			else
			{
				Log.error() << "Orthographic zoom not yet implemented" << std::endl;
/*
				float scale = std::max(0.05, 1.-y);

				gl_renderer_->setOrthographicZoom(gl_renderer_->getOrthographicZoom() * scale);
				gl_renderer_->initPerspective();
*/
				scene_->updateGL();
			}
		}


		void InteractionMode::setMouseSensitivity(float sens)
		{
			mouse_sensitivity_ = sens;
		}

		QAction* InteractionMode::getMainAction()
		{
			return main_action_;
		}

		void InteractionMode::deactivate()
		{
			setCurrent(false);
			if(main_action_) main_action_->setChecked(false);
		}

		void InteractionMode::activate()
		{
			setCurrent(true);
			if(main_action_) main_action_->setChecked(true);
		}

	}
}
