// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.51 Plan 02 — LabelSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/labelSection.h>


#include <BALL/VIEW/KERNEL/controllers/labelController.h>
#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/WIDGETS/labeledSlider.h>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			constexpr int DEBOUNCE_MS = 100;

			// Font-size slider bounds (point size). Mirrors the legacy
			// LabelDialog font default (Helvetica 12) which seeds the
			// controller's font_size_.
			constexpr int FONT_SIZE_MIN = 6;
			constexpr int FONT_SIZE_MAX = 48;
		}

		LabelSection::LabelSection(LabelController* controller, QWidget* parent)
			: InspectorSection(tr("Label"),
			                   QStringLiteral("Representation/Label"),
			                   parent),
				controller_(controller),
				text_(nullptr), mode_(nullptr), font_size_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			text_ = new QLineEdit(content);
			text_->setText(controller_ ? controller_->labelText() : QString());

			// Mode items map 1:1 to LabelController's int labelType, which in
			// turn mirrors the four LabelModel::Mode enum values in order:
			// 0=ALL_ITEMS, 1=ALL_ATOMS, 2=ALL_RESIDUES, 3=ONE_LABEL. The
			// controller's apply() (Task 2) maps the int back to the enum.
			mode_ = new QComboBox(content);
			mode_->addItem(tr("All Items"));
			mode_->addItem(tr("All Atoms"));
			mode_->addItem(tr("All Residues"));
			mode_->addItem(tr("One Label"));
			mode_->setCurrentIndex(controller_ ? controller_->labelType() : 0);

			font_size_ = new LabeledSlider(FONT_SIZE_MIN, FONT_SIZE_MAX,
				controller_ ? controller_->fontSize() : 12,
				QStringLiteral("pt"), content);

			col->addWidget(new FormRow(tr("Text"),      text_,      content));
			col->addWidget(new FormRow(tr("Mode"),      mode_,      content));
			col->addWidget(new FormRow(tr("Font Size"), font_size_, content));

			setContent(content);

			debounce_.setSingleShot(true);
			debounce_.setInterval(DEBOUNCE_MS);
			connect(&debounce_, &QTimer::timeout, this, &LabelSection::onDebounceFire_);

			connect(text_, &QLineEdit::textChanged,
			        this, &LabelSection::onTextChanged_);
			connect(mode_, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
			        this, &LabelSection::onModeChanged_);
			connect(font_size_, &LabeledSlider::valueChanged,
			        this, &LabelSection::onFontSizeChanged_);

			if (controller_)
			{
				connect(controller_, &LabelController::labelTextChanged,
				        this, &LabelSection::onControllerTextChanged_);
				connect(controller_, &LabelController::labelTypeChanged,
				        this, &LabelSection::onControllerModeChanged_);
				connect(controller_, &LabelController::fontSizeChanged,
				        this, &LabelSection::onControllerFontSizeChanged_);

				// 999.64 — per-section reset. The single-call Controller::reset()
				// sets the method-defined label defaults (which re-emit the
				// *Changed signals → the widgets resync) and runs ONE §2 apply()
				// (one event, one reversible payload), replacing the legacy
				// two-step revert(); apply().
				setResettable(true, tr("Reset Label settings to defaults?"));
				connect(this, &InspectorSection::resetRequested, this, [this]() {
					if (controller_) controller_->reset();
				});
			}
		}

		LabelSection::~LabelSection()
		{
			// v1.7-RC1 I-1 — flush pending debounced edit on destruction.
			if (debounce_.isActive())
			{
				debounce_.stop();
				onDebounceFire_();
			}
		}

		void LabelSection::onTextChanged_(const QString& t)
		{
			if (controller_) controller_->setLabelText(t);
			debounce_.start();
		}
		void LabelSection::onModeChanged_(int index)
		{
			if (controller_) controller_->setLabelType(index);
			debounce_.start();
		}
		void LabelSection::onFontSizeChanged_(int v)
		{
			if (controller_) controller_->setFontSize(v);
			debounce_.start();
		}

		void LabelSection::onControllerTextChanged_(const QString& t)
		{
			if (text_->text() != t) text_->setText(t);
		}
		void LabelSection::onControllerModeChanged_(int t)
		{
			if (mode_->currentIndex() != t) mode_->setCurrentIndex(t);
		}
		void LabelSection::onControllerFontSizeChanged_(int s)
		{
			if (font_size_->value() != s) font_size_->setValue(s);
		}

		void LabelSection::onDebounceFire_()
		{
			if (controller_) controller_->apply();
		}

	} // namespace VIEW
} // namespace BALL
