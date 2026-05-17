// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_PROJECTDOCK_H
#define BALL_VIEW_WIDGETS_PROJECTDOCK_H

// Phase 999.45 — BALLView Refresh: ProjectDock
// =============================================
//
// ProjectDock is the left-rail "Project" container in the refreshed
// Default workspace (Handover §5.1). It groups 3 of BALLView's
// existing dock-widget controls into a tabified set:
//
//   Tab 1: Structures        (MolecularControl)
//   Tab 2: Representations   (GeometricControl)
//   Tab 3: Datasets          (DatasetControl)
//
// **Implementation note (v1.7 Wave 4):** MolecularControl /
// GeometricControl / DatasetControl all inherit from VIEW::DockWidget
// (themselves QDockWidget subclasses), and BALL's MainControl message
// bus expects them to remain registered ModularWidgets. We therefore
// don't reparent them into a plain QStackedWidget — instead, ProjectDock
// is a *helper* (not a QWidget itself) that:
//
//   1. Renames the 3 docks' objectName-s into a stable trio
//      (projectDock.structures / .representations / .datasets) so
//      WorkspaceManager can address the group as a unit.
//   2. Calls QMainWindow::tabifyDockWidget on the 3 so they share a
//      single tabbed dock-area slot on the left rail.
//   3. Hides each one's title bar so the bottom tab strip (Qt's
//      built-in tabified-dock tab bar) is the only visible chrome.
//   4. Raises the Structures tab as the default active tab.
//
// Calling code:
//
//   #ifdef BALL_UI_V2
//     ProjectDock::tabify(this, mol_ctrl, geom_ctrl, dataset_ctrl);
//   #endif
//
// Mainframe calls this AFTER constructing the 3 controls and BEFORE
// WorkspaceManager::apply(Default, this) — see Task 7.
//
// The combined dock object is reachable via main->findChild<QDockWidget*>(
// "projectDock") — we set this objectName on the FIRST (Structures)
// dock as the canonical entry point.

#include <BALL/COMMON/global.h>

class QMainWindow;
class QDockWidget;

namespace BALL
{
	namespace VIEW
	{
		/**
		 *  Helper that tabifies the 3 project-related docks into a
		 *  single left-rail Project group (Handover §5.1).
		 *
		 *  Not itself a QWidget — operates on existing docks owned by
		 *  the QMainWindow.
		 */
		class BALL_VIEW_EXPORT ProjectDock
		{
			public:
				/**
				 *  Tabify the 3 docks into a single Project group.
				 *
				 *  Assigns objectName-s, hides title bars, calls
				 *  tabifyDockWidget, and raises the Structures dock as
				 *  the default visible tab. All 3 dock pointers must be
				 *  non-null and already added to the QMainWindow with
				 *  addDockWidget(LeftDockWidgetArea, ...).
				 *
				 *  @param main           the QMainWindow that owns the docks
				 *  @param structures     MolecularControl dock (becomes "projectDock")
				 *  @param representations GeometricControl dock (becomes "projectDock.representations")
				 *  @param datasets       DatasetControl dock (becomes "projectDock.datasets")
				 */
				static void tabify(QMainWindow* main,
				                   QDockWidget* structures,
				                   QDockWidget* representations,
				                   QDockWidget* datasets);

				/**
				 *  objectName the WorkspaceManager / View > Workspace
				 *  presets address the Project group by.
				 */
				static const char* kProjectDockObjectName;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_PROJECTDOCK_H
