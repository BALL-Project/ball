#include <BALL/VIEW/KERNEL/MODES/pickingMode.h>

#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

namespace BALL
{
	namespace VIEW
	{

		PickingMode::PickingMode(Scene* scene)
			: InteractionMode(scene),
			  pick_select_(false),
			  ignore_pick_(false)
		{
			String description = "Shortcut|Display|Picking_Mode";
			main_action_ = scene_->insertMenuEntry(MainControl::DISPLAY, tr("&Picking Mode"), this,
			                                       0, description, QKeySequence("Ctrl+P"),
                                             tr("Switch to picking mode, e.g. to identify single atoms or groups"),
																						 UIOperationMode::MODE_ADVANCED);

			if (main_action_)
			{
				scene_->setMenuHelp(main_action_, "scene.html#identify_atoms");
				scene_->setIcon(main_action_, "actions/select-rectangular", false);
				main_action_->setCheckable(true);
				connect(main_action_, SIGNAL(triggered()), SLOT(modeChangeSlot_()));
			}
		}

		void PickingMode::mouseDoubleClickEvent(QMouseEvent* evt)
		{
			QPoint p = scene_->mapFromGlobal(evt->pos());
			pickParent_(p);
		}

		void PickingMode::mouseMoveEventImpl_(QMouseEvent* evt)
		{
			if (evt->buttons() & (Qt::LeftButton | Qt::RightButton))
			{
				selectionPressedMoved_();
			}
		}

		void PickingMode::mousePressEventImpl_(QMouseEvent* evt)
		{
			std::cout << "Starting pick" << std::endl;
			if (evt->modifiers() == Qt::NoModifier && (evt->buttons() & (Qt::LeftButton | Qt::RightButton)))
			{
				pick_select_ = (evt->buttons() == Qt::LeftButton);
				mouse_pos_pick_ = mouse_pos_new_;
			}
			else
			{
				pickParent_(evt->pos());
			}

		}

		void PickingMode::mouseReleaseEventImpl_(QMouseEvent* /*evt*/)
		{
			if (ignore_pick_)
			{
				ignore_pick_ = false;

				return;
			}

			selectObjects_();
		}

		Qt::Key PickingMode::getKey() const
		{
			return Qt::Key_Q;
		}

		void PickingMode::activate()
		{
			InteractionMode::activate();

			scene_->enterPickingMode();
			scene_->setCursor(QCursor(Qt::CrossCursor));
		}

		void PickingMode::deactivate()
		{
			InteractionMode::deactivate();

			scene_->exitPickingMode();
		}

		void PickingMode::selectionPressedMoved_()
		{
			Position x0, x1, y0, y1;

			x0 = BALL_MIN(mouse_pos_new_.x(), mouse_pos_pick_.x());
			x1 = BALL_MAX(mouse_pos_new_.x(), mouse_pos_pick_.x());
			y0 = BALL_MIN(mouse_pos_new_.y(), mouse_pos_pick_.y());
			y1 = BALL_MAX(mouse_pos_new_.y(), mouse_pos_pick_.y());

			QPoint p0 = scene_->mapFromGlobal(QPoint(x0, y0));
			QPoint p1 = scene_->mapFromGlobal(QPoint(x1, y1));

			scene_->setRubberBandGeometry(QRect(p0, p1));
			scene_->setRubberBandVisible(true);
		}

		void PickingMode::pickParent_(const QPoint& p)
		{
			ignore_pick_ = true;
			list<GeometricObject*> objects;
			scene_->pickObjects(p, objects);

			if (objects.empty()) return;

			Composite* composite = (Composite*)(**objects.begin()).getComposite();
			if (composite == 0) return;

			Composite* to_select = 0;
			Atom* atom = dynamic_cast<Atom*>(composite);
			if (atom != 0)
			{
				to_select = atom->getParent();
			}
			else
			{
				Bond* bond = dynamic_cast<Bond*>(composite);
				if (bond!= 0)
				{
					to_select = (Composite*)bond->getFirstAtom()->getParent();
				}
				else
				{
					to_select = composite;
				}
			}

			if (to_select != 0)
			{
				if (to_select->isSelected())
				{
					getMainControl()->deselectCompositeRecursive(to_select, true);
				}
				else
				{
					getMainControl()->selectCompositeRecursive(to_select, true);
				}
				getMainControl()->update(*to_select, true);
			}
		}

		void PickingMode::selectObjects_()
		{
			scene_->setRubberBandVisible(false);

			QPoint p0 = scene_->mapFromGlobal(mouse_pos_pick_);
			QPoint p1 = scene_->mapFromGlobal(mouse_pos_new_);

			list<GeometricObject*> objects;

			// draw the representations
			scene_->pickObjects(p0, p1, objects);

			// sent collected objects
			GeometricObjectSelectionMessage* message = new GeometricObjectSelectionMessage;
			message->setSelection(objects);
			message->setSelected(pick_select_);
			scene_->notify(message);
		}

	}
}
