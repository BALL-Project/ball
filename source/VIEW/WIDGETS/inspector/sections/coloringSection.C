// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — ColoringSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/coloringSection.h>


#include <BALL/VIEW/KERNEL/controllers/coloringController.h>
#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtCore/QSignalBlocker>
#include <QtWidgets/QComboBox>
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
				method_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			method_ = new QComboBox(content);
			populateMethods_(method_);
			col->addWidget(new FormRow(tr("Method"), method_, content));

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

			if (controller_)
			{
				connect(controller_, &ColoringController::coloringMethodChanged,
				        this, &ColoringSection::onControllerMethodChanged_);
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

		void ColoringSection::onMethodChosen_(int idx)
		{
			if (idx < 0 || !controller_) return;
			controller_->setColoringMethod(method_->itemData(idx).toInt());
			debounce_.start();
		}

		void ColoringSection::onControllerMethodChanged_(int m)
		{
			int i = findIndexForData_(method_, m);
			if (i >= 0 && method_->currentIndex() != i)
			{
				QSignalBlocker b(method_);
				method_->setCurrentIndex(i);
			}
		}

		void ColoringSection::onDebounceFire_()
		{
			if (controller_) controller_->apply();
		}

	} // namespace VIEW
} // namespace BALL

