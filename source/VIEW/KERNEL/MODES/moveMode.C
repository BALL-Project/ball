#include <BALL/VIEW/KERNEL/MODES/moveMode.h>

#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/RENDERING/camera.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

#define  ROTATE_FACTOR    50.
#define  ROTATE_FACTOR2   50.
#define  TRANSLATE_FACTOR  4.
#define  ZOOM_FACTOR       7.

namespace BALL
{
	namespace VIEW
	{
		/// Different Mouse Mode actions
		enum ModeAction
		{
			///
			TRANSLATE_ACTION,

			///
			ZOOM_ACTION,

			///
			ROTATE_ACTION,

			///
			ROTATE_CLOCKWISE_ACTION
		};


		MoveMode::MoveMode(Scene* scene)
			: InteractionMode(scene),
			  move_atom_(0)
		{
			String description = "Shortcut|Display|Move_Mode";
			main_action_ = scene_->insertMenuEntry(MainControl::DISPLAY, tr("Move Mode"), 0,
			                                       0, description, QKeySequence("Ctrl+W"), 
			                                       tr("Switch to move mode, e.g. move selected items"),
																						 UIOperationMode::MODE_ADVANCED);

			if (main_action_)
			{
				scene_->setMenuHelp(main_action_, "molecularControl.html#move_molecule");
				scene_->setIcon(main_action_, "actions/transform-move", false);
				main_action_->setCheckable(true);
				connect(main_action_, SIGNAL(triggered()), SLOT(modeChangeSlot_()));
			}
		}

		void MoveMode::mouseMoveEventImpl_(QMouseEvent* evt)
		{
			processMoveModeMouseEvents_(evt);
		}

		void MoveMode::mouseReleaseEventImpl_(QMouseEvent* evt)
		{
			processMoveModeMouseEvents_(evt);
		}

		Index MoveMode::getMoveModeAction_(QMouseEvent* e)
		{
			// TODO make keys configurable in shortcutRegistry 
			if (e->modifiers() == Qt::NoModifier)
			{
				if (e->buttons() == Qt::LeftButton)  return TRANSLATE_ACTION;
				if (e->buttons() == Qt::MidButton)   return ZOOM_ACTION;
				if (e->buttons() == Qt::RightButton) return ROTATE_ACTION;
			}
			else if (e->buttons() == Qt::LeftButton)
			{
				if (e->modifiers() == Qt::ShiftModifier) return ZOOM_ACTION;
				if (e->modifiers() == Qt::ControlModifier) return ROTATE_ACTION;
				if (e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier)) return ROTATE_CLOCKWISE_ACTION;
			}
			else if (e->buttons() == (Qt::LeftButton | Qt::RightButton))
			{
				return ROTATE_CLOCKWISE_ACTION;
			}

			return ROTATE_ACTION;
		}


		void MoveMode::processMoveModeMouseEvents_(QMouseEvent* e)
		{
			Camera& camera = scene_->getStage()->getCamera();

			// Difference between the old and new position in the window 
			float delta_x = getXDiff_() / 4.0;
			float delta_y = getYDiff_() / 4.0;

			// stop if no movement
			if (delta_x == 0 && delta_y == 0) return;

			Index action = getMoveModeAction_(e);

			// if we have a selection of Composites, we perform the moving here:
			const HashSet<Composite*>& selection = scene_->getMainControl()->getSelection();
			if (selection.size() != 0)
			{
				list<Composite*> composites;
				std::copy(selection.begin(), selection.end(), std::front_inserter(composites));

				switch (action)
				{
					case ZOOM_ACTION:
						{
							scene_->moveComposites(composites, Vector3(0,0, delta_y * ZOOM_FACTOR));
							return;
						}

					case TRANSLATE_ACTION:
						{
							scene_->moveComposites(composites, Vector3(delta_x * TRANSLATE_FACTOR ,-delta_y * TRANSLATE_FACTOR, 0));
							return;
						}

					case ROTATE_ACTION:
						{
							scene_->rotateComposites(composites, delta_x * ROTATE_FACTOR * 4., delta_y * ROTATE_FACTOR * 4., 0);
							return;
						}

					case ROTATE_CLOCKWISE_ACTION:
						{
							scene_->rotateComposites(composites, 0, 0, delta_x * ROTATE_FACTOR * 4.);
							return;
						}
				}

				return;
			}

			// moving for Clipping Planes is done in DatasetControl:
			// we create the Matrix for this here and send it per Message
			Matrix4x4 m;

			switch (action)
			{
				case ZOOM_ACTION:
					{
						Vector3 v = -scene_->getStage()->getCamera().getViewVector();
						if (Maths::isZero(v.getSquareLength())) v = Vector3(1,0,0);
						v.normalize();

						v *= delta_y * ZOOM_FACTOR * 1.5;

						m.setTranslation(v);
						break;
					}

				case TRANSLATE_ACTION:
					{
						// calculate translation in x-axis direction
						Vector3 right_translate = camera.getRightVector() * delta_x * TRANSLATE_FACTOR * 3.0;

						// calculate translation in y-axis direction
						Vector3 up_translate = camera.getLookUpVector() * delta_y * TRANSLATE_FACTOR * 3.0;

						m.setTranslation(right_translate - up_translate);
						break;
					}

				case ROTATE_ACTION:
					{
						delta_x *= ROTATE_FACTOR * 4.;
						delta_y *= ROTATE_FACTOR * 4.;
						float angle_total = fabs(delta_x) + fabs(delta_y);

						Vector3 rotation_axis = (camera.getLookUpVector() * delta_x / angle_total) +
							(camera.getRightVector()  * delta_y / angle_total);

						m.rotate(Angle(angle_total, false), rotation_axis);
						break;
					}

				case ROTATE_CLOCKWISE_ACTION:
				default:
					delta_x *= ROTATE_FACTOR2;
					Vector3 rotation_axis = camera.getViewVector();
					m.rotate(Angle(delta_x, false), rotation_axis);
			}

			TransformationMessage* msg = new TransformationMessage;
			msg->setMatrix(m);
			scene_->notify(msg);
		}

		Qt::Key MoveMode::getKey() const
		{
			return Qt::Key_W;
		}

		void MoveMode::activate()
		{
			InteractionMode::activate();

			scene_->setCursor(QCursor(Qt::SizeAllCursor));
		}

		void MoveMode::populateContextMenu(QMenu* menu)
		{
			if(move_atom_ == 0)
			{
				move_atom_ = new QAction(tr("Move Atom"), this);
				connect(move_atom_, SIGNAL(triggered()), this, SLOT(moveAtomTriggered_()));
			}

			move_atom_->setEnabled(scene_->getCurrentAtom() != 0);
			menu->addAction(move_atom_);
		}

		void MoveMode::moveAtomTriggered_()
		{
			if (scene_->getCurrentAtom() == 0) return;

			scene_->getCurrentAtom()->setSelected(true);
			scene_->notify(new CompositeMessage(*scene_->getCurrentAtom(), CompositeMessage::SELECTED_COMPOSITE));

			emit InteractionMode::requestModeChange(this);
		}

	}
}
