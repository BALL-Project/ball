// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H
#define BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H

#ifndef BALL_VIEW_KERNEL_MAINCONTROL_H
# include <BALL/VIEW/KERNEL/mainControl.h>
#endif

#ifndef BALL_VIEW_WIDGETS_SCENE_H
# include <BALL/VIEW/WIDGETS/scene.h>
#endif

#include <QtWidgets/QWidget>

namespace BALL
{
	using namespace BALL::VIEW;

	class Mainframe	
		: public BALL::VIEW::MainControl
	{
		Q_OBJECT

		public:

		///
		BALL_EMBEDDABLE(Mainframe, MainControl)

		///
		Mainframe(QWidget* parent = 0, const char* name = 0);

		///
		virtual ~Mainframe();

		public Q_SLOTS:

		///
		void show();

		/// Catch key events
		bool eventFilter(QObject*, QEvent* e);
			
		/// remove all loaded Molecules and Representations, reset Coloring options
		void reset();

		// Help menu
		void about();

		void checkMenus();

#ifdef BALL_UI_V2
		/// Phase 999.43 dev-only: open the IconBrowser dialog. Only
		/// wired into the Tools menu when BALL_UI_V2 is ON AND the
		/// build is a debug build (!NDEBUG). See iconBrowser.h.
		void openIconBrowser();
#endif

		protected:
			void changeEvent(QEvent* evt);
			void setupPluginHandlers_();

			Scene* scene_;
			QAction* save_project_action_;
			QAction* qload_action_, *qsave_action_;
	};

} // namespace BALL

#endif // BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H
