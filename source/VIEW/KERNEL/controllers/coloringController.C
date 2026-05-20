// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringController implementation.
// UFG-03 cut-over (v1.7-modernization) — apply() now mutates the
// attached Representation through DisplayProperties' owned
// ColoringSettingsDialog (for the per-ColoringMethod processor
// factory) and the Representation's own setters.
//

#include <BALL/VIEW/KERNEL/controllers/coloringController.h>


#include <BALL/VIEW/KERNEL/representation.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/DIALOGS/displayProperties.h>
#include <BALL/VIEW/DIALOGS/coloringSettingsDialog.h>
#include <BALL/VIEW/MODELS/colorProcessor.h>
#include <BALL/VIEW/KERNEL/controllers/controllerApplyGuard.h>
#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{

		ColoringController::ColoringController(Representation* rep, QObject* parent)
			: QObject(parent), rep_(rep), coloring_method_(0),
				value_min_(0.0f), value_max_(100.0f),
				applying_(false)
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

		void ColoringController::apply()
		{
			if (rep_ == nullptr)
			{
				Log.warn() << "[ColoringController::apply] no Representation attached — skipping." << std::endl;
				return;
			}

			MainControl* mc = MainControl::getInstance(0);
			if (mc != nullptr && mc->isBusy())
			{
				Log.info() << "[ColoringController::apply] MainControl busy — deferring." << std::endl;
				return;
			}

			// v1.7.x-24 — re-entrancy shield. If a notification triggered by
			// this apply() (e.g. the Representation::update() refresh below)
			// synchronously re-enters apply(), bail rather than re-running the
			// mutation — this is the cascade class behind the v1.7.x-13 freeze.
			// The RAII guard clears the flag on every exit path.
			if (applying_) return;
			ControllerApplyGuard apply_guard(applying_);

			ColoringMethod new_method = static_cast<ColoringMethod>(coloring_method_);
			bool method_changed = (rep_->getColoringMethod() != new_method);

			if (method_changed || rep_->getColorProcessor() == nullptr)
			{
				DisplayProperties* dp = DisplayProperties::getInstance(0);
				ColorProcessor* cp = nullptr;
				if (dp != nullptr && dp->getColoringSettingsDialog() != nullptr)
				{
					cp = dp->getColoringSettingsDialog()->createColorProcessor(new_method);
				}
				if (cp == nullptr)
				{
					// Fallback when DisplayProperties hasn't been
					// initialised (no preferences tab yet) — at least
					// install a baseline processor so the renderer
					// has something to invoke.
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

			// Color-only update: pass rebuild=false so the model
			// processor isn't re-run. Representation::update with
			// rebuild=false still re-walks the color processor over
			// the existing geometry.
			rep_->update(false);

			Q_EMIT appliedStub();
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

