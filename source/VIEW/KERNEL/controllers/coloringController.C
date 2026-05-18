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
#include <BALL/COMMON/logStream.h>

namespace BALL
{
	namespace VIEW
	{

		ColoringController::ColoringController(Representation* rep, QObject* parent)
			: QObject(parent), rep_(rep), coloring_method_(0)
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

	} // namespace VIEW
} // namespace BALL

