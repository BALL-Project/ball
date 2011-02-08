#ifndef BALL_VIEW_KERNEL_MODES_INTERACTIONMODE_H
#define BALL_VIEW_KERNEL_MODES_INTERACTIONMODE_H

#include <BALL/COMMON/global.h>

#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtCore/QObject>

class QAction;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
class QToolBar;
class QMenu;

namespace BALL
{
	namespace VIEW
	{
		class Scene;
		class MainControl;

		class BALL_VIEW_EXPORT InteractionMode : public QObject
		{
			Q_OBJECT

			public:
				InteractionMode(Scene* scene);

				virtual ~InteractionMode();
				virtual void keyPressEvent(QKeyEvent* evt);
				virtual void keyReleaseEvent(QKeyEvent* evt);

				virtual void mouseDoubleClickEvent(QMouseEvent* evt);
				void mouseMoveEvent(QMouseEvent* evt);
				void mousePressEvent(QMouseEvent* evt);
				void mouseReleaseEvent(QMouseEvent* evt);

				virtual void wheelEvent(QWheelEvent* evt);

				virtual void addToolBarEntries(QToolBar* tb);
				virtual void populateContextMenu(QMenu*) {};

				virtual Qt::Key getKey() const = 0;
				virtual QString getName() const = 0;

				virtual void deactivate();
				virtual void activate();

				void setMouseSensitivity(float sens);

				QAction* getMainAction();

				bool isCurrent() const { return is_current_; }
				void setCurrent(bool current) { is_current_ = current; }

			signals:
				void requestModeChange(InteractionMode* mode);
			protected slots:
				void modeChangeSlot_();

			protected:
				Scene* scene_;
				QPoint mouse_pos_old_;
				QPoint mouse_pos_new_;
				QSize desktop_size_;
				float mouse_sensitivity_;
				float mouse_wheel_sensitivity_;
				QAction* main_action_;
				bool is_current_;

				virtual void mouseMoveEventImpl_   (QMouseEvent* evt);
				virtual void mousePressEventImpl_  (QMouseEvent* evt);
				virtual void mouseReleaseEventImpl_(QMouseEvent* evt);

				float getXDiff_() const;
				float getYDiff_() const;
				void zoomSystem_();
		};

	}
}

#endif //BALL_VIEW_KERNEL_MODES_INTERACTIONMODE_H
