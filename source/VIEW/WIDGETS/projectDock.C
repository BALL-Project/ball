// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/WIDGETS/projectDock.h>

#include <QtCore/QString>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{
		const char* ProjectDock::kProjectDockObjectName = "projectDock";

		namespace
		{
			// Replace a dock's title-bar with a zero-height widget so the
			// only visible chrome is the tabified-dock tab strip. Saving
			// state still recognizes the dock; only the visible title
			// banner is suppressed.
			void hideTitleBar(QDockWidget* dock)
			{
				if (dock == nullptr) return;
				QWidget* empty = new QWidget(dock);
				empty->setFixedHeight(0);
				dock->setTitleBarWidget(empty);
			}
		}

		void ProjectDock::tabify(QMainWindow* main,
		                         QDockWidget* structures,
		                         QDockWidget* representations,
		                         QDockWidget* datasets)
		{
			if (main == nullptr) return;
			if (structures == nullptr || representations == nullptr || datasets == nullptr) return;

			// Stable objectName-s so WorkspaceManager + saveState/restoreState
			// can address the group by name. The canonical entry point is
			// the Structures dock (== kProjectDockObjectName).
			structures->setObjectName(QString::fromUtf8(kProjectDockObjectName));
			representations->setObjectName(QString::fromUtf8("projectDock.representations"));
			datasets->setObjectName(QString::fromUtf8("projectDock.datasets"));

			// Make sure all 3 live on the left rail before tabifying. If
			// they're elsewhere (e.g. DatasetControl ships from the
			// legacy path on the Top dock area), move them here first.
			main->addDockWidget(Qt::LeftDockWidgetArea, structures);
			main->addDockWidget(Qt::LeftDockWidgetArea, representations);
			main->addDockWidget(Qt::LeftDockWidgetArea, datasets);

			// Tabify into one slot. tabifyDockWidget(first, second) stacks
			// `second` as a tab alongside `first`. Chain the calls to put
			// all 3 in the same tab group.
			main->tabifyDockWidget(structures, representations);
			main->tabifyDockWidget(representations, datasets);

			// Default visible tab = Structures (raised to front).
			structures->raise();

			// Suppress the title bars — the tab strip is the new identity.
			hideTitleBar(structures);
			hideTitleBar(representations);
			hideTitleBar(datasets);

			// Make sure all 3 are visible — addDockWidget alone doesn't
			// guarantee visibility if a prior preset hid them.
			structures->setVisible(true);
			representations->setVisible(true);
			datasets->setVisible(true);
		}

	} // namespace VIEW
} // namespace BALL
