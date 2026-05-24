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
			// Base no-op. Subclasses pull the owner's current state into their
			// mirror in a single event (the 999.64 reset path).
		}

		void Controller::invalidateDeclared_()
		{
			// Base no-op. Each subclass declares its own §2a invalidation
			// level here (soft refresh / display-list rebuild / scene-
			// structural side effects).
		}

	} // namespace VIEW
} // namespace BALL
