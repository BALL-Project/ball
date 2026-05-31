// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/bottomDrawer.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/WIDGETS/fileObserver.h>
#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>

// Phase 999.75 DRAWER-02 — PyBALL tab slot (v2.1 PyBALL revival).
// BALL_PYTHON_SUPPORT is /* #undef */ in every current build's config.h
// (SIP 4.9 is unavailable), so this slot is INERT in normal builds. The
// macro reaches here transitively via bottomDrawer.h → BALL/COMMON/global.h
// → BALL/CONFIG/config.h. Every PyWidget / pyWidget.h / Python reference
// MUST stay inside this guard so no Python symbols compile or link when the
// flag is off. Do NOT enable Python/SIP here — that is parked for v2.1.
#ifdef BALL_PYTHON_SUPPORT
# include <BALL/VIEW/WIDGETS/pyWidget.h>
#endif

#include <QtCore/QPropertyAnimation>
#include <QtCore/QEasingCurve>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSizePolicy>

#include <algorithm>

namespace BALL
{
	namespace VIEW
	{
		const int BottomDrawer::kCollapsedHeight;
		const int BottomDrawer::kExpandedHeight;
		const int BottomDrawer::kAnimationDurationMs;

		BottomDrawer::BottomDrawer(LogView* logView, FileObserver* fileObserver, QWidget* parent)
			: QDockWidget(tr("Logs"), parent),
			  log_view_(logView),
			  file_observer_(fileObserver),
			  title_bar_(nullptr),
			  chevron_(nullptr),
			  summary_label_(nullptr),
			  body_(nullptr),
			  tab_bar_(nullptr),
			  stack_(nullptr),
			  height_animation_(nullptr),
			  expanded_(false)
		{
			setObjectName("bottomDrawer");
			// Opaque-paint contract (ARCHITECTURE-CONTRACT.md §10 / THEME.md):
			// this dock animates its own maximumHeight (height_animation_ below),
			// so it must paint opaquely or the parent shows through during the
			// slide (the UFG-05/09/10 flicker class). We use the SAFE half for a
			// styled QDockWidget — autoFillBackground + the opaqueContainer theme
			// token (themed fill via theme-neutral.qss) — and deliberately NOT
			// WA_OpaquePaintEvent, which would suppress the dock's own frame paint.
			setAutoFillBackground(true);
			setProperty("opaqueContainer", true);
			// Bottom drawer should be allowed only in the bottom area.
			setAllowedAreas(Qt::BottomDockWidgetArea);
			// We hide the standard title bar and use a custom one (the
			// collapsed strip becomes the title bar).
			setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

			buildTitleBar();
			buildBody();

			// Compose: title bar at top, body below. The body is the
			// QDockWidget's main widget; title is the title-bar widget.
			setTitleBarWidget(title_bar_);
			setWidget(body_);

			// Start collapsed.
			body_->setVisible(false);
			setMinimumHeight(kCollapsedHeight);
			setMaximumHeight(kCollapsedHeight);

			height_animation_ = new QPropertyAnimation(this, "maximumHeight", this);
			height_animation_->setDuration(kAnimationDurationMs);
			height_animation_->setEasingCurve(QEasingCurve::OutCubic);
			connect(height_animation_, &QPropertyAnimation::finished, this, [this]() {
				// After expanding, lift the maximumHeight cap so the user
				// can drag-resize larger if they want. After collapsing,
				// pin both min and max to kCollapsedHeight so the dock
				// stays the strip size.
				if (expanded_)
				{
					setMaximumHeight(QWIDGETSIZE_MAX);
					body_->setVisible(true);
				}
				else
				{
					setMaximumHeight(kCollapsedHeight);
					setMinimumHeight(kCollapsedHeight);
					body_->setVisible(false);
				}
				Q_EMIT expandedChanged(expanded_);
			});
		}

		BottomDrawer::~BottomDrawer() = default;

		void BottomDrawer::buildTitleBar()
		{
			title_bar_ = new QWidget(this);
			title_bar_->setObjectName("bottomDrawerTitleBar");
			title_bar_->setFixedHeight(kCollapsedHeight);

			QHBoxLayout* layout = new QHBoxLayout(title_bar_);
			layout->setContentsMargins(8, 0, 8, 0);
			layout->setSpacing(6);

			chevron_ = new QToolButton(title_bar_);
			chevron_->setAutoRaise(true);
			chevron_->setObjectName("bottomDrawerChevron");
			// Collapsed → chevron-up means "click to expand upward (more space)"
			chevron_->setIcon(VIEW::Icons::get("actions/chevron-up"));
			chevron_->setFixedSize(20, 20);
			connect(chevron_, &QToolButton::clicked, this, &BottomDrawer::toggle);
			layout->addWidget(chevron_);

			summary_label_ = new QLabel(tr("Logs"), title_bar_);
			summary_label_->setObjectName("bottomDrawerSummary");
			layout->addWidget(summary_label_);
			layout->addStretch(1);

			// Clicking anywhere on the strip toggles too — but we keep
			// that simple: only the chevron triggers toggle for now.
		}

		void BottomDrawer::buildBody()
		{
			body_ = new QWidget(this);
			body_->setObjectName("bottomDrawerBody");
			QVBoxLayout* layout = new QVBoxLayout(body_);
			layout->setContentsMargins(0, 0, 0, 0);
			layout->setSpacing(0);

			tab_bar_ = new QTabBar(body_);
			tab_bar_->setObjectName("bottomDrawerTabs");
			tab_bar_->setDocumentMode(true);

			stack_ = new QStackedWidget(body_);
			stack_->setObjectName("bottomDrawerStack");

			// Phase 999.75 DRAWER-02 — all tabs are now registered through the
			// single addDrawerTab_ helper instead of two duplicated blocks.
			// LogView and FileObserver inherit GenericControl → DockWidget →
			// QDockWidget. Reparenting them into a QStackedWidget would strip
			// their dock chrome but keep their inner widgets. We use their
			// QDockWidget::widget() inner content directly so we don't trash
			// the message-bus parent chain (the UFG-17 double-attach fix lives
			// inside addDrawerTab_: hide the outer dock only when the inner
			// widget was actually added).
			addDrawerTab_(tr("Log"),
			              (log_view_ != nullptr) ? log_view_->widget() : nullptr,
			              log_view_);
			addDrawerTab_(tr("Files"),
			              (file_observer_ != nullptr) ? file_observer_->widget() : nullptr,
			              file_observer_);

#ifdef BALL_PYTHON_SUPPORT
			// Phase 999.75 DRAWER-02 — PyBALL tab (third tab). Layout-ready for
			// the v2.1 PyBALL revival; ENTIRELY compile-time guarded so nothing
			// Python is compiled or linked when BALL_PYTHON_SUPPORT is OFF (the
			// normal case — SIP 4.9 is unavailable). PyWidget inherits
			// DockWidget like LogView/FileObserver, so we register its inner
			// widget() through the same addDrawerTab_ path and hide the outer
			// dock (UFG-17). Constructed as a child of this drawer.
			PyWidget* py_widget = new PyWidget(this);
			addDrawerTab_(tr("Python"),
			              (py_widget != nullptr) ? py_widget->widget() : nullptr,
			              py_widget);
#endif

			connect(tab_bar_, &QTabBar::currentChanged, stack_, &QStackedWidget::setCurrentIndex);

			layout->addWidget(tab_bar_);
			layout->addWidget(stack_, 1);
		}

		void BottomDrawer::addDrawerTab_(const QString& label, QWidget* content, QDockWidget* outerToHide)
		{
			tab_bar_->addTab(label);
			if (content != nullptr)
			{
				stack_->addWidget(content);
				// UFG-17 double-attach fix: hide the original outer dock so we
				// don't see two of it — but ONLY on the real path (when the
				// inner widget was actually added). A null content keeps the
				// outer dock visible (matching the original per-tab code).
				if (outerToHide != nullptr)
				{
					outerToHide->hide();
				}
			}
			else
			{
				QLabel* placeholder = new QLabel(tr("(unavailable)"), stack_);
				stack_->addWidget(placeholder);
			}
		}

		void BottomDrawer::setExpanded(bool expanded)
		{
			if (expanded_ == expanded) return;
			expanded_ = expanded;

			if (expanded_)
			{
				// chevron-down (about to collapse) when expanded; flip now.
				chevron_->setIcon(VIEW::Icons::get("actions/chevron-down"));
				// Make sure body is visible BEFORE the animation so we
				// don't see an empty cavity during expansion.
				body_->setVisible(true);
				// Drop the minimumHeight floor to kCollapsedHeight so the
				// animation can start from the 24 px collapsed strip.
				// animateTo() handles raising maximumHeight to make room
				// for the animation to grow into, and we explicitly nudge
				// the parent QMainWindow's dock-area layout so the bottom
				// row actually grows past 24 px (animating maximumHeight
				// alone is not enough for QDockWidget in a QMainWindow).
				setMinimumHeight(kCollapsedHeight);
				animateTo(kExpandedHeight);
			}
			else
			{
				chevron_->setIcon(VIEW::Icons::get("actions/chevron-up"));
				// Drop the floor first so the animation can shrink past
				// the prior expanded minimumHeight (if any).
				setMinimumHeight(kCollapsedHeight);
				animateTo(kCollapsedHeight);
			}
		}

		void BottomDrawer::toggle()
		{
			setExpanded(!expanded_);
		}

		void BottomDrawer::animateTo(int targetHeight)
		{
			height_animation_->stop();
			// Drive maximumHeight from the current rendered height (what
			// the user actually sees) to the target. Using
			// maximumHeight() as the start would give a 240->240 no-op
			// after the previous animation's finished slot lifted the
			// cap to QWIDGETSIZE_MAX, or a 24->24 no-op the first time
			// (because the constructor pins both min and max to
			// kCollapsedHeight). The visible-height delta is the only
			// thing that translates to motion the user perceives.
			const int currentHeight = height();
			// Raise the cap so the animation has room to grow into. We
			// need cap >= targetHeight for the animation's endValue to
			// take effect, and cap >= currentHeight so we don't clamp
			// the current visible value on the way up.
			setMaximumHeight(std::max(currentHeight, targetHeight));
			height_animation_->setStartValue(currentHeight);
			height_animation_->setEndValue(targetHeight);
			height_animation_->start();

			// Force the parent QMainWindow's dock-area layout to allocate
			// the new row size. Animating maximumHeight on a QDockWidget
			// is not enough — QMainWindow's dock layout caches the
			// previous row geometry and will not re-grow it without an
			// explicit resizeDocks() hint. Without this, the chevron
			// click flips expanded_ and the body is shown, but the dock
			// row stays at 24 px and the body has zero visible pixels.
			if (QMainWindow* mw = qobject_cast<QMainWindow*>(parentWidget()))
			{
				mw->resizeDocks({this}, {targetHeight}, Qt::Vertical);
			}
		}

	} // namespace VIEW
} // namespace BALL
