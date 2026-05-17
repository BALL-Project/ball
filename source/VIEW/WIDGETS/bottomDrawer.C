// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/bottomDrawer.h>
#include <BALL/VIEW/WIDGETS/logView.h>
#include <BALL/VIEW/WIDGETS/fileObserver.h>
#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>

#include <QtCore/QPropertyAnimation>
#include <QtCore/QEasingCurve>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSizePolicy>

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
			tab_bar_->addTab(tr("Log"));
			tab_bar_->addTab(tr("Files"));

			stack_ = new QStackedWidget(body_);
			stack_->setObjectName("bottomDrawerStack");

			// LogView and FileObserver inherit GenericControl → DockWidget →
			// QDockWidget. Reparenting them into a QStackedWidget would
			// strip their dock chrome but keep their inner widgets. We use
			// their QDockWidget::widget() inner content directly so we
			// don't trash the message-bus parent chain.
			if (log_view_ != nullptr && log_view_->widget() != nullptr)
			{
				stack_->addWidget(log_view_->widget());
				// Hide the original outer dock so we don't see two of it.
				log_view_->hide();
			}
			else
			{
				QLabel* placeholder = new QLabel(tr("(log unavailable)"), stack_);
				stack_->addWidget(placeholder);
			}

			if (file_observer_ != nullptr && file_observer_->widget() != nullptr)
			{
				stack_->addWidget(file_observer_->widget());
				file_observer_->hide();
			}
			else
			{
				QLabel* placeholder = new QLabel(tr("(files unavailable)"), stack_);
				stack_->addWidget(placeholder);
			}

			connect(tab_bar_, &QTabBar::currentChanged, stack_, &QStackedWidget::setCurrentIndex);

			layout->addWidget(tab_bar_);
			layout->addWidget(stack_, 1);
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
				setMinimumHeight(kCollapsedHeight);
				// Lift the maximumHeight cap so the animation can grow
				// past kCollapsedHeight; animateTo() drives the visible
				// value.
				setMaximumHeight(kExpandedHeight);
				animateTo(kExpandedHeight);
			}
			else
			{
				chevron_->setIcon(VIEW::Icons::get("actions/chevron-up"));
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
			height_animation_->setStartValue(maximumHeight());
			height_animation_->setEndValue(targetHeight);
			height_animation_->start();
		}

	} // namespace VIEW
} // namespace BALL
