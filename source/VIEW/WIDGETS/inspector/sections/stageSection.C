// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — StageSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/stageSection.h>

#ifdef BALL_UI_V2

#include <BALL/VIEW/KERNEL/controllers/stageController.h>
#include <BALL/VIEW/WIDGETS/swatchButton.h>
#include <BALL/VIEW/WIDGETS/labeledSlider.h>
#include <BALL/VIEW/WIDGETS/formRow.h>

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			constexpr int DEBOUNCE_MS = 100;  // Handover §04 live-commit window
		}

		StageSection::StageSection(StageController* controller, QWidget* parent)
			: InspectorSection(tr("Stage"),
			                   QStringLiteral("Scene/Stage"),
			                   parent),
				controller_(controller),
				bg_swatch_(nullptr),
				show_coord_(nullptr),
				fog_slider_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			// Background colour ----------------------------------------------------
			bg_swatch_ = new SwatchButton(
				controller_ ? controller_->backgroundColor() : QColor(Qt::black),
				content);
			col->addWidget(new FormRow(tr("Background"), bg_swatch_, content));

			// Show coordinate system ----------------------------------------------
			show_coord_ = new QCheckBox(content);
			if (controller_)
				show_coord_->setChecked(controller_->showCoordinateSystem());
			col->addWidget(new FormRow(tr("Show coordinate system"), show_coord_, content));

			// Fog intensity (stand-in slider; the legacy dialog has many more
			// controls — they come online when the Controller cuts over) ----------
			int fog_init = controller_
				? static_cast<int>(controller_->fogIntensity() * 100.0f)
				: 0;
			fog_slider_ = new LabeledSlider(0, 100, fog_init, QStringLiteral("%"), content);
			col->addWidget(new FormRow(tr("Fog intensity"), fog_slider_, content));

			setContent(content);

			// Debounce timer -------------------------------------------------------
			debounce_.setSingleShot(true);
			debounce_.setInterval(DEBOUNCE_MS);
			connect(&debounce_, &QTimer::timeout,
			        this, &StageSection::onDebounceFire_);

			// User-edit → controller (debounced) -----------------------------------
			connect(bg_swatch_, &SwatchButton::colorChanged,
			        this, &StageSection::onBackgroundColorChosen_);
			connect(show_coord_, &QCheckBox::toggled,
			        this, &StageSection::onCoordinateSystemToggled_);
			connect(fog_slider_, &LabeledSlider::valueChanged,
			        this, &StageSection::onFogIntensityChanged_);

			// Controller → widget (no debounce; immediate echo) --------------------
			if (controller_)
			{
				connect(controller_, &StageController::backgroundColorChanged,
				        this, &StageSection::onControllerBackgroundColorChanged_);
				connect(controller_, &StageController::showCoordinateSystemChanged,
				        this, &StageSection::onControllerCoordinateSystemChanged_);
				connect(controller_, &StageController::fogIntensityChanged,
				        this, &StageSection::onControllerFogIntensityChanged_);
			}
		}

		StageSection::~StageSection() = default;

		void StageSection::scheduleApply_()
		{
			// Restart on every edit — drag streams collapse to 1 apply() ~100ms
			// after the last move.
			debounce_.start();
		}

		void StageSection::onBackgroundColorChosen_(const QColor& c)
		{
			if (controller_)
				controller_->setBackgroundColor(c);
			scheduleApply_();
		}

		void StageSection::onCoordinateSystemToggled_(bool checked)
		{
			if (controller_)
				controller_->setShowCoordinateSystem(checked);
			scheduleApply_();
		}

		void StageSection::onFogIntensityChanged_(int v)
		{
			if (controller_)
				controller_->setFogIntensity(static_cast<float>(v) / 100.0f);
			scheduleApply_();
		}

		void StageSection::onControllerBackgroundColorChanged_(const QColor& c)
		{
			if (bg_swatch_ && bg_swatch_->color() != c)
				bg_swatch_->setColor(c);
		}

		void StageSection::onControllerCoordinateSystemChanged_(bool b)
		{
			if (show_coord_ && show_coord_->isChecked() != b)
			{
				QSignalBlocker block(show_coord_);
				show_coord_->setChecked(b);
			}
		}

		void StageSection::onControllerFogIntensityChanged_(float v)
		{
			int iv = static_cast<int>(v * 100.0f);
			if (fog_slider_ && fog_slider_->value() != iv)
				fog_slider_->setValue(iv);
		}

		void StageSection::onDebounceFire_()
		{
			if (controller_)
				controller_->apply();
		}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
