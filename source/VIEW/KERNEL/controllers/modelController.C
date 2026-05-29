// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ModelController implementation.
// UFG-03 cut-over (v1.7-modernization) — apply() mutates the attached
// Representation through the Representation's own setters and the
// per-ModelType processor factory.
//
// Phase 999.57 Plan 03 (VIEW-CLEAN-02 render-path cut-over): apply()
// and revert() no longer reach through DisplayProperties' owned
// ModelSettingsDialog for the processor factory. apply() now builds a
// headless ModelProcessorParams from the controller's own mirrored
// member fields and constructs the processor via the relocated
// ModelProcessorFactory::create — making the controller fully
// dialog-independent. revert() now mirrors from the controller's own
// last-known values (the controller is the source of truth for its
// mirrored params), with model/drawing state still pulled from the
// Representation.
//

#include <BALL/VIEW/KERNEL/controllers/modelController.h>


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/MODELS/modelProcessorFactory.h>
#include <BALL/VIEW/MODELS/representationBuilder.h>
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
			// blobs (§2 step 3). Cheap String form, matching the harness
			// OwnerSnapshot discipline.
			String snapshotModelState_(const Representation* rep)
			{
				if (rep == nullptr) return String("model{null}");
				std::ostringstream s;
				s << "model=" << static_cast<int>(rep->getModelType())
				  << ";mode=" << static_cast<int>(rep->getDrawingMode())
				  << ";precision=" << static_cast<int>(rep->getDrawingPrecision())
				  << ";transparency=" << static_cast<int>(rep->getTransparency());
				return String(s.str());
			}
		}

		ModelController::ModelController(Representation* rep, QObject* parent)
			: Controller(parent),
				rep_(rep),
				model_type_(0),
				drawing_mode_(0),
				drawing_precision_(0),
				transparency_(0),
				ball_radius_(0.4f),
				ball_stick_bond_radius_(0.2f),
				dashed_bonds_(false),
				stick_radius_(0.2f),
				surface_probe_radius_(1.5f),
				cartoon_tube_radius_(0.4f),
				params_dirty_(false)
		{
			revert();
		}

		ModelController::~ModelController() = default;

		void ModelController::setRepresentation(Representation* rep)
		{
			if (rep_ == rep) return;
			rep_ = rep;
			revert();
		}

		void ModelController::revert()
		{
			if (rep_ == nullptr) return;
			int mt = static_cast<int>(rep_->getModelType());
			if (mt != model_type_) { model_type_ = mt; Q_EMIT modelTypeChanged(mt); }
			int dm = static_cast<int>(rep_->getDrawingMode());
			if (dm != drawing_mode_) { drawing_mode_ = dm; Q_EMIT drawingModeChanged(dm); }
			int dp = static_cast<int>(rep_->getDrawingPrecision());
			if (dp != drawing_precision_) { drawing_precision_ = dp; Q_EMIT drawingPrecisionChanged(dp); }
			int tr = static_cast<int>(rep_->getTransparency());
			if (tr != transparency_) { transparency_ = tr; Q_EMIT transparencyChanged(tr); }

			// Phase 999.57 Plan 03: the per-type advanced parameters
			// (ball_radius_, stick radii, dashed bonds, surface probe radius,
			// cartoon tube radius) are now owned by the controller itself —
			// they are no longer mirrored from DisplayProperties'
			// ModelSettingsDialog. revert() therefore re-syncs only the
			// Representation-derived model/drawing/transparency state above and
			// leaves the controller's own mirrored params untouched (they are
			// the source of truth for the headless ModelProcessorFactory path).

			params_dirty_ = false;
		}

		void ModelController::reset()
		{
			// §2 reset path (999.64 consumes) — re-sync the mirror from the
			// owner in a single pass. revert() already does exactly this.
			revert();
		}

		bool ModelController::apply()
		{
			// ── 1. Preconditions (§2 step 1) — fail-fast, return false ───────
			if (rep_ == nullptr)
			{
				Log.warn() << "[ModelController::apply] no Representation attached — skipping." << std::endl;
				return false;                    // rejected — nothing to mutate
			}

			// Busy guard — mirrors the legacy modal's "disable Apply
			// while busy" pattern (see MainControl::isBusy). If the
			// renderer or another modal is doing work, defer rather
			// than racing the scene mutation. Treated as a soft rejection.
			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[ModelController::apply] MainControl busy — deferring." << std::endl;
				return false;                    // rejected — busy
			}

			// ── 2. Re-entrancy shield (§2 step 2 / §4) ───────────────────────
			// If a notification triggered by this apply() (the §2a
			// invalidation refresh below) synchronously re-enters apply(),
			// DROP it rather than re-running the mutation — this is the cascade
			// class behind the v1.7.x-13 freeze. Nest-aware: depth > 0 ⇒ drop.
			if (applying_depth_ > 0) return false;          // dropped — re-entry
			ControllerApplyGuard apply_guard(applying_depth_);

			// ── 3. Capture reversible intent (§2 step 3) ─────────────────────
			ApplyPayload payload;
			payload.command_id = "model.setType";
			payload.before     = snapshotModelState_(rep_);
			payload.target     = rep_;
			payload.t_us       = PreciseTime::now().getMicroSeconds();

			// ── 4. Mutate the single owner (§2 step 4) ───────────────────────
			applyInternal_();

			payload.after = snapshotModelState_(rep_);

			// ── 5. Emit ONE typed event (§2 step 5) ──────────────────────────
			Q_EMIT appliedStub();

			// ── 6. Request the DECLARED invalidation (§2 step 6 / §2a) ───────
			// Model declares a display-list REBUILD — see invalidateDeclared_().
			invalidateDeclared_();

			// ── 7. Record reversible intent (§2 step 7) — capture only ───────
			recordIntent_(payload);

			return true;                          // mutated
			// guard destructor releases the re-entrancy block on scope exit
		}

		bool ModelController::applyInternal_()
		{
			// §13 cookbook step 2 — the existing mutation body, moved here
			// from apply(). Precondition (rep_ != nullptr) is guaranteed by
			// apply(). Does NOT request invalidation; that is the declared
			// §2a hook invalidateDeclared_(), called by apply() after this.
			ModelType new_type = static_cast<ModelType>(model_type_);
			DrawingMode new_mode = static_cast<DrawingMode>(drawing_mode_);
			DrawingPrecision new_precision = static_cast<DrawingPrecision>(drawing_precision_);

			bool model_type_changed = (rep_->getModelType() != new_type);

			// Phase 999.57 Plan 03: construct the model processor through the
			// headless ModelProcessorFactory, parameterized by the controller's
			// own mirrored fields — no longer reaching through DisplayProperties'
			// ModelSettingsDialog. ModelProcessorFactory::create bakes the params
			// into the processor at creation time, so a per-type PARAM change
			// (params_dirty_) with the same model type also requires recreating
			// the processor.
			//
			// The params the controller does NOT mirror (cartoon helix/arrow/
			// strand, DNA radii, force params, HBond radius, VDW factor, backbone
			// tube) fall back to ModelProcessorParams' struct defaults, which
			// equal the legacy dialog defaults (Plan 01) — so this is behavior-
			// preserving for every knob the controller never exposed.
			if (model_type_changed ||
			    rep_->getModelProcessor() == nullptr ||
			    params_dirty_)
			{
				ModelProcessorParams params;
				params.ball_radius                 = ball_radius_;
				params.ball_and_stick_stick_radius = ball_stick_bond_radius_;
				params.ball_and_stick_dashed_bonds_enabled = dashed_bonds_;
				params.stick_stick_radius          = stick_radius_;
				params.surface_probe_radius        = surface_probe_radius_;
				params.cartoon_tube_radius         = cartoon_tube_radius_;

				ModelProcessor* mp = ModelProcessorFactory::create(new_type, params);
				// §3c — Representation mutation flows through RepresentationBuilder.
				if (mp != nullptr)
				{
					RepresentationBuilder::setModelProcessor(*rep_, mp);
				}
				RepresentationBuilder::setModelType(*rep_, new_type);
			}

			RepresentationBuilder::setDrawingMode(*rep_, new_mode);
			RepresentationBuilder::setDrawingPrecision(*rep_, new_precision);

			// Surface models use a continuous precision; mirror the
			// legacy applyModelSettings_() mapping via the public
			// SurfaceDrawingPrecisions table.
			if (new_type == MODEL_SE_SURFACE || new_type == MODEL_SA_SURFACE)
			{
				int idx = drawing_precision_;
				if (idx < 0) idx = 0;
				if (idx > 3) idx = 3;
				RepresentationBuilder::setSurfaceDrawingPrecision(*rep_, SurfaceDrawingPrecisions[idx]);
			}

			RepresentationBuilder::setTransparency(*rep_, static_cast<Size>(transparency_));

			params_dirty_ = false;
			return true;
		}

		void ModelController::invalidateDeclared_()
		{
			// §2a Model row — display-list REBUILD. Model is the ONE
			// Representation-domain controller that rebuilds: swapping the
			// model processor (e.g. Stick → Surface) changes the primitive set
			// itself, so a soft refresh (update(false)) would re-draw stale
			// geometry. update(true) re-walks the composites through the model
			// processor. NEVER a blanket full-scene rebuild (the v1.7.x-13
			// freeze) — this is scoped to the one Representation we mutated.
			if (rep_ != nullptr) rep_->update(true);
		}


		void ModelController::setModelType(int t)
		{
			if (t == model_type_) return;
			model_type_ = t;
			Q_EMIT modelTypeChanged(t);
		}

		void ModelController::setDrawingMode(int m)
		{
			if (m == drawing_mode_) return;
			drawing_mode_ = m;
			Q_EMIT drawingModeChanged(m);
		}

		void ModelController::setDrawingPrecision(int p)
		{
			if (p == drawing_precision_) return;
			drawing_precision_ = p;
			Q_EMIT drawingPrecisionChanged(p);
		}

		void ModelController::setTransparency(int t)
		{
			if (t == transparency_) return;
			transparency_ = t;
			Q_EMIT transparencyChanged(t);
		}

		void ModelController::setBallRadius(float v)
		{
			if (v == ball_radius_) return;
			ball_radius_ = v;
			params_dirty_ = true;
			Q_EMIT ballRadiusChanged(v);
		}

		void ModelController::setBallStickBondRadius(float v)
		{
			if (v == ball_stick_bond_radius_) return;
			ball_stick_bond_radius_ = v;
			params_dirty_ = true;
			Q_EMIT ballStickBondRadiusChanged(v);
		}

		void ModelController::setDashedBonds(bool v)
		{
			if (v == dashed_bonds_) return;
			dashed_bonds_ = v;
			params_dirty_ = true;
			Q_EMIT dashedBondsChanged(v);
		}

		void ModelController::setStickRadius(float v)
		{
			if (v == stick_radius_) return;
			stick_radius_ = v;
			params_dirty_ = true;
			Q_EMIT stickRadiusChanged(v);
		}

		void ModelController::setSurfaceProbeRadius(float v)
		{
			if (v == surface_probe_radius_) return;
			surface_probe_radius_ = v;
			params_dirty_ = true;
			Q_EMIT surfaceProbeRadiusChanged(v);
		}

		void ModelController::setCartoonTubeRadius(float v)
		{
			if (v == cartoon_tube_radius_) return;
			cartoon_tube_radius_ = v;
			params_dirty_ = true;
			Q_EMIT cartoonTubeRadiusChanged(v);
		}

	} // namespace VIEW
} // namespace BALL

