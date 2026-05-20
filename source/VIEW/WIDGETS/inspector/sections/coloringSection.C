// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/coloringSection.h>


#include <BALL/VIEW/KERNEL/controllers/coloringController.h>
#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/WIDGETS/labeledSlider.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtCore/QSignalBlocker>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			constexpr int DEBOUNCE_MS = 100;

			void populateMethods_(QComboBox* c)
			{
				c->addItem(QStringLiteral("Element"),             COLORING_ELEMENT);
				c->addItem(QStringLiteral("Residue index"),       COLORING_RESIDUE_INDEX);
				c->addItem(QStringLiteral("Residue name"),        COLORING_RESIDUE_NAME);
				c->addItem(QStringLiteral("Secondary structure"), COLORING_SECONDARY_STRUCTURE);
				c->addItem(QStringLiteral("Atom charge"),         COLORING_ATOM_CHARGE);
				c->addItem(QStringLiteral("Distance"),            COLORING_DISTANCE);
				c->addItem(QStringLiteral("Temperature factor"),  COLORING_TEMPERATURE_FACTOR);
				c->addItem(QStringLiteral("Occupancy"),           COLORING_OCCUPANCY);
				c->addItem(QStringLiteral("Forces"),              COLORING_FORCES);
				c->addItem(QStringLiteral("Residue type"),        COLORING_RESIDUE_TYPE);
				c->addItem(QStringLiteral("Chain"),               COLORING_CHAIN);
				c->addItem(QStringLiteral("Molecule"),            COLORING_MOLECULE);
				c->addItem(QStringLiteral("Custom"),              COLORING_CUSTOM);
			}

			int findIndexForData_(QComboBox* c, int value)
			{
				for (int i = 0; i < c->count(); ++i)
					if (c->itemData(i).toInt() == value) return i;
				return -1;
			}
		}

		ColoringSection::ColoringSection(ColoringController* controller, QWidget* parent)
			: InspectorSection(tr("Coloring"),
			                   QStringLiteral("Representation/Coloring"),
			                   parent),
				controller_(controller),
				method_(nullptr),
				type_options_(nullptr),
				value_min_(nullptr), value_max_(nullptr),
				page_empty_(0), page_value_range_(0)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			method_ = new QComboBox(content);
			populateMethods_(method_);
			col->addWidget(new FormRow(tr("Method"), method_, content));

			// v1.7.x-17 — per-method options. One QStackedWidget page per
			// option group; an empty page for methods without extra
			// options. The visible page tracks the selected method.
			type_options_ = new QStackedWidget(content);

			// Empty page (default for methods without extra options).
			QWidget* empty_page = new QWidget(type_options_);
			page_empty_ = type_options_->addWidget(empty_page);

			// Value-range page (shared by the value-based methods).
			// Float value <-> int slider mapped by *10 / /10.0f over
			// range 0..1000 (=> 0.0–100.0 in 0.1 steps).
			{
				QWidget* page = new QWidget(type_options_);
				QVBoxLayout* pcol = new QVBoxLayout(page);
				pcol->setContentsMargins(0, 0, 0, 0);
				pcol->setSpacing(6);
				value_min_ = new LabeledSlider(0, 1000,
					static_cast<int>((controller_ ? controller_->valueMin() : 0.0f) * 10.0f),
					QStringLiteral(""), page);
				value_max_ = new LabeledSlider(0, 1000,
					static_cast<int>((controller_ ? controller_->valueMax() : 100.0f) * 10.0f),
					QStringLiteral(""), page);
				pcol->addWidget(new FormRow(tr("Min value"), value_min_, page));
				pcol->addWidget(new FormRow(tr("Max value"), value_max_, page));
				page_value_range_ = type_options_->addWidget(page);
			}

			col->addWidget(type_options_);

			setContent(content);

			debounce_.setSingleShot(true);
			debounce_.setInterval(DEBOUNCE_MS);
			connect(&debounce_, &QTimer::timeout, this, &ColoringSection::onDebounceFire_);

			if (controller_)
			{
				int i = findIndexForData_(method_, controller_->coloringMethod());
				if (i >= 0) method_->setCurrentIndex(i);
			}

			connect(method_,
			        QOverload<int>::of(&QComboBox::currentIndexChanged),
			        this, &ColoringSection::onMethodChosen_);

			connect(value_min_, &LabeledSlider::valueChanged,
			        this, &ColoringSection::onValueMinChanged_);
			connect(value_max_, &LabeledSlider::valueChanged,
			        this, &ColoringSection::onValueMaxChanged_);

			// Show the page matching the initial coloring method.
			showPageForMethod_(controller_ ? controller_->coloringMethod()
			                                : method_->currentData().toInt());

			if (controller_)
			{
				connect(controller_, &ColoringController::coloringMethodChanged,
				        this, &ColoringSection::onControllerMethodChanged_);
				connect(controller_, &ColoringController::valueMinChanged,
				        this, &ColoringSection::onControllerValueMinChanged_);
				connect(controller_, &ColoringController::valueMaxChanged,
				        this, &ColoringSection::onControllerValueMaxChanged_);

				// v1.7.x-18 — per-section reset.
				setResettable(true, tr("Reset Coloring settings to the "
				                       "representation's current values?"));
				connect(this, &InspectorSection::resetRequested, this, [this]() {
					if (controller_) { controller_->revert(); controller_->apply(); }
				});
			}
		}

		ColoringSection::~ColoringSection()
		{
			// v1.7-RC1 I-1 — flush pending debounced edit on destruction.
			if (debounce_.isActive())
			{
				debounce_.stop();
				onDebounceFire_();
			}
		}

		void ColoringSection::scheduleApply_() { debounce_.start(); }

		void ColoringSection::showPageForMethod_(int method)
		{
			if (!type_options_) return;
			int page = page_empty_;
			switch (method)
			{
				case COLORING_TEMPERATURE_FACTOR:
				case COLORING_OCCUPANCY:
				case COLORING_DISTANCE:
				case COLORING_FORCES:       page = page_value_range_; break;
				default:                    page = page_empty_;       break;
			}
			type_options_->setCurrentIndex(page);
		}

		void ColoringSection::onMethodChosen_(int idx)
		{
			if (idx < 0 || !controller_) return;
			int m = method_->itemData(idx).toInt();
			controller_->setColoringMethod(m);
			showPageForMethod_(m);
			scheduleApply_();
		}

		void ColoringSection::onValueMinChanged_(int v)
		{
			if (!controller_) return;
			controller_->setValueMin(v / 10.0f);
			scheduleApply_();
		}

		void ColoringSection::onValueMaxChanged_(int v)
		{
			if (!controller_) return;
			controller_->setValueMax(v / 10.0f);
			scheduleApply_();
		}

		void ColoringSection::onControllerMethodChanged_(int m)
		{
			int i = findIndexForData_(method_, m);
			if (i >= 0 && method_->currentIndex() != i)
			{
				QSignalBlocker b(method_);
				method_->setCurrentIndex(i);
			}
			showPageForMethod_(m);
		}

		void ColoringSection::onControllerValueMinChanged_(float v)
		{
			int iv = static_cast<int>(v * 10.0f);
			if (value_min_->value() != iv)
			{
				QSignalBlocker b(value_min_);
				value_min_->setValue(iv);
			}
		}

		void ColoringSection::onControllerValueMaxChanged_(float v)
		{
			int iv = static_cast<int>(v * 10.0f);
			if (value_max_->value() != iv)
			{
				QSignalBlocker b(value_max_);
				value_max_->setValue(iv);
			}
		}

		void ColoringSection::onDebounceFire_()
		{
			if (controller_) controller_->apply();
		}

	} // namespace VIEW
} // namespace BALL

