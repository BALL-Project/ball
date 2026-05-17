// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorDock implementation.
//

#ifdef BALL_UI_V2

#include <BALL/VIEW/WIDGETS/inspector/inspectorDock.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorView.h>

#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		InspectorDock::InspectorDock(QWidget* parent, const QString& title)
			: QDockWidget(title, parent),
				view_(nullptr)
		{
			setObjectName("inspectorDock");
			setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
			setFeatures(QDockWidget::DockWidgetMovable
			          | QDockWidget::DockWidgetFloatable
			          | QDockWidget::DockWidgetClosable);

			view_ = new InspectorView(this);
			setWidget(view_);

#ifdef Q_OS_MACOS
			// macOS: hide the dock's title bar chrome so the Inspector
			// blends with the right-rail per Handover §Cross-platform
			// notes. The custom InspectorTabs strip serves as the
			// identity / drag handle.
			setTitleBarWidget(new QWidget(this));
#endif

			view_->loadState();
		}

		InspectorDock::~InspectorDock() = default;

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
