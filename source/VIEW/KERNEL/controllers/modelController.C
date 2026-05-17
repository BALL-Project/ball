// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ModelController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/modelController.h>


#include <BALL/VIEW/KERNEL/representation.h>
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
			// TODO(999.44-RC-patch): mirror ModelSettingsDialog::apply().
			// During the migration window the legacy dialog still owns
			// the actual mutation path.
			Log.info() << "[ModelController::apply] STUB — legacy ModelSettingsDialog "
				"owns mutation. Mirrored values: modelType=" << model_type_
				<< " drawingMode=" << drawing_mode_
				<< " precision=" << drawing_precision_
				<< " transparency=" << transparency_ << std::endl;
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

