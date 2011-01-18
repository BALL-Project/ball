#ifndef BALL_VIEW_KERNEL_MODES_ROTATEMODE_H
#define BALL_VIEW_KERNEL_MODES_ROTATEMODE_H

#include <BALL/VIEW/KERNEL/MODES/interactionMode.h>

namespace BALL
{
	namespace VIEW
	{

		class RotateMode : public InteractionMode
		{
			public:
				RotateMode(Scene* scene);

				virtual void mouseDoubleClickEvent(QMouseEvent* evt);

				virtual Qt::Key getKey() const;
				virtual QString getName() const { return "RotateMode"; }

				virtual void activate();

			protected:
				virtual void mouseMoveEventImpl_(QMouseEvent* evt);
				virtual void mouseReleaseEventImpl_(QMouseEvent* evt);

				void rotateSystemClockwise_();
				void processRotateModeMouseEvents_(QMouseEvent* evt);
				void rotateSystem_();
				void translateSystem_();
		};

	}
}

#endif //BALL_VIEW_KERNEL_MODES_ROTATEMODE_H
