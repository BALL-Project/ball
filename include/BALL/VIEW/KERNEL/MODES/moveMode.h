#ifndef BALL_VIEW_KERNEL_MODES_MOVEMODE_H
#define BALL_VIEW_KERNEL_MODES_MOVEMODE_H

#include <BALL/VIEW/KERNEL/MODES/interactionMode.h>

namespace BALL
{
	namespace VIEW
	{

		class MoveMode : public InteractionMode
		{
			Q_OBJECT

			public:
				MoveMode(Scene* scene);

				virtual Qt::Key getKey() const;
				virtual QString getName() const { return "MoveMode"; }

				virtual void activate();
				virtual void populateContextMenu(QMenu* menu);

			protected slots:
				void moveAtomTriggered_();

			protected:
				QAction* move_atom_;
				Index getMoveModeAction_(QMouseEvent* e);
				void processMoveModeMouseEvents_(QMouseEvent* e);

				virtual void mouseMoveEventImpl_(QMouseEvent* evt);
				virtual void mouseReleaseEventImpl_(QMouseEvent* evt);
		};

	}
}

#endif //BALL_VIEW_KERNEL_MODES_PICKINGMODE_H
