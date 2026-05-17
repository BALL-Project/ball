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

#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtWidgets/QWidget>

namespace BALL
{
	using namespace BALL::VIEW;

#ifdef BALL_UI_V2
	namespace VIEW
	{
		class InspectorDock;
		class SelectionInspectorAdapter;
		class RepresentationInspectorAdapter;
		class WelcomeScreen;  // Phase 999.47 §7.1
	}
#endif

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

		/// Phase 999.44 Plan 02: open a legacy Preferences stack page
		/// by its setWidgetStackName(...) string. Handler for the
		/// Tools › Legacy Settings ▸ <name> submenu entries.
		void openLegacySetting(const QString& stackName);

		/// Phase 999.44 Plan 02: show the first-run BALLView Refresh
		/// migration notice (Display/Model/Material moved → Inspector).
		/// One-shot, gated by [Inspector] firstRunMigrationNoticeShown.
		void showInspectorMigrationNoticeIfNeeded_();

		// --- Phase 999.47 (Handover §7) Onboarding ---

		/// Show the WelcomeScreen panel (swap centralWidget). No-op when
		/// already showing or when a composite is loaded.
		void showWelcomeScreen_();

		/// Hide the WelcomeScreen panel (swap centralWidget back to
		/// the Scene). No-op when WelcomeScreen not currently mounted.
		void hideWelcomeScreen_();

		/// Append a path to the WelcomeScreen recent-files list +
		/// persist via QSettings. Called from openFileTriggered_().
		void rememberRecentFile_(const QString& path);

		/// Read recent-files from QSettings and push to the
		/// WelcomeScreen widget. Called once at construction.
		void loadRecentFiles_();

		/// Compare BALL_VERSION to QSettings[/Onboarding/lastVersion];
		/// if different, surface the What's-new card and persist the
		/// new value. Idempotent within one launch.
		void maybeShowWhatsNew_();

		/// Mount the in-app `ballview://` URL handler so help-doc
		/// "Try this →" links route to the CommandRegistry. Called
		/// from the constructor.
		void installBallviewUrlHandler_();

	private Q_SLOTS:

		void onWelcomeOpenFileRequested_();
		void onWelcomeOpenFromPdbRequested_();
		void onWelcomeRecentFileRequested_(const QString& path);
		void onWelcomeSampleRequested_(const QString& absolutePath);
		void onWelcomeSkipToggled_(bool skip);
		void onBallviewUrlInvoked_(const QUrl& url);
#endif

		protected:
			void changeEvent(QEvent* evt);
			void setupPluginHandlers_();

			Scene* scene_;
			QAction* save_project_action_;
			QAction* qload_action_, *qsave_action_;

#ifdef BALL_UI_V2
			/// Phase 999.44 Plan 02: right-rail Inspector dock + its
			/// View ▸ Hide Inspector menu action.
			VIEW::InspectorDock*              inspector_dock_;
			QAction*                          hide_inspector_action_;
			/// Phase 999.44 Plan 03: selection-bus adapter that
			/// forwards ControlSelectionMessage / NewSelectionMessage
			/// into the Inspector Selection tab.
			VIEW::SelectionInspectorAdapter*  selection_adapter_;
			/// Phase 999.44 Plan 04: representation adapter that
			/// listens for RepresentationMessage and updates the
			/// Inspector Representation tab.
			VIEW::RepresentationInspectorAdapter* representation_adapter_;
			/// Phase 999.47 §7.1 — WelcomeScreen panel mounted as the
			/// centralWidget when no composite is open.
			VIEW::WelcomeScreen*              welcome_screen_;
			/// Phase 999.47 — running list of recent-files paths,
			/// persisted via QSettings[/Onboarding/recentFiles].
			QStringList                       recent_files_;
			/// Phase 999.47 — true after maybeShowWhatsNew_() runs,
			/// to keep the card from re-showing mid-session.
			bool                              whats_new_shown_this_launch_;
#endif
	};

} // namespace BALL

#endif // BALL_APPLICATIONS_BALLVIEW_MAINFRAME_H
