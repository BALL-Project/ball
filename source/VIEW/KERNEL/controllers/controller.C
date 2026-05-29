// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.59-01 (v1.7.x-24) — Base Controller implementation.
//
// Trivial base implementations for the command contract (§2): apply()
// returns false (no-op base), reset() is a no-op, invalidateDeclared_()
// is empty. Subclasses override these in 999.59-02/03. The base exists so
// the nine cut-overs are harmonization onto one shape, not nine
// independent re-implementations.
//

#include <BALL/VIEW/KERNEL/controllers/controller.h>
#include <BALL/VIEW/KERNEL/controllers/controllerJob.h>

using namespace BALL;

namespace BALL
{
	namespace VIEW
	{

		Controller::Controller(QObject* parent)
			: QObject(parent)
		{
		}

		Controller::~Controller() = default;

		bool Controller::apply()
		{
			// Base no-op: nothing to mutate. Subclasses override with the §2
			// validate / guard / capture / mutate / emit / invalidate body and
			// return true on mutation.
			return false;
		}

		void Controller::reset()
		{
			// Base no-op (the 999.64 reset path). A mutating subclass sets its
			// mirror to its method-defined defaults, emits the change
			// notifications, then runs one §2 apply() — exactly one event, one
			// reversible payload. The read-only sections (Properties / Summary /
			// RepHeader) have no owner, so this base no-op is their correct reset.
		}

		void Controller::invalidateDeclared_()
		{
			// Base no-op. Each subclass declares its own §2a invalidation
			// level here (soft refresh / display-list rebuild / scene-
			// structural side effects).
		}

		void Controller::recordIntent_(const ApplyPayload& payload)
		{
			// v1.7.4 — CAPTURE ONLY (§2 step 7 / §2d). Store the last payload
			// so a future v2.0 UndoStack can consume it. We do NOT push to any
			// stack and no UI reads it in v1.7.4. Hoisted to the base in
			// 999.59-05 (the nine controllers previously each held an identical
			// copy of this body).
			last_payload_ = payload;
		}

		const ApplyPayload& Controller::lastIntent_() const
		{
			return last_payload_;
		}

		bool Controller::kickJob_(ControllerJob& job)
		{
			// §6 — defer to the job and re-enter on the GUI thread. The QUEUED
			// connection guarantees applyResult_ runs on this Controller's
			// (GUI) thread even though the job emits finished() from its worker
			// thread. apply() returns this false: nothing mutated synchronously.
			QObject::connect(&job, &ControllerJob::finished,
			                 this, &Controller::applyResult_,
			                 Qt::QueuedConnection);
			job.start();
			return false;
		}

		void Controller::applyResult_(const ApplyPayload& /*payload*/)
		{
			// Base no-op. A subclass that defers work to a ControllerJob
			// overrides this to run the §2 apply() contract (guard / mutate /
			// emit / invalidate) on the GUI thread with the job's result.
		}

	} // namespace VIEW
} // namespace BALL
