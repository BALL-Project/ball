// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.59-01 (v1.7.x-24) — Base Controller class for the command
// contract (ARCHITECTURE-CONTRACT.md §2 / §2a / §9).
//
// NEW in 999.59-01: there is no base Controller class today — the nine
// VIEW controllers are independent QObjects. This base lands the §2
// `bool apply()` shape, the §2a per-domain `invalidateDeclared_()` hook,
// the `virtual void reset()` single-event reset path (999.64 consumes),
// and the nest-aware re-entrancy depth counter the guard binds to. No
// existing controller inherits from it yet — that is 999.59-02 / 03.
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLER_H

// ---------------------------------------------------------------------------
// Feature macro — the 999.60 contract-test harness flips per-controller
// parity fixtures from STATUS-skip to live the moment this macro is defined
// (see test/contract/contractTestHarness.h: CONTRACT_FIXTURE_DISABLED). It
// is the signal that the §2 `bool apply()` command contract has landed. The
// exact spelling is fixed by the harness — do NOT rename.
// ---------------------------------------------------------------------------
#define BALL_VIEW_CONTROLLER_APPLY_RETURNS_BOOL 1

#ifndef BALL_COMMON_GLOBAL_H
#	include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLERAPPLYGUARD_H
#	include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_APPLYPAYLOAD_H
#	include <BALL/VIEW/KERNEL/controllers/applyPayload.h>
#endif

#include <QtCore/QObject>

namespace BALL
{
	namespace VIEW
	{

		class ControllerJob;

		/**
		 * Base class for the BALLView command-shaped controllers
		 * (ARCHITECTURE-CONTRACT.md §2 / §2a / §9).
		 *
		 * Every mutating controller in the VIEW layer routes owner mutation
		 * through a single command-shaped <tt>apply()</tt>:
		 * validate → guard → capture → mutate the single owner → emit one
		 * typed event → request the controller's <i>declared</i> invalidation
		 * (§2a) → record reversible intent. This base declares that surface;
		 * the nine concrete controllers harmonise onto it in 999.59-02/03.
		 *
		 * <b>Lifetime (§9 convention 1).</b> A Controller is owner-rooted: the
		 * caller (typically MainControl) passes itself as <tt>parent</tt> so
		 * Qt's parent/child ownership destroys the controller with its owner.
		 *
		 * <b>Re-entrancy (§2 / §4).</b> <tt>apply()</tt> runs ONLY on the GUI
		 * thread. The protected <tt>applying_depth_</tt> counter binds to a
		 * stack-scoped ControllerApplyGuard inside each subclass's
		 * <tt>apply()</tt>; a re-entrant call (depth > 0) is DROPPED and
		 * returns false. The guard is a nest-aware flag, not a mutex (§4
		 * Codex HIGH #6).
		 *
		 * The base implementations are deliberately trivial no-ops so the
		 * base is instantiable but inert until a subclass overrides.
		 */
		class BALL_VIEW_EXPORT Controller
			: public QObject
		{
			Q_OBJECT

			public:

				/**
				 * Construct an owner-rooted controller. The caller passes its
				 * owner (e.g. MainControl) as @p parent so Qt parent/child
				 * ownership destroys the controller with its owner (§9).
				 * @param parent the owning QObject, or nullptr.
				 */
				explicit Controller(QObject* parent = nullptr);

				/// Destructor.
				~Controller() override;

				/**
				 * Apply the pending mutation to the single owner, command-shaped
				 * per §2. Returns true when the owner was mutated, false when
				 * the call was dropped (re-entry) or rejected (precondition /
				 * validation failure). The base implementation is a no-op that
				 * returns false; subclasses override with the §2 body.
				 * @return true if the owner was mutated, false otherwise.
				 */
				virtual bool apply();

				/**
				 * Reset the controller to the owner's current state in a single
				 * event — the reset path 999.64 consumes (NOT the legacy
				 * two-step revert(); apply()). The base implementation is a
				 * documented no-op; subclasses pull owner state into their
				 * mirror and emit one change notification.
				 */
				virtual void reset();

			protected:

				/**
				 * Request this controller's DECLARED invalidation (§2a). There
				 * is no single rule: most controllers request a soft refresh,
				 * Model requests a display-list rebuild, Stage declares
				 * scene-structural side effects. Each subclass overrides this
				 * to declare its own level in one place; the base is an empty
				 * no-op. Called by the subclass's <tt>apply()</tt> after the
				 * mutation, never a blanket full-scene rebuild.
				 */
				virtual void invalidateDeclared_();

				/**
				 * Record the reversible intent for a mutation (§2 step 7 / §2d).
				 * v1.7.4 is CAPTURE-ONLY: it stores @p payload as the last intent
				 * so a future v2.0 UndoStack can consume it. It does NOT push to
				 * any stack and no UI reads it in v1.7.4. Every subclass's
				 * <tt>apply()</tt> calls this with a populated ApplyPayload
				 * (before/after snapshots, unique command_id, target, timestamp)
				 * before returning true. Hoisted to the base in 999.59-05 — the
				 * nine controllers previously each carried an identical copy.
				 * @param payload the captured before/after intent.
				 */
				void recordIntent_(const ApplyPayload& payload);

				/**
				 * The last reversible intent captured by recordIntent_ (§2d).
				 * Reachable for v2.0 undo consumption / tests; not consumed in
				 * v1.7.4.
				 * @return the most recently recorded ApplyPayload.
				 */
				const ApplyPayload& lastIntent_() const;

				/**
				 * Defer a mutation to an async ControllerJob (§6). The owning
				 * Controller calls this from <tt>apply()</tt> when the work is
				 * too long to run synchronously: it connects @p job's
				 * <tt>finished()</tt> to <tt>applyResult_()</tt> with a QUEUED
				 * connection (so the result runs on the GUI thread), starts the
				 * job, and returns false — NOTHING is mutated synchronously. The
				 * worker thread must not touch the owner or GL context. No
				 * concrete job ships in v1.7.4; this is the plumbing only.
				 * @param job the job to kick (lifetime owned by the caller).
				 * @return always false (nothing mutated synchronously).
				 */
				bool kickJob_(ControllerJob& job);

				/**
				 * GUI-thread handler for a ControllerJob result (§6). Invoked via
				 * the queued <tt>finished()</tt> connection, on the GUI thread, so
				 * a subclass override can run the §2 apply() contract — guard /
				 * mutate / emit / invalidate — safely. The base is a no-op.
				 * @param payload the job's captured result.
				 */
				virtual void applyResult_(const ApplyPayload& payload);

				/**
				 * Nest-aware re-entrancy depth (§2 / §4). A subclass's
				 * <tt>apply()</tt> checks <tt>applying_depth_ > 0</tt> and
				 * drops, then binds a stack-scoped ControllerApplyGuard to
				 * this counter. GUI-thread-only — not thread-safe by design.
				 */
				int applying_depth_ = 0;

				/**
				 * Capture-only store for the last reversible intent (§2d).
				 * v2.0 UndoStack consumes this; v1.7.4 only records it.
				 */
				ApplyPayload last_payload_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLER_H
