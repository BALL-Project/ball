// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.59-05 (v1.7.x-24) — Controller async-job base implementation
// (ARCHITECTURE-CONTRACT.md §6).
//
// v1.7.4: base + queued re-entry plumbing only — NO concrete job. start()
// emits started() and queues a finished() result via QMetaObject::invokeMethod,
// exercising the worker→GUI hand-off path a real (v2.0) job uses.
//

#include <BALL/VIEW/KERNEL/controllers/controllerJob.h>

namespace BALL
{
	namespace VIEW
	{

		ControllerJob::ControllerJob(QObject* parent)
			: QObject(parent)
		{
		}

		ControllerJob::~ControllerJob() = default;

		void ControllerJob::start()
		{
			Q_EMIT started();

			// v1.7.4 scaffold — no concrete work. Queue the result back onto
			// this object's thread (the worker→GUI re-entry a real job uses): a
			// real job does its work off-thread, then QMetaObject::invokeMethod-
			// queues finished() so the owning Controller's §2 apply() runs on the
			// GUI thread under the re-entrancy guard. The empty payload here is a
			// placeholder; a v2.0 concrete job populates before/after/command_id.
			QMetaObject::invokeMethod(this, [this]()
			{
				Q_EMIT finished(ApplyPayload());
			}, Qt::QueuedConnection);
		}

	} // namespace VIEW
} // namespace BALL
