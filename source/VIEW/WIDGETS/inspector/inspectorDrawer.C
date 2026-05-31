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
			  rail_label_(nullptr),
			  width_animation_(nullptr),
			  expanded_(true)
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
			connect(width_animation_, &QPropertyAnimation::finished, this, [this]() {
				// After expanding, lift the maximumWidth cap so the user can
				// drag-resize wider. After collapsing, pin both min and max to
				// kCollapsedWidth so the dock stays the rail width.
				if (expanded_)
				{
					setMaximumWidth(QWIDGETSIZE_MAX);
					view_->setVisible(true);
				}
				else
				{
					setMaximumWidth(kCollapsedWidth);
					setMinimumWidth(kCollapsedWidth);
					view_->setVisible(false);
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
			connect(chevron_, &QToolButton::clicked, this, &InspectorDrawer::toggle);
			layout->addWidget(chevron_, 0, Qt::AlignHCenter);

			// Vertical identity label so the collapsed rail is recognizable.
			// A QLabel does not rotate text natively; the narrow rail width
			// (kCollapsedWidth) keeps the visible identity to a chevron plus a
			// compact "I" marker, which reads as the Inspector rail. The
			// accessible name carries the full identity.
			rail_label_ = new QLabel(QStringLiteral("I"), rail_);
			rail_label_->setObjectName("inspectorDrawerLabel");
			rail_label_->setAccessibleName(tr("Inspector"));
			rail_label_->setToolTip(tr("Inspector"));
			rail_label_->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
			layout->addWidget(rail_label_, 0, Qt::AlignHCenter);
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
				// room and nudges the parent QMainWindow's dock layout.
				setMinimumWidth(kCollapsedWidth);
				animateTo(kExpandedWidth);
			}
			else
			{
				// chevron-left (about to expand inward) when collapsed.
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

			// Force the parent QMainWindow's dock-area layout to allocate the
			// new column width. Animating maximumWidth on a QDockWidget alone
			// is not enough — QMainWindow's dock layout caches the previous
			// column geometry and will not re-grow it without an explicit
			// resizeDocks() hint (Qt::Horizontal for a left/right dock).
			if (QMainWindow* mw = qobject_cast<QMainWindow*>(parentWidget()))
			{
				mw->resizeDocks({this}, {targetWidth}, Qt::Horizontal);
			}
		}

	} // namespace VIEW
} // namespace BALL
