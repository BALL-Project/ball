// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — LightController implementation.
//

#include <BALL/VIEW/KERNEL/controllers/lightController.h>


#include <BALL/VIEW/KERNEL/stage.h>
#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{

		LightController::LightController(Stage* stage, QObject* parent)
			: QObject(parent), stage_(stage),
				light_count_(0), ambient_intensity_(0.3f)
		{
			revert();
		}

		LightController::~LightController() = default;

		void LightController::setStage(Stage* stage)
		{
			if (stage_ == stage) return;
			stage_ = stage;
			revert();
		}

		void LightController::revert()
		{
			if (stage_ == nullptr) return;
			int n = static_cast<int>(stage_->getLightSources().size());
			if (n != light_count_)
			{
				light_count_ = n;
				Q_EMIT lightCountChanged(n);
			}
			// Ambient intensity not directly exposed on Stage — kept as
			// controller-side default until cut-over wires it through
			// the Renderer/RenderSurface boundary (SEED-001 step 5).
		}

		void LightController::apply()
		{
			Log.info() << "[LightController::apply] STUB — legacy LightSettings "
				"owns mutation. lightCount=" << light_count_
				<< " ambient=" << ambient_intensity_ << std::endl;
			Q_EMIT appliedStub();
		}

		void LightController::setAmbientIntensity(float v)
		{
			if (v == ambient_intensity_) return;
			ambient_intensity_ = v;
			Q_EMIT ambientIntensityChanged(v);
		}

	} // namespace VIEW
} // namespace BALL

