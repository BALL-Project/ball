// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.51 Plan 01 — ClippingSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/clippingSection.h>


#include <BALL/VIEW/KERNEL/controllers/clippingController.h>
#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/WIDGETS/labeledSlider.h>

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			constexpr int DEBOUNCE_MS = 100;

			// The controller stores offset_ as a float. The Inspector
			// exposes it as a percent slider (0..100) mirroring the
			// makePercentSlider_ helper in materialSection.C; 100% maps to
			// a unit offset along the plane normal.
			LabeledSlider* makePercentSlider_(QWidget* parent, int init)
			{
				return new LabeledSlider(0, 100, init,
				                         QStringLiteral("%"), parent);
			}
		}

		ClippingSection::ClippingSection(ClippingController* controller, QWidget* parent)
			: InspectorSection(tr("Clipping"),
			                   QStringLiteral("Representation/Clipping"),
			                   parent),
				controller_(controller),
				enabled_(nullptr), offset_(nullptr), capped_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			enabled_ = new QCheckBox(content);
			enabled_->setChecked(controller_ ? controller_->enabled() : false);

			offset_  = makePercentSlider_(content,
				controller_ ? static_cast<int>(controller_->offset() * 100.0f) : 0);

			capped_  = new QCheckBox(content);
			capped_->setChecked(controller_ ? controller_->capped() : true);

			col->addWidget(new FormRow(tr("Enabled"), enabled_, content));
			col->addWidget(new FormRow(tr("Offset"),  offset_,  content));
			col->addWidget(new FormRow(tr("Capped"),  capped_,  content));

			setContent(content);

			debounce_.setSingleShot(true);
			debounce_.setInterval(DEBOUNCE_MS);
			connect(&debounce_, &QTimer::timeout, this, &ClippingSection::onDebounceFire_);

			connect(enabled_, &QCheckBox::toggled,       this, &ClippingSection::onEnabledChanged_);
			connect(offset_,  &LabeledSlider::valueChanged, this, &ClippingSection::onOffsetChanged_);
			connect(capped_,  &QCheckBox::toggled,       this, &ClippingSection::onCappedChanged_);

			if (controller_)
			{
				connect(controller_, &ClippingController::enabledChanged,
				        this, &ClippingSection::onControllerEnabledChanged_);
				connect(controller_, &ClippingController::offsetChanged,
				        this, &ClippingSection::onControllerOffsetChanged_);
				connect(controller_, &ClippingController::cappedChanged,
				        this, &ClippingSection::onControllerCappedChanged_);

				// v1.7.x-18 — per-section reset. revert() re-reads the
				// live clipping state into the controller (which re-emits
				// the *Changed signals → the widgets resync), then apply()
				// pushes it back through the clipping-plane backend.
				setResettable(true, tr("Reset Clipping settings to the "
				                       "representation's current values?"));
				connect(this, &InspectorSection::resetRequested, this, [this]() {
					if (controller_) { controller_->revert(); controller_->apply(); }
				});
			}
		}

		ClippingSection::~ClippingSection()
		{
			// v1.7-RC1 I-1 — flush pending debounced edit on destruction.
			if (debounce_.isActive())
			{
				debounce_.stop();
				onDebounceFire_();
			}
		}

		void ClippingSection::onEnabledChanged_(bool b)
		{
			if (controller_) controller_->setEnabled(b);
			debounce_.start();
		}
		void ClippingSection::onOffsetChanged_(int v)
		{
			if (controller_) controller_->setOffset(v / 100.0f);
			debounce_.start();
		}
		void ClippingSection::onCappedChanged_(bool b)
		{
			if (controller_) controller_->setCapped(b);
			debounce_.start();
		}

		void ClippingSection::onControllerEnabledChanged_(bool b)
		{
			if (enabled_->isChecked() != b) enabled_->setChecked(b);
		}
		void ClippingSection::onControllerOffsetChanged_(float v)
		{
			int iv = static_cast<int>(v * 100.0f);
			if (offset_->value() != iv) offset_->setValue(iv);
		}
		void ClippingSection::onControllerCappedChanged_(bool b)
		{
			if (capped_->isChecked() != b) capped_->setChecked(b);
		}

		void ClippingSection::onDebounceFire_()
		{
			if (controller_) controller_->apply();
		}

	} // namespace VIEW
} // namespace BALL
