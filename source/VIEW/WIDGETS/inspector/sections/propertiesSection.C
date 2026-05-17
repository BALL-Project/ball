// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 03 — PropertiesSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/propertiesSection.h>

#ifdef BALL_UI_V2

#include <BALL/VIEW/WIDGETS/formRow.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/CONCEPT/composite.h>

#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			// Return the single Atom in the selection if (and only if)
			// the selection contains exactly one composite AND it IS an
			// Atom. Returns nullptr otherwise.
			Atom* singleAtomOrNull_(const std::list<Composite*>& selection)
			{
				if (selection.size() != 1) return nullptr;
				return dynamic_cast<Atom*>(*selection.begin());
			}
		}

		PropertiesSection::PropertiesSection(QWidget* parent)
			: InspectorSection(tr("Properties"),
			                   QStringLiteral("Selection/Properties"),
			                   parent),
				stack_(nullptr),
				placeholder_(nullptr),
				element_label_(nullptr),
				position_label_(nullptr),
				charge_label_(nullptr),
				radius_label_(nullptr)
		{
			stack_ = new QStackedWidget(this);

			// Page 0: placeholder.
			placeholder_ = new QLabel(
				tr("Select a single atom for details."),
				stack_);
			placeholder_->setWordWrap(true);
			placeholder_->setContentsMargins(8, 8, 8, 8);
			stack_->addWidget(placeholder_);

			// Page 1: per-atom property panel.
			QWidget* atom_panel = new QWidget(stack_);
			QVBoxLayout* col = new QVBoxLayout(atom_panel);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(4);
			element_label_  = new QLabel(QStringLiteral("—"), atom_panel);
			position_label_ = new QLabel(QStringLiteral("—"), atom_panel);
			charge_label_   = new QLabel(QStringLiteral("—"), atom_panel);
			radius_label_   = new QLabel(QStringLiteral("—"), atom_panel);
			col->addWidget(new FormRow(tr("Element"), element_label_, atom_panel));
			col->addWidget(new FormRow(tr("Position"), position_label_, atom_panel));
			col->addWidget(new FormRow(tr("Charge"), charge_label_, atom_panel));
			col->addWidget(new FormRow(tr("Radius"), radius_label_, atom_panel));
			stack_->addWidget(atom_panel);

			stack_->setCurrentIndex(0);
			setContent(stack_);
		}

		PropertiesSection::~PropertiesSection() = default;

		void PropertiesSection::updateForSelection(const std::list<Composite*>& selection)
		{
			Atom* a = singleAtomOrNull_(selection);
			if (a == nullptr)
			{
				stack_->setCurrentIndex(0);
				return;
			}
			stack_->setCurrentIndex(1);
			element_label_->setText(QString::fromUtf8(a->getElement().getSymbol().c_str()));
			const Vector3& p = a->getPosition();
			position_label_->setText(QStringLiteral("(%1, %2, %3) Å")
				.arg(p.x, 0, 'f', 3)
				.arg(p.y, 0, 'f', 3)
				.arg(p.z, 0, 'f', 3));
			charge_label_->setText(QString::number(a->getCharge(), 'f', 3));
			radius_label_->setText(QStringLiteral("%1 Å").arg(a->getRadius(), 0, 'f', 3));
		}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
