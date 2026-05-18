// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ModelController implementation.
// UFG-03 cut-over (v1.7-modernization) — apply() now mutates the
// attached Representation through DisplayProperties' owned
// ModelSettingsDialog (for the per-ModelType processor factory) and
// the Representation's own setters. The legacy ModelSettingsDialog
// continues to *also* mutate via DisplayProperties::applyTo_ when the
// user opens the legacy modal; the Inspector path now writes through
// the same processor-factory so both UIs stay consistent.
//

#include <BALL/VIEW/KERNEL/controllers/modelController.h>


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/modelSettingsDialog.h>
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
				transparency_(0)
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

			ModelType new_type = static_cast<ModelType>(model_type_);
			DrawingMode new_mode = static_cast<DrawingMode>(drawing_mode_);
			DrawingPrecision new_precision = static_cast<DrawingPrecision>(drawing_precision_);

			bool model_type_changed = (rep_->getModelType() != new_type);
			bool rebuild =
				model_type_changed ||
				(rep_->getModelProcessor() == nullptr) ||
				(rep_->getDrawingPrecision() != new_precision);

			// If DisplayProperties is in the tree, reuse its
			// ModelSettingsDialog so per-model parameters (stick radius,
			// surface probe radius, ribbon mode, etc.) flow through the
			// same advanced-options state the legacy modal mutates.
			// Otherwise fall back to a stub processor swap; the renderer
			// will still pick up the model-type change.
			if (model_type_changed || rep_->getModelProcessor() == nullptr)
			{
				DisplayProperties* dp = DisplayProperties::getInstance(0);
				if (dp != nullptr && dp->getModelSettingsDialog() != nullptr)
				{
					ModelProcessor* mp = dp->getModelSettingsDialog()->createModelProcessor(new_type);
					if (mp != nullptr)
					{
						rep_->setModelProcessor(mp);
					}
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

	} // namespace VIEW
} // namespace BALL

