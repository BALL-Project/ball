#include <BALL/VIEW/KERNEL/MODES/interactionModeManager.h>

#include <BALL/VIEW/WIDGETS/scene.h>

#include <BALL/VIEW/KERNEL/MODES/editMode.h>
#include <BALL/VIEW/KERNEL/MODES/moveMode.h>
#include <BALL/VIEW/KERNEL/MODES/rotateMode.h>
#include <BALL/VIEW/KERNEL/MODES/pickingMode.h>

#include <QtGui/QMenu>

namespace BALL
{
	namespace VIEW
	{
		InteractionModeManager::InteractionModeManager(Scene* scene)
			: stereo_mode_(Renderer::NO_STEREO),
			  current_mode_(0),
			  last_mode_(0),
			  scene_(scene)
		{
		}

		void InteractionModeManager::init()
		{
			actions_ = new QActionGroup(this);
			//Rotate mode is the default mode
			setMode_(new RotateMode(scene_));

			modes_.push_back(current_mode_);
			modes_.push_back(new EditMode(scene_));
			modes_.push_back(new MoveMode(scene_));
			modes_.push_back(new PickingMode(scene_));

			for(std::list<InteractionMode*>::iterator it = modes_.begin(); it != modes_.end(); ++it)
			{
				if((*it)->getMainAction())
				{
					actions_->addAction((*it)->getMainAction());
					connect(*it, SIGNAL(requestModeChange(InteractionMode*)), this, SLOT(setMode_(InteractionMode*)));
				}
			}
		}

		QString InteractionModeManager::getCurrentModeName() const
		{
			return current_mode_ ? current_mode_->getName() : QString::null;
		}

		void InteractionModeManager::keyPressEvent(QKeyEvent* evt)
		{
			for(std::list<InteractionMode*>::iterator it = modes_.begin(); it != modes_.end(); ++it)
			{
				if(evt->key() == (*it)->getKey())
				{
					setMode_(*it);
					return;
				}
			}

			if(handleStereoKeyPress_(evt))
			{
				return;
			}

			//Let the current mode process the key event
			current_mode_->keyPressEvent(evt);
		}

		void InteractionModeManager::setMode_(InteractionMode* new_mode)
		{
			if(new_mode == 0)
			{
				return;
			}

			last_mode_ = current_mode_;
			current_mode_ = new_mode;

			if(last_mode_) last_mode_->deactivate();
			current_mode_->activate();
		}

		void InteractionModeManager::switchToLastMode()
		{
			setMode_(last_mode_);
		}

		bool InteractionModeManager::handleStereoKeyPress_(QKeyEvent* evt)
		{
			// TODO make keys configurable in shortcutRegistry 
			if ((stereo_mode_ == Renderer::NO_STEREO) &&
					(evt->key() == Qt::Key_Escape))
			{
				setMode_(last_mode_);
				return true;
			}

			if (stereo_mode_ == Renderer::NO_STEREO)
			{
				evt->ignore();
				return false;
			}

			// TODO make keys configurable in shortcutRegistry 
			if ((evt->key() == Qt::Key_Y && evt->modifiers() == Qt::AltModifier) ||
					evt->key() == Qt::Key_Escape)
			{
				scene_->exitStereo();
				return true;
			}

			// TODO make keys configurable in shortcutRegistry 
			// setting of eye and focal distance
			if (evt->key() != Qt::Key_Left  &&
					evt->key() != Qt::Key_Right &&
					evt->key() != Qt::Key_Up    &&
					evt->key() != Qt::Key_Down)
			{
				evt->ignore();
				return false;
			}

			// TODO make keys configurable in shortcutRegistry 
			// setting of eye distance
			if (evt->key() == Qt::Key_Left ||
					evt->key() == Qt::Key_Right)
			{
				float new_distance = scene_->getStage()->getEyeDistance();

				// TODO make keys configurable in shortcutRegistry 
				float modifier = 1;
				if (evt->key() == Qt::Key_Left)
				{
					modifier = -0.1;
				}
				else
				{
					modifier = +0.1;
				}

				// TODO make keys configurable in shortcutRegistry 
				if (evt->modifiers() == Qt::ShiftModifier)
				{
					modifier *= 10;
				}

				new_distance += modifier;

				// prevent strange values
				if (new_distance < 0)
				{
					new_distance = 0;
				}

				if (new_distance > 4)
				{
					new_distance = 4;
				}

				scene_->getStage()->setEyeDistance(new_distance);
			}
			// setting of focal distance
			else
			{
				float new_focal_distance = scene_->getStage()->getFocalDistance();

				// TODO make keys configurable in shortcutRegistry 
				float modifier = 1;
				if (evt->key() == Qt::Key_Down)
				{
					modifier = -1;
				}
				else
				{
					modifier = +1;
				}

				// TODO make keys configurable in shortcutRegistry 
				if (evt->modifiers() == Qt::ShiftModifier)
				{
					modifier *= 10;
				}

				new_focal_distance += modifier;

				// prevent strange values
				if (new_focal_distance < 7)
				{
					new_focal_distance = 7;
				}

				if (new_focal_distance > 1000)
				{
					new_focal_distance = 1000;
				}

				scene_->getStage()->setFocalDistance(new_focal_distance);
			}

			//TODO: readd this
			//stage_settings_->updateFromStage();
			scene_->updateGL();

			return true;
		}

		void InteractionModeManager::keyReleaseEvent(QKeyEvent* evt)
		{
			current_mode_->keyReleaseEvent(evt);
		}

		void InteractionModeManager::mouseDoubleClickEvent(QMouseEvent* evt)
		{
			current_mode_->mouseDoubleClickEvent(evt);
		}

		void InteractionModeManager::mouseMoveEvent(QMouseEvent* evt)
		{
			current_mode_->mouseMoveEvent(evt);
		}

		void InteractionModeManager::mousePressEvent(QMouseEvent* evt)
		{
			current_mode_->mousePressEvent(evt);
		}

		void InteractionModeManager::mouseReleaseEvent(QMouseEvent* evt)
		{
			current_mode_->mouseReleaseEvent(evt);
		}

		void InteractionModeManager::wheelEvent(QWheelEvent* evt)
		{
			current_mode_->wheelEvent(evt);
		}

		void InteractionModeManager::populateContextMenu(QMenu* menu)
		{
			for(std::list<InteractionMode*>::iterator it = modes_.begin(); it != modes_.end(); ++it)
			{
				menu->addAction((*it)->getMainAction());
			}

			menu->addSeparator();

			for(std::list<InteractionMode*>::iterator it = modes_.begin(); it != modes_.end(); ++it)
			{
				(*it)->populateContextMenu(menu);
			}
		}

		void InteractionModeManager::addToolBarEntries(QToolBar* tb)
		{
			for(std::list<InteractionMode*>::iterator it = modes_.begin(); it != modes_.end(); ++it)
			{
				(*it)->addToolBarEntries(tb);
			}
		}

	}
}
