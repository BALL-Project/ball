// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 03 — SelectionInspectorAdapter implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/selectionAdapter.h>

#ifdef BALL_UI_V2

#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorView.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/COMMON/rtti.h>

namespace BALL
{
	namespace VIEW
	{

		SelectionInspectorAdapter::SelectionInspectorAdapter(MainControl* mc,
		                                                     InspectorView* view)
			: ConnectionObject(),
				main_control_(mc),
				view_(view)
		{
			if (mc != nullptr) mc->registerConnectionObject(*this);
		}

		SelectionInspectorAdapter::~SelectionInspectorAdapter()
		{
			if (main_control_ != nullptr)
				main_control_->unregisterConnectionObject(*this);
		}

		void SelectionInspectorAdapter::onNotify(Message* message)
		{
			if (message == nullptr || view_ == nullptr || main_control_ == nullptr)
				return;

			// We respond to ANY selection-style message by re-fetching
			// the live selection list — cheaper than maintaining our
			// own copy and immune to the Control vs NewSelection ↔
			// GeometricObjectSelection split.
			if (RTTI::isKindOf<ControlSelectionMessage>(message)
			 || RTTI::isKindOf<NewSelectionMessage>(message))
			{
				view_->setSelection(main_control_->getMolecularControlSelection());
			}
		}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
