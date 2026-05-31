// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_BOTTOMDRAWER_H
#define BALL_VIEW_WIDGETS_BOTTOMDRAWER_H

// Phase 999.45 — BALLView Refresh: BottomDrawer
// ==============================================
//
// BottomDrawer is the bottom-rail "Logs/FileObserver" drawer in the
// refreshed Default workspace (Handover §5.2). Two states:
//
//   Collapsed (24 px tall): chevron-up icon + summary label
//                           ("Logs · N new"). Clicking chevron expands.
//   Expanded  (240 px tall): QTabBar at top switching between LogView
//                           and FileObserver. Chevron flips to
//                           chevron-down; clicking collapses.
//
// Animation: 200 ms ease-out on maximumHeight (Qt's QPropertyAnimation).
// Drag-resize works because QDockWidget supports it natively; we only
// override minimumHeight/maximumHeight at the two states' boundaries.
//
// objectName: "bottomDrawer" — referenced by WorkspaceManager presets.

#include <BALL/COMMON/global.h>

#include <QtWidgets/QDockWidget>

class QTabBar;
class QStackedWidget;
class QToolButton;
class QLabel;
class QWidget;
class QPropertyAnimation;

namespace BALL
{
	namespace VIEW
	{
		class LogView;
		class FileObserver;

		/**
		 *  Animated bottom drawer hosting Logs + FileObserver tabs.
		 *
		 *  Constructed by Mainframe under BALL_UI_V2 and registered as
		 *  a bottom-area QDockWidget.
		 */
		class BALL_VIEW_EXPORT BottomDrawer : public QDockWidget
		{
			Q_OBJECT

			public:
				/**
				 *  @param logView      reused LogView modular widget (kept by ptr; not owned)
				 *  @param fileObserver reused FileObserver modular widget (kept by ptr; not owned)
				 *  @param parent       owning QMainWindow (typically Mainframe)
				 */
				explicit BottomDrawer(LogView* logView,
				                      FileObserver* fileObserver,
				                      QWidget* parent = nullptr);

				~BottomDrawer() override;

				/// True when the drawer is in its expanded (240 px) state.
				bool isExpanded() const { return expanded_; }

				/// Collapsed-state height in pixels.
				static const int kCollapsedHeight = 24;

				/// Expanded-state height in pixels.
				static const int kExpandedHeight = 240;

				/// Animation duration in milliseconds.
				static const int kAnimationDurationMs = 200;

			public Q_SLOTS:
				/// Programmatically expand/collapse with animation.
				void setExpanded(bool expanded);

				/// Toggle current state.
				void toggle();

			Q_SIGNALS:
				/// Emitted exactly once per state transition (after animation completes).
				void expandedChanged(bool expanded);

			private:
				void buildTitleBar();
				void buildBody();
				void animateTo(int targetHeight);

				/**
				 * Phase 999.75 DRAWER-02 — single tab-registration path.
				 * Adds one tab to tab_bar_ + stack_:
				 *  - @p label    the QTabBar tab text.
				 *  - @p content  the inner widget to host in the QStackedWidget
				 *                (e.g. log_view_->widget()). If null, a
				 *                "(unavailable)" placeholder QLabel is added
				 *                instead and @p outerToHide is left visible.
				 *  - @p outerToHide  the outer dock (e.g. log_view_) whose chrome
				 *                we hide ONLY when @p content was actually added
				 *                — this is the UFG-17 double-attach fix
				 *                (reparent the inner widget + hide the outer dock
				 *                so we don't see two of it). May be null.
				 */
				void addDrawerTab_(const QString& label, QWidget* content, QDockWidget* outerToHide);

				LogView* log_view_;
				FileObserver* file_observer_;

				// Title bar (always visible — serves as the 24 px collapsed
				// strip when body is hidden).
				QWidget* title_bar_;
				QToolButton* chevron_;
				QLabel* summary_label_;

				// Body content (hidden when collapsed).
				QWidget* body_;
				QTabBar* tab_bar_;
				QStackedWidget* stack_;

				QPropertyAnimation* height_animation_;

				bool expanded_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_BOTTOMDRAWER_H
