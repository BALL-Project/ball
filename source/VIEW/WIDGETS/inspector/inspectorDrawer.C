// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.75 — Collapsible drawers: InspectorDrawer implementation.
//
// Horizontal mirror of BottomDrawer (see source/VIEW/WIDGETS/bottomDrawer.C):
// animate "maximumWidth" instead of "maximumHeight", nudge the parent
// QMainWindow with resizeDocks(..., Qt::Horizontal) instead of Vertical,
// and use chevron-left/chevron-right instead of chevron-up/chevron-down.
//

#include <BALL/VIEW/WIDGETS/inspector/inspectorDrawer.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorView.h>
#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>

#include <QtCore/QPropertyAnimation>
#include <QtCore/QEasingCurve>
#include <QtCore/Qt>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

#include <algorithm>

namespace BALL
{
	namespace VIEW
	{
		const int InspectorDrawer::kCollapsedWidth;
		const int InspectorDrawer::kExpandedWidth;
		const int InspectorDrawer::kAnimationDurationMs;

		InspectorDrawer::InspectorDrawer(QWidget* parent, const QString& title)
			: QDockWidget(title, parent),
			  view_(nullptr),
			  rail_(nullptr),
			  chevron_(nullptr),
			  width_animation_(nullptr),
			  expanded_(true),
			  expanded_width_(kExpandedWidth)
		{
			// objectName is DELIBERATELY the legacy "inspectorDock" (not
			// "inspectorDrawer"): WorkspaceManager presets address the
			// Inspector dock via findChild<QDockWidget*>("inspectorDock").
			// Keeping the legacy name preserves preset compatibility (UFG /
			// T-999.75-02).
			setObjectName("inspectorDock");

			// Opaque-paint contract — mirror BottomDrawer exactly. This dock
			// animates its own maximumWidth (width_animation_ below), so it
			// must paint opaquely or the parent shows through during the
			// slide. We use the SAFE half for a styled QDockWidget
			// (autoFillBackground + the opaqueContainer theme token) and
			// deliberately NOT WA_OpaquePaintEvent, which would suppress the
			// dock's own frame paint.
			setAutoFillBackground(true);
			setProperty("opaqueContainer", true);

			setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
			setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

			// Build the custom rail (collapsed strip) and install it as the
			// title bar. This REPLACES InspectorDock's macOS-only
			// setTitleBarWidget(new QWidget) — the custom rail works on all
			// platforms, the same way BottomDrawer's custom title bar solved
			// the macOS no-chrome problem.
			buildRail();
			setTitleBarWidget(rail_);

			view_ = new InspectorView(this);
			setWidget(view_);
			// loadState() must run BEFORE reading isDrawerExpanded() so the
			// persisted [Inspector] expanded= value is available.
			view_->loadState();

			width_animation_ = new QPropertyAnimation(this, "maximumWidth", this);
			width_animation_->setDuration(kAnimationDurationMs);
			width_animation_->setEasingCurve(QEasingCurve::OutCubic);
			// Lock minimumWidth to the animated maximumWidth on every tick so
			// the QMainWindow dock AREA is forced to follow the tween: a docked
			// widget with min==max must be allocated exactly that width.
			// Animating maximumWidth alone let the dock area stay pinned at the
			// collapsed minimum, which is why a re-expand previously came back
			// narrower than before.
			connect(width_animation_, &QPropertyAnimation::valueChanged, this,
			        [this](const QVariant& value) { setMinimumWidth(value.toInt()); });
			connect(width_animation_, &QPropertyAnimation::finished, this, [this]() {
				// After expanding, lift the maximumWidth cap so the user can
				// drag-resize wider. After collapsing, pin both min and max to
				// kCollapsedWidth so the dock stays the rail width.
				if (expanded_)
				{
					// Relax the locked width so the user can drag-resize again,
					// but keep the Inspector usable (>= kExpandedWidth, matching
					// the old InspectorDock UFG-04 minimum). Then re-assert the
					// restored width authoritatively now the cap is lifted —
					// QMainWindow won't shrink it because expanded_width_ >=
					// kExpandedWidth == the new minimum.
					setMaximumWidth(QWIDGETSIZE_MAX);
					setMinimumWidth(kExpandedWidth);
					view_->setVisible(true);
					if (QMainWindow* mw = qobject_cast<QMainWindow*>(parentWidget()))
					{
						mw->resizeDocks({this}, {expanded_width_}, Qt::Horizontal);
					}
				}
				else
				{
					setMaximumWidth(kCollapsedWidth);
					setMinimumWidth(kCollapsedWidth);
					view_->setVisible(false);
					if (QMainWindow* mw = qobject_cast<QMainWindow*>(parentWidget()))
					{
						mw->resizeDocks({this}, {kCollapsedWidth}, Qt::Horizontal);
					}
				}
				Q_EMIT expandedChanged(expanded_);
			});

			// Adopt the persisted state. Default true (Inspector expanded by
			// default, matching today's InspectorDock behavior).
			expanded_ = view_->isDrawerExpanded();
			if (expanded_)
			{
				chevron_->setIcon(VIEW::Icons::get("actions/chevron-right"));
				view_->setVisible(true);
				setMinimumWidth(kExpandedWidth);
			}
			else
			{
				chevron_->setIcon(VIEW::Icons::get("actions/chevron-left"));
				view_->setVisible(false);
				setMinimumWidth(kCollapsedWidth);
				setMaximumWidth(kCollapsedWidth);
			}
		}

		InspectorDrawer::~InspectorDrawer() = default;

		void InspectorDrawer::buildRail()
		{
			rail_ = new QWidget(this);
			rail_->setObjectName("inspectorDrawerRail");
			rail_->setFixedWidth(kCollapsedWidth);

			QVBoxLayout* layout = new QVBoxLayout(rail_);
			layout->setContentsMargins(0, 8, 0, 8);
			layout->setSpacing(6);

			chevron_ = new QToolButton(rail_);
			chevron_->setAutoRaise(true);
			chevron_->setObjectName("inspectorDrawerChevron");
			// Expanded → chevron-right means "click to collapse outward (to the
			// right edge)"; collapsed → chevron-left means "click to expand
			// inward". The constructor sets the correct initial icon after the
			// persisted state is known.
			chevron_->setIcon(VIEW::Icons::get("actions/chevron-right"));
			chevron_->setFixedSize(20, 20);
			// The chevron carries the Inspector identity (tooltip + accessible
			// name) so the rail needs no extra text marker — an explicit "I"
			// label below the chevron read as a stray glyph in the expanded
			// title bar (999.75 UAT), so it was removed.
			chevron_->setToolTip(tr("Inspector"));
			chevron_->setAccessibleName(tr("Inspector"));
			connect(chevron_, &QToolButton::clicked, this, &InspectorDrawer::toggle);
			layout->addWidget(chevron_, 0, Qt::AlignHCenter);
			layout->addStretch(1);
		}

		void InspectorDrawer::setExpanded(bool expanded)
		{
			if (expanded_ == expanded) return;
			expanded_ = expanded;

			if (expanded_)
			{
				// chevron-right (about to collapse outward) when expanded.
				chevron_->setIcon(VIEW::Icons::get("actions/chevron-right"));
				// Make sure the body is visible BEFORE the animation so we
				// don't see an empty cavity during expansion.
				view_->setVisible(true);
				// Drop the minimumWidth floor so the animation can start from
				// the collapsed rail. animateTo() raises maximumWidth to make
				// room and nudges the parent QMainWindow's dock layout. Restore
				// the width the user last saw (expanded_width_) rather than the
				// fixed kExpandedWidth, so re-opening keeps the prior size.
				setMinimumWidth(kCollapsedWidth);
				animateTo(expanded_width_);
			}
			else
			{
				// chevron-left (about to expand inward) when collapsed.
				// Capture the current expanded width BEFORE shrinking so a
				// later re-expand restores it (floor at kExpandedWidth so a
				// stray sub-minimum value can't make the drawer shrink over
				// repeated toggles).
				expanded_width_ = std::max(width(), kExpandedWidth);
				chevron_->setIcon(VIEW::Icons::get("actions/chevron-left"));
				setMinimumWidth(kCollapsedWidth);
				animateTo(kCollapsedWidth);
			}

			// Persist the new state via the InspectorView [Inspector] INI
			// plumbing (debounced; no-ops while loading_).
			view_->setDrawerExpanded(expanded_);
		}

		void InspectorDrawer::toggle()
		{
			setExpanded(!expanded_);
		}

		void InspectorDrawer::animateTo(int targetWidth)
		{
			width_animation_->stop();
			// Drive maximumWidth from the current rendered width (what the
			// user actually sees) to the target — mirrors BottomDrawer's
			// height-based reasoning on the horizontal axis.
			const int currentWidth = width();
			// Raise the cap so the animation has room to grow into: cap >=
			// targetWidth so the endValue takes effect, and cap >=
			// currentWidth so we don't clamp the current visible value.
			setMaximumWidth(std::max(currentWidth, targetWidth));
			width_animation_->setStartValue(currentWidth);
			width_animation_->setEndValue(targetWidth);
			width_animation_->start();
			// NOTE: the dock-area width is driven by the min==max lock wired in
			// the constructor (each tick forces the QMainWindow allocation) plus
			// the authoritative resizeDocks() in the animation's finished
			// handler. A resizeDocks() issued HERE fired while the animation had
			// momentarily reset maximumWidth to the collapsed start value, which
			// clamped the target and left the drawer narrower on re-open — so it
			// was removed.
		}

	} // namespace VIEW
} // namespace BALL
