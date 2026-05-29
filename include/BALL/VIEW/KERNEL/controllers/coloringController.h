// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringController (read-only mirror).
//
// Source dialog: source/VIEW/DIALOGS/coloringSettingsDialog.C (~878 LOC).
//
// Phase 999.59-02 (v1.7.x-24) — cut over to the §2 `bool apply()` command
// contract (ARCHITECTURE-CONTRACT.md §2 / §2a / §13). ColoringController now
// inherits the base Controller and overrides bool apply() in the §2 shape.
// Its §2a declared invalidation is a SOFT refresh
// (representation_->update(false)): a coloring-method change rewalks the
// color processor over the existing geometry but leaves the primitive set
// intact, so no display-list rebuild is needed.
//

#ifndef BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H
#define BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H


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
// 999.59-02 per-controller contract-fixture activation symbol (Codex MEDIUM
// #7) — flips coloring_contract_test's parity block live (see
// test/contract/contractFixtureActivation.h). Defined ONLY because apply()
// below returns bool.
// ---------------------------------------------------------------------------
#define BALL_VIEW_COLORING_APPLY_BOOL 1

namespace BALL
{
	namespace VIEW
	{
		class Representation;

		/**
		 * Per-Representation Coloring settings — the coloringMethod
		 * enum + any per-method parameters. Cut over to the §2 `bool apply()`
		 * command contract in 999.59-02 (declared §2a invalidation: SOFT
		 * refresh — a color change leaves the primitive set intact).
		 */
		class BALL_VIEW_EXPORT ColoringController : public Controller
		{
			Q_OBJECT

			Q_PROPERTY(int coloringMethod READ coloringMethod WRITE setColoringMethod NOTIFY coloringMethodChanged)
			Q_PROPERTY(float valueMin READ valueMin WRITE setValueMin NOTIFY valueMinChanged)
			Q_PROPERTY(float valueMax READ valueMax WRITE setValueMax NOTIFY valueMaxChanged)

			public:
				explicit ColoringController(Representation* rep = nullptr,
				                            QObject* parent = nullptr);
				~ColoringController() override;

				Representation* representation() const { return rep_; }
				void setRepresentation(Representation* rep);

				int coloringMethod() const { return coloring_method_; }
				float valueMin() const { return value_min_; }
				float valueMax() const { return value_max_; }
				// 999.59-02 — re-entrancy state now lives in the base depth
				// counter (applying_depth_).
				bool isApplying() const { return applying_depth_ > 0; }

				/**
				 * Apply the staged coloring settings to the attached
				 * Representation, command-shaped per §2. Returns true when the
				 * Representation was mutated, false when dropped (re-entry) or
				 * rejected (no Representation / busy). Overrides
				 * Controller::apply(). Called directly by the Inspector section
				 * (controller_->apply()), not as a string-based Qt slot, so the
				 * bool return is binding-surface-compatible with prior void.
				 * @return true if the owner was mutated, false otherwise.
				 */
				bool apply() override;

				/**
				 * Reset the mirror to the owner's current state (999.64 reset
				 * path). Overrides Controller::reset(); delegates to revert().
				 */
				void reset() override;

			public Q_SLOTS:
				void revert();
				void setColoringMethod(int m);
				void setValueMin(float v);
				void setValueMax(float v);

			Q_SIGNALS:
				void coloringMethodChanged(int m);
				void valueMinChanged(float v);
				void valueMaxChanged(float v);
				void appliedStub();

			protected:
				/**
				 * §2a declared invalidation — SOFT refresh
				 * (representation_->update(false)). A coloring-method or
				 * value-range change re-walks the color processor over the
				 * existing primitives; the primitive set is unchanged, so no
				 * display-list rebuild is required (contrast ModelController,
				 * which rebuilds). Overrides Controller::invalidateDeclared_().
				 */
				void invalidateDeclared_() override;

			private:
				/**
				 * §13-cookbook mutation body, moved out of apply(): rebuilds
				 * the color processor when the method changed and pushes the
				 * value range. Precondition rep_ != nullptr guaranteed by
				 * apply(). Does NOT invalidate (that is invalidateDeclared_()).
				 * @return true (it always installs the staged coloring state).
				 */
				bool applyInternal_();

				/// Capture-only reversible intent (§2 step 7; v2.0 UndoStack).

				Representation* rep_;
				int coloring_method_;
				float value_min_;
				float value_max_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_KERNEL_CONTROLLERS_COLORINGCONTROLLER_H
