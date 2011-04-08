#include <BALL/VIEW/KERNEL/MODES/rotateMode.h>

#define  ROTATE_FACTOR    50.
#define  ROTATE_FACTOR2   50.
#define  TRANSLATE_FACTOR 4.

#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{

		RotateMode::RotateMode(Scene* scene)
			: InteractionMode(scene)
		{
			String description = "Shortcut|Display|Rotate_Mode";
			scene_->getMainControl()->insertPopupMenuSeparator(MainControl::DISPLAY, UIOperationMode::MODE_ADVANCED);
			main_action_ = scene_->insertMenuEntry(MainControl::DISPLAY, tr("&Rotate Mode"), this, 
			                                       0, description, QKeySequence("Ctrl+R"),
																						 tr("Switch to rotate/zoom mode"), UIOperationMode::MODE_ADVANCED);

			if (main_action_)
			{
				scene_->setMenuHelp(main_action_, "scene.html#rotate_mode");
				main_action_->setCheckable(true);
				scene_->setIcon(main_action_, "actions/transform-rotate", false);
				connect(main_action_, SIGNAL(triggered()), SLOT(modeChangeSlot_()));
			}
		}

		void RotateMode::mouseDoubleClickEvent(QMouseEvent* /*evt*/)
		{
			scene_->showInfos();
		}

		void RotateMode::mouseMoveEventImpl_(QMouseEvent* evt)
		{
			processRotateModeMouseEvents_(evt);
		}

		void RotateMode::mouseReleaseEventImpl_(QMouseEvent* evt)
		{
			processRotateModeMouseEvents_(evt);
//			light_settings_->updateFromStage();
		}

		void RotateMode::processRotateModeMouseEvents_(QMouseEvent* e)
		{
			if(getXDiff_() == 0 && getYDiff_() == 0)
			{
				return;
			}

			switch ((Index)(e->buttons() | e->modifiers()))
			{
				case (Qt::SHIFT | Qt::LeftButton):
				case  Qt::MidButton:
					zoomSystem_();
					break;

				case (Qt::CTRL | Qt::LeftButton):
				case  Qt::RightButton:
					translateSystem_();
					break;

				case ((Index)Qt::LeftButton | Qt::RightButton):
				case (Qt::SHIFT | Qt::CTRL | Qt::LeftButton):
					rotateSystemClockwise_();
					break;

				case Qt::LeftButton:
					rotateSystem_();
					break;

				default:
					break;
			}

			if (scene_->isRecording())
			{
				scene_->captureCamera();
			}
		}

		Qt::Key RotateMode::getKey() const
		{
			return Qt::Key_R;
		}

		void RotateMode::activate()
		{
			InteractionMode::activate();

			scene_->setCursor(QCursor(Qt::ArrowCursor));
		}

		void RotateMode::rotateSystem_()
		{
			float x = getXDiff_() * ROTATE_FACTOR;
			float y = getYDiff_() * ROTATE_FACTOR;

			scene_->rotate(-x, -y);
		}

		void RotateMode::rotateSystemClockwise_()
		{
			float x = getXDiff_();

			if (x == 0) return;

			x *= ROTATE_FACTOR2;

			scene_->rotateClockwise(x);
		}

		void RotateMode::translateSystem_()
		{
			float x = getXDiff_() * TRANSLATE_FACTOR;
			float y = getYDiff_() * TRANSLATE_FACTOR;
			scene_->move(Vector3(x, -y, 0));
		}

	}
}
