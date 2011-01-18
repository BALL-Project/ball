#ifndef BALL_VIEW_KERNEL_MODES_PICKINGMODE_H
#define BALL_VIEW_KERNEL_MODES_PICKINGMODE_H

#include <BALL/VIEW/KERNEL/MODES/interactionMode.h>

namespace BALL
{
	namespace VIEW
	{

		class PickingMode : public InteractionMode
		{
			public:
				PickingMode(Scene* scene);

				virtual void mouseDoubleClickEvent(QMouseEvent* evt);

				virtual Qt::Key getKey() const;
				virtual QString getName() const { return "PickingMode"; }

				virtual void activate();
				virtual void deactivate();

			protected:
				bool pick_select_;
				bool ignore_pick_;
				QPoint mouse_pos_pick_;

				void pickParent_(const QPoint& p);
				void selectionPressedMoved_();
				void selectObjects_();
				virtual void mouseMoveEventImpl_(QMouseEvent* evt);
				virtual void mousePressEventImpl_(QMouseEvent* evt);
				virtual void mouseReleaseEventImpl_(QMouseEvent* evt);
		};

	}
}

#endif //BALL_VIEW_KERNEL_MODES_PICKINGMODE_H
