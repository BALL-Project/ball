// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.59-01 (v1.7.x-24) — Controller command-contract: reversible
// intent payload (ARCHITECTURE-CONTRACT.md §2).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_APPLYPAYLOAD_H
#define BALL_VIEW_KERNEL_CONTROLLERS_APPLYPAYLOAD_H

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/**
		 * Reversible-intent payload captured by every Controller::apply()
		 * (ARCHITECTURE-CONTRACT.md §2, step 3 + step 7).
		 *
		 * A command-shaped apply() snapshots the owner state it is about to
		 * change into an ApplyPayload, emits it on its single typed event,
		 * and (in v1.7.4) records it for capture. v2.0's UndoStack consumes
		 * the captured payloads to reverse a mutation — so the struct is a
		 * plain, copy-constructible aggregate with NO QObject base, cheap to
		 * store on a stack.
		 *
		 * The <tt>before</tt> / <tt>after</tt> snapshots are String form to
		 * match the harness OwnerSnapshot string blobs (cheap to compare with
		 * EXPECT_EQ and to print on mismatch without a bespoke diff routine).
		 *
		 * Fields:
		 *  - <tt>command_id</tt> : dotted command identity, e.g. "x.setFoo".
		 *  - <tt>before</tt>     : owner-state snapshot prior to the mutation.
		 *  - <tt>after</tt>      : owner-state snapshot the mutation installs.
		 *  - <tt>target</tt>     : the mutated owner (non-owning pointer; a
		 *                          <tt>void*</tt> so the header carries no
		 *                          QObject / owner-type dependency and stays
		 *                          headless-includable).
		 *  - <tt>t_us</tt>       : capture timestamp in microseconds (populate
		 *                          from <tt>PreciseTime::now().getMicroSeconds()</tt>).
		 *
		 * Header-only POD aggregate — implicitly copy-constructible and
		 * copy-assignable (String is value-copyable). No QObject base.
		 */
		struct ApplyPayload
		{
			String command_id;   ///< Dotted command identity (e.g. "model.setType").
			String before;       ///< Owner-state snapshot before the mutation.
			String after;        ///< Owner-state snapshot the mutation installs.
			void*  target = nullptr; ///< Mutated owner (non-owning).
			long   t_us   = 0;    ///< Capture time in microseconds (PreciseTime::now().getMicroSeconds()).
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_APPLYPAYLOAD_H
