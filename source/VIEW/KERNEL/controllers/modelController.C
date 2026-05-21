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
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{

		ModelController::ModelController(Representation* rep, QObject* parent)
			: QObject(parent),
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
				params_dirty_(false),
				applying_(false)
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

		void ModelController::apply()
		{
			if (rep_ == nullptr)
			{
				Log.warn() << "[ModelController::apply] no Representation attached — skipping." << std::endl;
				return;
			}

			// Busy guard — mirrors the legacy modal's "disable Apply
			// while busy" pattern (see MainControl::isBusy). If the
			// renderer or another modal is doing work, defer rather
			// than racing the scene mutation.
			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[ModelController::apply] MainControl busy — deferring." << std::endl;
				return;
			}

			// v1.7.x-24 — re-entrancy shield. If a notification triggered by
			// this apply() (e.g. the Representation::update() refresh below)
			// synchronously re-enters apply(), bail rather than re-running the
			// mutation — this is the cascade class behind the v1.7.x-13 freeze.
			// The RAII guard clears the flag on every exit path.
			if (applying_) return;
			ControllerApplyGuard apply_guard(applying_);

			ModelType new_type = static_cast<ModelType>(model_type_);
			DrawingMode new_mode = static_cast<DrawingMode>(drawing_mode_);
			DrawingPrecision new_precision = static_cast<DrawingPrecision>(drawing_precision_);

			bool model_type_changed = (rep_->getModelType() != new_type);
			bool rebuild =
				model_type_changed ||
				(rep_->getModelProcessor() == nullptr) ||
				(rep_->getDrawingPrecision() != new_precision) ||
				params_dirty_;

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
				if (mp != nullptr)
				{
					rep_->setModelProcessor(mp);
				}
				rep_->setModelType(new_type);
			}

			rep_->setDrawingMode(new_mode);
			rep_->setDrawingPrecision(new_precision);

			// Surface models use a continuous precision; mirror the
			// legacy applyModelSettings_() mapping via the public
			// SurfaceDrawingPrecisions table.
			if (new_type == MODEL_SE_SURFACE || new_type == MODEL_SA_SURFACE)
			{
				int idx = drawing_precision_;
				if (idx < 0) idx = 0;
				if (idx > 3) idx = 3;
				rep_->setSurfaceDrawingPrecision(SurfaceDrawingPrecisions[idx]);
			}

			rep_->setTransparency(static_cast<Size>(transparency_));

			// Push to the scene. rebuild=true forces the model
			// processor to re-walk the composites; rebuild=false is the
			// cheap path for transparency/drawing-mode-only changes.
			rep_->update(rebuild);

			params_dirty_ = false;

			Q_EMIT appliedStub();
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

