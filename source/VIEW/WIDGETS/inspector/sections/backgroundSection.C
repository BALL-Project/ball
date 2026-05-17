// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 05 — BackgroundSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/backgroundSection.h>


#include <BALL/VIEW/KERNEL/controllers/stageController.h>
#include <BALL/VIEW/WIDGETS/swatchButton.h>
#include <BALL/VIEW/WIDGETS/formRow.h>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		BackgroundSection::BackgroundSection(StageController* stage_controller,
		                                     QWidget* parent)
			: InspectorSection(tr("Background"),
			                   QStringLiteral("Scene/Background"),
			                   parent),
				controller_(stage_controller),
				swatch_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			swatch_ = new SwatchButton(
				controller_ ? controller_->backgroundColor() : QColor(Qt::black),
				content);
			col->addWidget(new FormRow(tr("Colour"), swatch_, content));

			setContent(content);

			debounce_.setSingleShot(true);
			debounce_.setInterval(100);
			connect(&debounce_, &QTimer::timeout, this, &BackgroundSection::onDebounceFire_);

			connect(swatch_, &SwatchButton::colorChanged,
			        this, &BackgroundSection::onColorPicked_);
			if (controller_)
			{
				connect(controller_, &StageController::backgroundColorChanged,
				        this, &BackgroundSection::onControllerColorChanged_);
			}
		}

		BackgroundSection::~BackgroundSection() = default;

		void BackgroundSection::onColorPicked_(const QColor& c)
		{
			if (controller_) controller_->setBackgroundColor(c);
			debounce_.start();
		}

		void BackgroundSection::onControllerColorChanged_(const QColor& c)
		{
			if (swatch_ && swatch_->color() != c) swatch_->setColor(c);
		}

		void BackgroundSection::onDebounceFire_()
		{
			if (controller_) controller_->apply();
		}

	} // namespace VIEW
} // namespace BALL

