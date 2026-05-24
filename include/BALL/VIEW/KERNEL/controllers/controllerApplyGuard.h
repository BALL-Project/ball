// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v1.7.x-24 — Controller command-contract: re-entrancy guard.
//
// Phase 999.59-01 — upgraded from the one-shot `bool&` flag to a
// nest-aware RAII depth counter (ARCHITECTURE-CONTRACT.md §4).
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLERAPPLYGUARD_H
#define BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLERAPPLYGUARD_H

namespace BALL
{
	namespace VIEW
	{

		/**
		 * Nest-aware RAII re-entrancy shield for Controller::apply().
		 *
		 * The VIEW notification bus (ConnectionObject) can synchronously
		 * re-enter a Controller through the very message its own apply()
		 * triggers — this is the cascade behind the v1.7.x-13 freeze
		 * (a render/selection refresh emitted mid-mutate re-invoked the
		 * mutating path). The command-contract therefore requires every
		 * apply() to run inside a guard.
		 *
		 * <b>Nest-aware depth counter (999.59-01).</b> The guard binds to an
		 * <tt>int&</tt> depth counter (NOT a one-shot <tt>bool&</tt>): the
		 * constructor increments it, the destructor decrements it. A caller
		 * tests "am I already applying?" by checking <tt>depth > 0</tt>
		 * BEFORE constructing the guard. Because the counter nests, a helper
		 * that legitimately re-enters apply() at a deeper level is DETECTED
		 * (depth > 0 ⇒ the re-entrant apply() drops and returns false), not
		 * corrupted: each scope decrements exactly the increment it made, so
		 * the outer guard survives the inner scope's exit.
		 *
		 * §2 body shape (ARCHITECTURE-CONTRACT.md §2):
		 *
		 *     bool FooController::apply()
		 *     {
		 *         // ... validate preconditions ...
		 *         if (applying_depth_ > 0) return false;     // 0. re-entry shield (dropped)
		 *         ControllerApplyGuard guard(applying_depth_); // begin guard (++depth)
		 *         // ... mutate the single owner, emit one event, invalidate ...
		 *         return true;                                // mutated
		 *     }                                               // guard ends (--depth, RAII)
		 *
		 * <b>GUI-thread-only — NOT a mutex (§4, Codex HIGH #6).</b> This guard
		 * is a nest-aware flag protecting against synchronous, single-threaded
		 * re-entry on the GUI thread. It is deliberately NOT thread-safe and
		 * MUST NOT be used as a cross-thread lock: <tt>apply()</tt> runs ONLY
		 * on the GUI thread by contract (long work is shunted to a
		 * <tt>ControllerJob</tt> worker thread that re-enters the GUI thread
		 * through a queued slot, §6). The plain <tt>int</tt> increment is
		 * correct precisely because there is no concurrent access.
		 *
		 * Stack-scoped on purpose: the counter is decremented in the
		 * destructor, so it is reset even if apply() throws or takes an early
		 * return after the guard is constructed.
		 *
		 * Header-only; no QObject dependency so Controllers stay headless-
		 * testable (see test/ControllerContract_test.C and test/contract/).
		 */
		class ControllerApplyGuard
		{
			public:
				/// Enter an apply() scope: increment the depth counter.
				explicit ControllerApplyGuard(int& depth)
					: depth_(depth)
				{
					++depth_;
				}

				/// Leave the apply() scope: decrement the depth counter (RAII).
				~ControllerApplyGuard()
				{
					--depth_;
				}

				ControllerApplyGuard(const ControllerApplyGuard&) = delete;
				ControllerApplyGuard& operator=(const ControllerApplyGuard&) = delete;

			private:
				int& depth_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLERAPPLYGUARD_H
