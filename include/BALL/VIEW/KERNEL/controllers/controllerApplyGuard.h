// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v1.7.x-24 — Controller command-contract: re-entrancy guard.
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLERAPPLYGUARD_H
#define BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLERAPPLYGUARD_H

namespace BALL
{
	namespace VIEW
	{

		/**
		 * RAII re-entrancy shield for Controller::apply().
		 *
		 * The VIEW notification bus (ConnectionObject) can synchronously
		 * re-enter a Controller through the very message its own apply()
		 * triggers — this is the cascade behind the v1.7.x-13 freeze
		 * (a render/selection refresh emitted mid-mutate re-invoked the
		 * mutating path). The command-contract therefore requires every
		 * apply() to run inside a guard: set a flag for the duration of the
		 * mutation, and have apply() (and any notification slot that could
		 * re-trigger it) early-return while the flag is set.
		 *
		 * Stack-scoped on purpose: the flag is cleared in the destructor, so
		 * it is reset even if apply() throws or takes an early return after
		 * the guard is constructed. Pattern:
		 *
		 *     void FooController::apply()
		 *     {
		 *         if (applying_) return;                  // 0. re-entry shield
		 *         // ... validate preconditions ...
		 *         ControllerApplyGuard guard(applying_);  // begin guard
		 *         // ... mutate the single owner, emit one event, invalidate ...
		 *     }                                           // guard ends (RAII)
		 *
		 * Header-only; no QObject dependency so Controllers stay headless-
		 * testable (see test/ControllerContract_test.C).
		 */
		class ControllerApplyGuard
		{
			public:
				explicit ControllerApplyGuard(bool& flag)
					: flag_(flag)
				{
					flag_ = true;
				}

				~ControllerApplyGuard()
				{
					flag_ = false;
				}

				ControllerApplyGuard(const ControllerApplyGuard&) = delete;
				ControllerApplyGuard& operator=(const ControllerApplyGuard&) = delete;

			private:
				bool& flag_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLERAPPLYGUARD_H
