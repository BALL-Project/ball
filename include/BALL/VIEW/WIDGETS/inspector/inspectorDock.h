// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorDock: QDockWidget host for the BALLView
// Refresh Unified Inspector. Right-rail dock; title-less on macOS
// (Handover §Cross-platform notes); wraps InspectorView.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORDOCK_H
#define BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORDOCK_H

// Phase 999.44: gated by BALL_UI_V2 (see inspectorSection.h).


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtWidgets/QDockWidget>

namespace BALL
{
	namespace VIEW
	{
		class InspectorView;

		/**
		 * QDockWidget host for the Unified Inspector. The dock owns the
		 * InspectorView and persists its dock-visible state under
		 * [Inspector] visible= via the view's INI plumbing.
		 *
		 * QSS objectName: `inspectorDock`.
		 */
		class BALL_VIEW_EXPORT InspectorDock : public QDockWidget
		{
			Q_OBJECT

			public:
				/**
				 * @param parent  The mainframe (or any QMainWindow).
				 * @param title  Dock title (used by Qt to label the dock entry
				 *   in View → docks menu; on macOS the dock chrome is hidden).
				 */
				InspectorDock(QWidget* parent, const QString& title);
				~InspectorDock() override;

				InspectorView* view() const { return view_; }

			private:
				InspectorView* view_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_INSPECTORDOCK_H
