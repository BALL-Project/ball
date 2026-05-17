// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — RepresentationInspectorAdapter:
// ConnectionObject child of MainControl that listens for
// RepresentationMessage and refreshes the Inspector Representation
// tab.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_REPRESENTATIONADAPTER_H
#define BALL_VIEW_WIDGETS_INSPECTOR_REPRESENTATIONADAPTER_H

#ifdef BALL_UI_V2

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <BALL/VIEW/KERNEL/connectionObject.h>

namespace BALL
{
	namespace VIEW
	{
		class InspectorView;
		class MainControl;

		/**
		 * Bridges MainControl::getRepresentationManager() into the
		 * Inspector Representation tab. On any RepresentationMessage
		 * the adapter snapshots the current representation list +
		 * picks the first one as active, then calls
		 * InspectorView::setRepresentations().
		 */
		class BALL_VIEW_EXPORT RepresentationInspectorAdapter : public ConnectionObject
		{
			public:
				RepresentationInspectorAdapter(MainControl* mc, InspectorView* view);
				~RepresentationInspectorAdapter() override;

				void onNotify(Message* message) override;

				/** Snapshot the rep list NOW and push it through. */
				void refresh();

			private:
				MainControl*   main_control_;
				InspectorView* view_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
#endif // BALL_VIEW_WIDGETS_INSPECTOR_REPRESENTATIONADAPTER_H
