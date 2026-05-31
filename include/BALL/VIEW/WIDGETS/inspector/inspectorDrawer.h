// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORDRAWER_H
#define BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORDRAWER_H

// Phase 999.75 — Collapsible drawers: InspectorDrawer (DRAWER-01)
// ================================================================
//
// InspectorDrawer is the right-edge collapsible host for the Unified
// Inspector. It is the HORIZONTAL analog of BottomDrawer (the bottom
// "Logs/Files" drawer): where BottomDrawer animates "maximumHeight"
// between a 24px collapsed strip and a 240px expanded body, the
// InspectorDrawer animates "maximumWidth" between a ~24px collapsed
// rail and the 280px expanded Inspector. Two states:
//
//   Collapsed (24 px wide): a thin vertical rail on the RIGHT edge
//                           with a chevron-left icon ("click to expand
//                           inward") + a vertical "Inspector" label.
//   Expanded  (280 px wide): the full InspectorView (Selection /
//                           Representation / Scene tabs). The chevron
//                           flips to chevron-right ("click to collapse
//                           outward").
//
// Animation: 200 ms ease-out on maximumWidth (QPropertyAnimation), with
// an explicit QMainWindow::resizeDocks(..., Qt::Horizontal) nudge — the
// same technique BottomDrawer uses on the vertical axis.
//
// objectName: "inspectorDock" (NOT "inspectorDrawer") — preserved so
// WorkspaceManager presets keep addressing the Inspector dock by its
// legacy name via findChild<QDockWidget*>("inspectorDock").
//
// NOTE (de-duplication deferred): InspectorDrawer deliberately MIRRORS
// BottomDrawer rather than extracting a shared CollapsibleDrawer base.
// This is the lowest-risk path — it cannot regress the already-shipped
// BottomDrawer. A future cleanup could factor the common collapse/
// animate/persist machinery into a shared base; that is intentionally
// out of scope for Phase 999.75.

#include <BALL/COMMON/global.h>

#include <QtWidgets/QDockWidget>

class QToolButton;
class QLabel;
class QWidget;
class QPropertyAnimation;

namespace BALL
{
	namespace VIEW
	{
		class InspectorView;

		/**
		 *  Animated right-edge drawer hosting the Unified Inspector.
		 *
		 *  Horizontal analog of BottomDrawer. Wraps an InspectorView and
		 *  collapses it to a ~24px chevron rail on the right edge. The
		 *  collapsed/expanded state persists across restarts via the
		 *  [Inspector] expanded= INI key (InspectorView::isDrawerExpanded /
		 *  setDrawerExpanded).
		 */
		class BALL_VIEW_EXPORT InspectorDrawer : public QDockWidget
		{
			Q_OBJECT

			public:
				/**
				 * @param parent  The mainframe (or any QMainWindow).
				 * @param title  Dock title (labels the View → docks menu entry;
				 *   the custom rail replaces the native dock chrome on all
				 *   platforms, mirroring BottomDrawer).
				 */
				InspectorDrawer(QWidget* parent, const QString& title);
				~InspectorDrawer() override;

				/// The wrapped Inspector view (REQUIRED — mainframe calls
				/// view()->attachSelectionTab(...) / attachRepresentationTab()).
				InspectorView* view() const { return view_; }

				/// True when the drawer is in its expanded (280 px) state.
				bool isExpanded() const { return expanded_; }

				/// Collapsed-state width in pixels (the rail).
				static const int kCollapsedWidth = 24;

				/// Expanded-state width in pixels (matches InspectorDock's
				/// UFG-04 setMinimumWidth(280)).
				static const int kExpandedWidth = 280;

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
				void buildRail();
				void animateTo(int targetWidth);

				InspectorView* view_;

				// Custom title-bar widget = the collapsed ~24px rail (chevron +
				// vertical identity label). Always installed via
				// setTitleBarWidget so the rail works on all platforms.
				QWidget* rail_;
				QToolButton* chevron_;
				QLabel* rail_label_;

				QPropertyAnimation* width_animation_;

				bool expanded_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORDRAWER_H
