// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ClippingController (read-only mirror).
//
// Source dialogs: source/VIEW/DIALOGS/clippingDialog.C (~119 LOC) +
// setClippingPlane.C (~92 LOC); merged per Handover §architecture.
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CLIPPINGCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_CLIPPINGCONTROLLER_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_CONTROLLER_H
# include <BALL/VIEW/KERNEL/controllers/controller.h>
#endif

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_APPLYPAYLOAD_H
# include <BALL/VIEW/KERNEL/controllers/applyPayload.h>
#endif

#include <QtCore/QObject>

// ---------------------------------------------------------------------------
// 999.59-03 per-controller contract-fixture activation symbol (Codex MEDIUM
// #7) — flips clipping_contract_test's parity block live (see
// test/contract/contractFixtureActivation.h). Defined ONLY because apply()
// below returns bool AND now takes the ControllerApplyGuard the §4 code
// explicitly omitted (the omission is CLOSED). §2a declared invalidation:
// SOFT refresh (a clip-plane change leaves the primitive set intact).
// ---------------------------------------------------------------------------
#define BALL_VIEW_CLIPPING_APPLY_BOOL 1

namespace BALL
{
	namespace VIEW
	{
		class Scene;
		class ClippingPlane;

		/**
		 * Clipping-plane controller — enabled state + plane offset. Cut over
		 * to the §2 `bool apply()` command contract in 999.59-03: the body
		 * (previously a void slot that explicitly OMITTED the re-entrancy
		 * guard, §4 / Codex HIGH #6) now takes the ControllerApplyGuard and
		 * declares a SOFT-refresh §2a invalidation (a clip-plane change leaves
		 * the primitive set intact).
		 */
		class BALL_VIEW_EXPORT ClippingController : public Controller
		{
			Q_OBJECT

			Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
			Q_PROPERTY(float offset READ offset WRITE setOffset NOTIFY offsetChanged)
			Q_PROPERTY(bool capped READ capped WRITE setCapped NOTIFY cappedChanged)

			public:
				explicit ClippingController(Scene* scene = nullptr,
				                            QObject* parent = nullptr);
				~ClippingController() override;

				Scene* scene() const { return scene_; }
				void setScene(Scene* scene);

				bool  enabled() const { return enabled_; }
				float offset() const  { return offset_; }
				bool  capped() const  { return capped_; }

				/// 999.59-03 — true while apply() is mutating (base depth counter).
				bool isApplying() const { return applying_depth_ > 0; }

				/**
				 * Apply the staged clip-plane state to the owner (find-or-create
				 * the Inspector's clipping plane through the RepresentationManager),
				 * command-shaped per §2. Returns true when the clip state was
				 * mutated, false when dropped (re-entry) or rejected (no
				 * MainControl / busy). Overrides Controller::apply(); now takes
				 * the ControllerApplyGuard the prior body omitted (§4).
				 * @return true if the owner was mutated, false otherwise.
				 */
				bool apply() override;

				/**
				 * Reset the clipping settings to the method-defined defaults
				 * (disabled, zero offset, capping enabled) in a SINGLE §2 event
				 * (999.64 reset path — NOT the legacy two-step revert(); apply()).
				 * Sets the mirror to the defaults, emits the change notifications,
				 * then runs one apply() (one event, one reversible payload).
				 * Overrides Controller::reset().
				 */
				void reset() override;

			public Q_SLOTS:
				void revert();
				void setEnabled(bool b);
				void setOffset(float v);
				void setCapped(bool b);

			Q_SIGNALS:
				void enabledChanged(bool b);
				void offsetChanged(float v);
				void cappedChanged(bool b);
				/// §2 step 5 — the ONE typed event apply() emits. Replaces the
				/// 999.44 migration-window stub signal (removed in the
				/// 999.59-03 cut-over).
				void applied();

			protected:
				/**
				 * §2a declared invalidation — SOFT refresh: a clip-plane change
				 * re-draws the EXISTING geometry via mc->redrawAllRepresentations()
				 * without rebuilding the primitive set. Overrides
				 * Controller::invalidateDeclared_().
				 */
				void invalidateDeclared_() override;

			private:
				/// §13-cookbook mutation body, moved out of apply(): find-or-
				/// create the Inspector's clipping plane (through the
				/// RepresentationManager when a MainControl is present; a
				/// controller-owned fallback plane otherwise, e.g. headless) and
				/// push the mirrored enabled / capped / offset state onto it.
				/// @return true (a clip-plane mutation is always performed).
				bool applyInternal_();

				/// Capture-only reversible intent (§2 step 7; v2.0 UndoStack).

				Scene* scene_;
				bool   enabled_;
				float  offset_;
				bool   capped_;

				// Controller-owned fallback clipping plane used when no
				// MainControl/RepresentationManager is reachable (e.g. the
				// headless contract test). In production the manager owns the
				// plane; this stays nullptr.
				ClippingPlane* owned_plane_ = nullptr;

		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_CLIPPINGCONTROLLER_H
