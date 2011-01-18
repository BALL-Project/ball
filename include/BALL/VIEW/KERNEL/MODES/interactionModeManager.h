#ifndef BALL_VIEW_KERNEL_MODES_MODEMANAGER_H
#define BALL_VIEW_KERNEL_MODES_MODEMANAGER_H

#include <list>

#include <BALL/VIEW/RENDERING/RENDERERS/glRenderer.h>

#include <QtCore/QObject>

class QAction;
class QMenu;
class QActionGroup;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
class QToolBar;

namespace BALL
{
	namespace VIEW
	{
		class InteractionMode;
		class Scene;

		class InteractionModeManager : public QObject
		{
			Q_OBJECT

			public:
				InteractionModeManager(Scene* scene);
				void init();

				QString getCurrentModeName() const;

				virtual void keyPressEvent(QKeyEvent* evt);
				virtual void keyReleaseEvent(QKeyEvent* evt);

				virtual void mouseDoubleClickEvent(QMouseEvent* evt);
				virtual void mouseMoveEvent(QMouseEvent* evt);
				virtual void mousePressEvent(QMouseEvent* evt);
				virtual void mouseReleaseEvent(QMouseEvent* evt);

				virtual void wheelEvent(QWheelEvent* evt);

				virtual void addToolBarEntries(QToolBar* tb);
				virtual void populateContextMenu(QMenu* menu);
				void setStereoMode(bool stereo_mode);

				void switchToLastMode();

			protected slots:
				void setMode_(InteractionMode* new_mode);

			protected:
				GLRenderer::StereoMode stereo_mode_;
				InteractionMode* current_mode_;
				InteractionMode* last_mode_;
				QActionGroup* actions_;
				std::list<InteractionMode*> modes_;
				Scene* scene_;
				bool need_update_;

				bool handleStereoKeyPress_(QKeyEvent* evt);
		};
	}
}

#endif //BALL_VIEW_KERNEL_MODES_MODEMANAGER_H
