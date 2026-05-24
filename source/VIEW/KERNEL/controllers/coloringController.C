// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringController implementation.
// UFG-03 cut-over (v1.7-modernization) — apply() mutates the attached
// Representation through the per-ColoringMethod processor factory and
// the Representation's own setters.
//
// Phase 999.57 Plan 03 (VIEW-CLEAN-02 render-path cut-over): apply()
// no longer reaches through DisplayProperties' owned
// ColoringSettingsDialog for the processor factory. It now constructs
// the color processor via the relocated ColorProcessorFactory::create
// with EMPTY ColoringOverrides (compiled defaults — the controller does
// not expose per-element/residue color overrides), then applies
// value_min_/value_max_ to the InterpolateColorProcessor exactly as
// before — making the controller fully dialog-independent.
//

#include <BALL/VIEW/KERNEL/controllers/coloringController.h>


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/MODELS/colorProcessorFactory.h>
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/CONCEPT/timeStamp.h>

#include <sstream>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// Compact owner-state snapshot for the ApplyPayload before/after
			// blobs (§2 step 3).
			String snapshotColoringState_(Representation* rep)
			{
				if (rep == nullptr) return String("coloring{null}");
				std::ostringstream s;
				s << "coloring=" << static_cast<int>(rep->getColoringMethod());
				InterpolateColorProcessor* icp =
					dynamic_cast<InterpolateColorProcessor*>(rep->getColorProcessor());
				if (icp != nullptr)
				{
					s << ";min=" << icp->getMinValue()
					  << ";max=" << icp->getMaxValue();
				}
				return String(s.str());
			}
		}

		ColoringController::ColoringController(Representation* rep, QObject* parent)
			: Controller(parent), rep_(rep), coloring_method_(0),
				value_min_(0.0f), value_max_(100.0f)
		{
			revert();
		}

		ColoringController::~ColoringController() = default;

		void ColoringController::setRepresentation(Representation* rep)
		{
			if (rep_ == rep) return;
			rep_ = rep;
			revert();
		}

		void ColoringController::revert()
		{
			if (rep_ == nullptr) return;
			int cm = static_cast<int>(rep_->getColoringMethod());
			if (cm != coloring_method_)
			{
				coloring_method_ = cm;
				Q_EMIT coloringMethodChanged(cm);
			}

			// Pull the value range from the attached processor if it is
			// one of the value-based (interpolating) color processors.
			InterpolateColorProcessor* icp =
				dynamic_cast<InterpolateColorProcessor*>(rep_->getColorProcessor());
			if (icp != nullptr)
			{
				float vmin = icp->getMinValue();
				if (vmin != value_min_) { value_min_ = vmin; Q_EMIT valueMinChanged(vmin); }
				float vmax = icp->getMaxValue();
				if (vmax != value_max_) { value_max_ = vmax; Q_EMIT valueMaxChanged(vmax); }
			}
		}

		void ColoringController::reset()
		{
			// §2 reset path (999.64 consumes) — single-pass re-sync from owner.
			revert();
		}

		bool ColoringController::apply()
		{
			// ── 1. Preconditions (§2 step 1) ─────────────────────────────────
			if (rep_ == nullptr)
			{
				Log.warn() << "[ColoringController::apply] no Representation attached — skipping." << std::endl;
				return false;                    // rejected — nothing to mutate
			}

			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[ColoringController::apply] MainControl busy — deferring." << std::endl;
				return false;                    // rejected — busy
			}

			// ── 2. Re-entrancy shield (§2 step 2 / §4) — nest-aware drop ─────
			if (applying_depth_ > 0) return false;          // dropped — re-entry
			ControllerApplyGuard apply_guard(applying_depth_);

			// ── 3. Capture reversible intent (§2 step 3) ─────────────────────
			ApplyPayload payload;
			payload.command_id = "coloring.setMethod";
			payload.before     = snapshotColoringState_(rep_);
			payload.target     = rep_;
			payload.t_us       = PreciseTime::now().getMicroSeconds();

			// ── 4. Mutate the single owner (§2 step 4) ───────────────────────
			applyInternal_();

			payload.after = snapshotColoringState_(rep_);

			// ── 5. Emit ONE typed event (§2 step 5) ──────────────────────────
			Q_EMIT appliedStub();

			// ── 6. Request the DECLARED invalidation (§2 step 6 / §2a) ───────
			invalidateDeclared_();               // soft refresh

			// ── 7. Record reversible intent (§2 step 7) — capture only ───────
			recordIntent_(payload);

			return true;                          // mutated
		}

		bool ColoringController::applyInternal_()
		{
			// §13 cookbook step 2 — existing mutation body, moved out of
			// apply(). Precondition (rep_ != nullptr) guaranteed by apply().
			// Does NOT invalidate (that is invalidateDeclared_()).
			ColoringMethod new_method = static_cast<ColoringMethod>(coloring_method_);
			bool method_changed = (rep_->getColoringMethod() != new_method);

			if (method_changed || rep_->getColorProcessor() == nullptr)
			{
				// Phase 999.57 Plan 03: build the color processor through the
				// headless ColorProcessorFactory with EMPTY ColoringOverrides.
				// An empty override set means "keep the processor's compiled
				// defaults" (CONFIG-01), which is exactly what this controller
				// wants — it exposes only coloring_method_ + value_min_/value_max_
				// and never carried per-element/residue color overrides. The
				// value range is applied below, unchanged.
				ColorProcessor* cp = ColorProcessorFactory::create(new_method);
				if (cp == nullptr)
				{
					// Defensive fallback — install a baseline processor so the
					// renderer always has something to invoke.
					cp = new ColorProcessor();
				}
				rep_->setColorProcessor(cp);
				rep_->setColoringMethod(new_method);
			}

			// Value-range parameters are live setters on the processor,
			// so a min/max-only change needs no recreate — push them onto
			// whatever processor is currently attached (newly created or
			// pre-existing) before the color re-walk.
			InterpolateColorProcessor* icp =
				dynamic_cast<InterpolateColorProcessor*>(rep_->getColorProcessor());
			if (icp != nullptr)
			{
				icp->setMinValue(value_min_);
				icp->setMaxValue(value_max_);
			}

			return true;
		}

		void ColoringController::invalidateDeclared_()
		{
			// §2a Coloring row — SOFT refresh. A coloring-method / value-range
			// change re-walks the color processor over the EXISTING geometry;
			// the primitive set is unchanged, so update(false) (no display-list
			// rebuild) is correct. Contrast ModelController, which rebuilds
			// because a model-processor swap changes the primitives.
			if (rep_ != nullptr) rep_->update(false);
		}

		void ColoringController::recordIntent_(const ApplyPayload& payload)
		{
			// v1.7.4 — capture only (no UndoStack yet, §2 step 7).
			last_payload_ = payload;
		}

		void ColoringController::setColoringMethod(int m)
		{
			if (m == coloring_method_) return;
			coloring_method_ = m;
			Q_EMIT coloringMethodChanged(m);
		}

		void ColoringController::setValueMin(float v)
		{
			if (v == value_min_) return;
			value_min_ = v;
			Q_EMIT valueMinChanged(v);
		}

		void ColoringController::setValueMax(float v)
		{
			if (v == value_max_) return;
			value_max_ = v;
			Q_EMIT valueMaxChanged(v);
		}

	} // namespace VIEW
} // namespace BALL

