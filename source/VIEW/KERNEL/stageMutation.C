// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.59-04 (v1.7.4) — StageMutation owner-side facade.
//
// ARCHITECTURE-CONTRACT.md §3b. The StageMutation mutators are inline in
// the header (each is a one-line forward to a private Stage setter that
// StageMutation is the single friend of). This TU exists so the facade
// has a stable home in the VIEW source tree and so a future non-inline
// mutator has a place to live without touching the build wiring.

#include <BALL/VIEW/KERNEL/stageMutation.h>

namespace BALL
{
	namespace VIEW
	{
		// All StageMutation members are inline (header §3b). Intentionally
		// no out-of-line definitions yet.
	} // namespace VIEW
} // namespace BALL
