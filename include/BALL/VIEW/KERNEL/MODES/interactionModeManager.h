#ifndef BALL_VIEW_KERNEL_MODES_MODEMANAGER_H
#define BALL_VIEW_KERNEL_MODES_MODEMANAGER_H

#ifndef BALL_VIEW_RENDERING_RENDERERS_RENDERER_H
	#include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>
#endif

#include <list>

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
				virtual ~InteractionModeManager();

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
				Renderer::StereoMode stereo_mode_;
				InteractionMode* current_mode_;
				InteractionMode* last_mode_;
				QActionGroup* actions_;
				std::list<InteractionMode*> modes_;
				Scene* scene_;
				bool need_update_;

				void init_();
				bool handleStereoKeyPress_(QKeyEvent* evt);
		};
	}
}

#endif //BALL_VIEW_KERNEL_MODES_MODEMANAGER_H
