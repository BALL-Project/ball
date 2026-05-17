// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — RepresentationInspectorAdapter implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/representationAdapter.h>


#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/representationManager.h>
#include <BALL/VIEW/WIDGETS/inspector/inspectorView.h>
#include <BALL/COMMON/rtti.h>

#include <list>

namespace BALL
{
	namespace VIEW
	{

		RepresentationInspectorAdapter::RepresentationInspectorAdapter(MainControl* mc,
		                                                               InspectorView* view)
			: ConnectionObject(),
				main_control_(mc),
				view_(view)
		{
			if (mc != nullptr) mc->registerConnectionObject(*this);
		}

		RepresentationInspectorAdapter::~RepresentationInspectorAdapter()
		{
			if (main_control_ != nullptr)
				main_control_->unregisterConnectionObject(*this);
		}

		void RepresentationInspectorAdapter::onNotify(Message* message)
		{
			if (message == nullptr || view_ == nullptr || main_control_ == nullptr)
				return;
			if (RTTI::isKindOf<RepresentationMessage>(message))
				refresh();
		}

		void RepresentationInspectorAdapter::refresh()
		{
			if (main_control_ == nullptr || view_ == nullptr) return;
			RepresentationManager& rm = main_control_->getRepresentationManager();
			const RepresentationList& reps = rm.getRepresentations();
			// Pick first representation as active for the tab. The tab
			// header's combo lets the user override this.
			Representation* active = reps.empty() ? nullptr : *reps.begin();
			std::list<Representation*> rep_list(reps.begin(), reps.end());
			view_->setRepresentations(rep_list, active);
		}

	} // namespace VIEW
} // namespace BALL

