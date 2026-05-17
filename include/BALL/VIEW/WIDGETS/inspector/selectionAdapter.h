// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 03 — SelectionInspectorAdapter: ConnectionObject
// child of MainControl that listens for selection-change messages
// (ControlSelectionMessage / NewSelectionMessage) and forwards the
// current selection list to the Inspector's Selection-tab sections.
//

#ifndef BALL_VIEW_WIDGETS_INSPECTOR_SELECTIONADAPTER_H
#define BALL_VIEW_WIDGETS_INSPECTOR_SELECTIONADAPTER_H


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
		 * ConnectionObject adapter between MainControl's message bus
		 * and the Inspector's Selection tab. Constructed with a
		 * MainControl* and an InspectorView*. Self-registers as a
		 * child ConnectionObject of MainControl in the constructor;
		 * unregisters in the destructor.
		 *
		 * When a selection-change message arrives, the adapter pulls
		 * the live selection from MainControl::getMolecularControlSelection
		 * and calls InspectorView::setSelection.
		 *
		 * Not a QObject — message-bus traffic doesn't need Qt signals;
		 * the forwarded call into InspectorView happens synchronously
		 * on the same thread that delivered the message.
		 */
		class BALL_VIEW_EXPORT SelectionInspectorAdapter : public ConnectionObject
		{
			public:
				SelectionInspectorAdapter(MainControl* mc, InspectorView* view);
				~SelectionInspectorAdapter() override;

				void onNotify(Message* message) override;

			private:
				MainControl*   main_control_;
				InspectorView* view_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_INSPECTOR_SELECTIONADAPTER_H
