// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — MaterialSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/materialSection.h>


#include <BALL/VIEW/KERNEL/controllers/materialController.h>
#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/WIDGETS/labeledSlider.h>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			constexpr int DEBOUNCE_MS = 100;

			LabeledSlider* makePercentSlider_(QWidget* parent, int init)
			{
				return new LabeledSlider(0, 100, init,
				                         QStringLiteral("%"), parent);
			}
		}

		MaterialSection::MaterialSection(MaterialController* controller, QWidget* parent)
			: InspectorSection(tr("Material"),
			                   QStringLiteral("Representation/Material"),
			                   parent),
				controller_(controller),
				ambient_(nullptr), diffuse_(nullptr),
				specular_(nullptr), shininess_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			ambient_   = makePercentSlider_(content,
				controller_ ? static_cast<int>(controller_->ambientFactor() * 100.0f) : 30);
			diffuse_   = makePercentSlider_(content,
				controller_ ? static_cast<int>(controller_->diffuseFactor() * 100.0f) : 70);
			specular_  = makePercentSlider_(content,
				controller_ ? static_cast<int>(controller_->specularFactor() * 100.0f) : 20);
			shininess_ = new LabeledSlider(0, 128,
				controller_ ? static_cast<int>(controller_->shininess()) : 30,
				QStringLiteral(""), content);

			col->addWidget(new FormRow(tr("Ambient"),   ambient_,   content));
			col->addWidget(new FormRow(tr("Diffuse"),   diffuse_,   content));
			col->addWidget(new FormRow(tr("Specular"),  specular_,  content));
			col->addWidget(new FormRow(tr("Shininess"), shininess_, content));

			setContent(content);

			debounce_.setSingleShot(true);
			debounce_.setInterval(DEBOUNCE_MS);
			connect(&debounce_, &QTimer::timeout, this, &MaterialSection::onDebounceFire_);

			connect(ambient_,   &LabeledSlider::valueChanged, this, &MaterialSection::onAmbientChanged_);
			connect(diffuse_,   &LabeledSlider::valueChanged, this, &MaterialSection::onDiffuseChanged_);
			connect(specular_,  &LabeledSlider::valueChanged, this, &MaterialSection::onSpecularChanged_);
			connect(shininess_, &LabeledSlider::valueChanged, this, &MaterialSection::onShininessChanged_);

			if (controller_)
			{
				connect(controller_, &MaterialController::ambientFactorChanged,
				        this, &MaterialSection::onControllerAmbientChanged_);
				connect(controller_, &MaterialController::diffuseFactorChanged,
				        this, &MaterialSection::onControllerDiffuseChanged_);
				connect(controller_, &MaterialController::specularFactorChanged,
				        this, &MaterialSection::onControllerSpecularChanged_);
				connect(controller_, &MaterialController::shininessChanged,
				        this, &MaterialSection::onControllerShininessChanged_);

				// v1.7.x-18 — per-section reset. revert() re-reads the
				// live Representation material into the controller (which
				// re-emits the *Changed signals → the sliders resync), then
				// apply() pushes it back through the scene-material backend.
				setResettable(true, tr("Reset Material settings to the "
				                       "representation's current values?"));
				connect(this, &InspectorSection::resetRequested, this, [this]() {
					if (controller_) { controller_->revert(); controller_->apply(); }
				});
			}
		}

		MaterialSection::~MaterialSection()
		{
			// v1.7-RC1 I-1 — flush pending debounced edit on destruction.
			if (debounce_.isActive())
			{
				debounce_.stop();
				onDebounceFire_();
			}
		}

		void MaterialSection::onAmbientChanged_(int v)
		{
			if (controller_) controller_->setAmbientFactor(v / 100.0f);
			debounce_.start();
		}
		void MaterialSection::onDiffuseChanged_(int v)
		{
			if (controller_) controller_->setDiffuseFactor(v / 100.0f);
			debounce_.start();
		}
		void MaterialSection::onSpecularChanged_(int v)
		{
			if (controller_) controller_->setSpecularFactor(v / 100.0f);
			debounce_.start();
		}
		void MaterialSection::onShininessChanged_(int v)
		{
			if (controller_) controller_->setShininess(static_cast<float>(v));
			debounce_.start();
		}

		void MaterialSection::onControllerAmbientChanged_(float v)
		{
			int iv = static_cast<int>(v * 100.0f);
			if (ambient_->value() != iv) ambient_->setValue(iv);
		}
		void MaterialSection::onControllerDiffuseChanged_(float v)
		{
			int iv = static_cast<int>(v * 100.0f);
			if (diffuse_->value() != iv) diffuse_->setValue(iv);
		}
		void MaterialSection::onControllerSpecularChanged_(float v)
		{
			int iv = static_cast<int>(v * 100.0f);
			if (specular_->value() != iv) specular_->setValue(iv);
		}
		void MaterialSection::onControllerShininessChanged_(float v)
		{
			int iv = static_cast<int>(v);
			if (shininess_->value() != iv) shininess_->setValue(iv);
		}

		void MaterialSection::onDebounceFire_()
		{
			if (controller_) controller_->apply();
		}

	} // namespace VIEW
} // namespace BALL

