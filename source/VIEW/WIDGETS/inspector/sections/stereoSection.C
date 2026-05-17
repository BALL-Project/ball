// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — StereoSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/stereoSection.h>

#ifdef BALL_UI_V2

#include <BALL/VIEW/KERNEL/controllers/stereoController.h>
#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/WIDGETS/labeledSlider.h>

#include <QtCore/QSignalBlocker>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		StereoSection::StereoSection(StereoController* controller, QWidget* parent)
			: InspectorSection(tr("Stereo"),
			                   QStringLiteral("Scene/Stereo"),
			                   parent),
				controller_(controller),
				enabled_box_(nullptr),
				eye_slider_(nullptr),
				focal_slider_(nullptr),
				swap_box_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			enabled_box_  = new QCheckBox(content);
			eye_slider_   = new LabeledSlider(0, 200,
				controller_ ? static_cast<int>(controller_->eyeDistance() * 10.0f) : 0,
				QStringLiteral(""), content);
			focal_slider_ = new LabeledSlider(0, 1000,
				controller_ ? static_cast<int>(controller_->focalDistance() * 10.0f) : 0,
				QStringLiteral(""), content);
			swap_box_ = new QCheckBox(content);
			if (controller_)
			{
				enabled_box_->setChecked(controller_->stereoEnabled());
				swap_box_->setChecked(controller_->swapSideBySide());
			}

			col->addWidget(new FormRow(tr("Enabled"),     enabled_box_, content));
			col->addWidget(new FormRow(tr("Eye distance"),  eye_slider_,  content));
			col->addWidget(new FormRow(tr("Focal distance"),focal_slider_,content));
			col->addWidget(new FormRow(tr("Swap side-by-side"), swap_box_, content));

			setContent(content);

			debounce_.setSingleShot(true);
			debounce_.setInterval(100);
			connect(&debounce_, &QTimer::timeout, this, &StereoSection::onDebounceFire_);

			connect(enabled_box_,  &QCheckBox::toggled, this, &StereoSection::onEnabledToggled_);
			connect(eye_slider_,   &LabeledSlider::valueChanged, this, &StereoSection::onEyeChanged_);
			connect(focal_slider_, &LabeledSlider::valueChanged, this, &StereoSection::onFocalChanged_);
			connect(swap_box_,     &QCheckBox::toggled, this, &StereoSection::onSwapToggled_);

			if (controller_)
			{
				connect(controller_, &StereoController::stereoEnabledChanged,
				        this, &StereoSection::onControllerEnabledChanged_);
				connect(controller_, &StereoController::eyeDistanceChanged,
				        this, &StereoSection::onControllerEyeChanged_);
				connect(controller_, &StereoController::focalDistanceChanged,
				        this, &StereoSection::onControllerFocalChanged_);
				connect(controller_, &StereoController::swapSideBySideChanged,
				        this, &StereoSection::onControllerSwapChanged_);
			}
		}

		StereoSection::~StereoSection() = default;

		void StereoSection::onEnabledToggled_(bool b)
		{
			if (controller_) controller_->setStereoEnabled(b);
			debounce_.start();
		}
		void StereoSection::onEyeChanged_(int v)
		{
			if (controller_) controller_->setEyeDistance(v / 10.0f);
			debounce_.start();
		}
		void StereoSection::onFocalChanged_(int v)
		{
			if (controller_) controller_->setFocalDistance(v / 10.0f);
			debounce_.start();
		}
		void StereoSection::onSwapToggled_(bool b)
		{
			if (controller_) controller_->setSwapSideBySide(b);
			debounce_.start();
		}

		void StereoSection::onControllerEnabledChanged_(bool b)
		{
			if (enabled_box_->isChecked() != b)
			{
				QSignalBlocker block(enabled_box_);
				enabled_box_->setChecked(b);
			}
		}
		void StereoSection::onControllerEyeChanged_(float v)
		{
			int iv = static_cast<int>(v * 10.0f);
			if (eye_slider_->value() != iv) eye_slider_->setValue(iv);
		}
		void StereoSection::onControllerFocalChanged_(float v)
		{
			int iv = static_cast<int>(v * 10.0f);
			if (focal_slider_->value() != iv) focal_slider_->setValue(iv);
		}
		void StereoSection::onControllerSwapChanged_(bool b)
		{
			if (swap_box_->isChecked() != b)
			{
				QSignalBlocker block(swap_box_);
				swap_box_->setChecked(b);
			}
		}

		void StereoSection::onDebounceFire_()
		{
			if (controller_) controller_->apply();
		}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
