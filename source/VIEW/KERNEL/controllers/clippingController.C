// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ClippingController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/clippingController.h>


#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{

		ClippingController::ClippingController(Scene* scene, QObject* parent)
			: QObject(parent), scene_(scene),
				enabled_(false), offset_(0.0f), capped_(true)
		{
		}

		ClippingController::~ClippingController() = default;

		void ClippingController::setScene(Scene* scene)
		{
			if (scene_ == scene) return;
			scene_ = scene;
			revert();
		}

		void ClippingController::revert() { /* read-only mirror — defaults */ }

		void ClippingController::apply()
		{
			// UFG-11 audit decision — Latent stub, no Inspector consumer
			// in v1.7. The Inspector does not add a ClippingSection
			// (see source/VIEW/WIDGETS/inspector/inspectorView.C —
			// only RepHeader / Model / Coloring / Material are wired
			// on the Representation tab as of v1.7.0-rc3). Cut-over
			// deferred to v1.7.x when the Clipping section lands as
			// part of the "Inspector feature parity with legacy
			// dialogs" tracking work. Mutation continues to flow
			// through the legacy ClippingDialog reachable via Tools >
			// Legacy Settings during the migration window.
			Log.info() << "[ClippingController::apply] STUB — no Inspector consumer "
				"in v1.7; legacy ClippingDialog owns mutation. enabled="
				<< (enabled_ ? "true" : "false")
				<< " offset=" << offset_
				<< " capped=" << (capped_ ? "true" : "false") << std::endl;
			Q_EMIT appliedStub();
		}

		void ClippingController::setEnabled(bool b)
		{
			if (b == enabled_) return;
			enabled_ = b;
			Q_EMIT enabledChanged(b);
		}

		void ClippingController::setOffset(float v)
		{
			if (v == offset_) return;
			offset_ = v;
			Q_EMIT offsetChanged(v);
		}

		void ClippingController::setCapped(bool b)
		{
			if (b == capped_) return;
			capped_ = b;
			Q_EMIT cappedChanged(b);
		}

	} // namespace VIEW
} // namespace BALL

