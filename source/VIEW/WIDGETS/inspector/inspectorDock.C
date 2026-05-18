// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorDock implementation.
//

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
			// v1.7.0-rc2 UFG-04 — minimum width that fits the full
			// "Selection / Representation / Scene" tab strip without
			// eliding. 280 px is the empirical width that fits all three
			// labels at the default font on the 3 reference platforms
			// (macOS 15 Apple Silicon, Ubuntu 24.04 GNOME, Windows 11)
			// plus the chevron + scrollbar gutter. Users can still drag
			// the dock narrower — at which point the InspectorTabs scroll
			// buttons (introduced alongside this fix) take over from the
			// rc1 elide-on-narrow behaviour.
			//
			// Deliberately NOT macOS-only: rc1 reports identical truncation
			// on the other 2 platforms (UFG-04 was filed against the macOS
			// screenshot but the elide is a layout bug, not a per-OS one).
			setMinimumWidth(280);

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

