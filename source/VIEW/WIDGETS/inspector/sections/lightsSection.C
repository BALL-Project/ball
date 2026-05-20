// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — LightsSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/lightsSection.h>


#include <BALL/VIEW/KERNEL/controllers/lightController.h>
#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/WIDGETS/labeledSlider.h>

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		LightsSection::LightsSection(LightController* controller, QWidget* parent)
			: InspectorSection(tr("Lights"),
			                   QStringLiteral("Scene/Lights"),
			                   parent),
				controller_(controller),
				count_label_(nullptr),
				ambient_slider_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			count_label_ = new QLabel(
				QString::number(controller_ ? controller_->lightCount() : 0),
				content);
			ambient_slider_ = new LabeledSlider(0, 100,
				controller_ ? static_cast<int>(controller_->ambientIntensity() * 100.0f) : 30,
				QStringLiteral("%"), content);

			col->addWidget(new FormRow(tr("Active lights"), count_label_,    content));
			col->addWidget(new FormRow(tr("Ambient"),       ambient_slider_, content));

			setContent(content);

			debounce_.setSingleShot(true);
			debounce_.setInterval(100);
			connect(&debounce_, &QTimer::timeout, this, &LightsSection::onDebounceFire_);

			connect(ambient_slider_, &LabeledSlider::valueChanged,
			        this, &LightsSection::onAmbientChanged_);

			if (controller_)
			{
				connect(controller_, &LightController::lightCountChanged,
				        this, &LightsSection::onControllerLightCountChanged_);
				connect(controller_, &LightController::ambientIntensityChanged,
				        this, &LightsSection::onControllerAmbientChanged_);

				// v1.7.x-18 — per-section reset.
				setResettable(true, tr("Reset Lights settings to the "
				                       "scene's current values?"));
				connect(this, &InspectorSection::resetRequested, this, [this]() {
					if (controller_) { controller_->revert(); controller_->apply(); }
				});
			}
		}

		LightsSection::~LightsSection()
		{
			// v1.7-RC1 I-1 — flush pending debounced edit on destruction.
			if (debounce_.isActive())
			{
				debounce_.stop();
				onDebounceFire_();
			}
		}

		void LightsSection::onAmbientChanged_(int v)
		{
			if (controller_) controller_->setAmbientIntensity(v / 100.0f);
			debounce_.start();
		}

		void LightsSection::onControllerLightCountChanged_(int c)
		{
			count_label_->setText(QString::number(c));
		}

		void LightsSection::onControllerAmbientChanged_(float v)
		{
			int iv = static_cast<int>(v * 100.0f);
			if (ambient_slider_->value() != iv) ambient_slider_->setValue(iv);
		}

		void LightsSection::onDebounceFire_()
		{
			if (controller_) controller_->apply();
		}

	} // namespace VIEW
} // namespace BALL

