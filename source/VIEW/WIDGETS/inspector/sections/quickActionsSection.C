// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 03 — QuickActionsSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/quickActionsSection.h>


#include <BALL/VIEW/KERNEL/mainControl.h>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		QuickActionsSection::QuickActionsSection(MainControl* main_control,
		                                         QWidget* parent)
			: InspectorSection(tr("Quick Actions"),
			                   QStringLiteral("Selection/QuickActions"),
			                   parent),
				main_control_(main_control),
				clear_btn_(nullptr),
				invert_btn_(nullptr),
				expand_btn_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(6);

			QHBoxLayout* row = new QHBoxLayout();
			row->setContentsMargins(0, 0, 0, 0);
			row->setSpacing(6);

			clear_btn_  = new QPushButton(tr("Clear Selection"), content);
			invert_btn_ = new QPushButton(tr("Invert Selection"), content);
			// v1.7.x-19 — grow the current atom selection to whole residues.
			expand_btn_ = new QPushButton(tr("Expand to Residues"), content);
			expand_btn_->setToolTip(tr("Select all atoms of every residue that "
				"the current selection touches"));

			row->addWidget(clear_btn_);
			row->addWidget(invert_btn_);
			row->addWidget(expand_btn_);
			row->addStretch(1);
			col->addLayout(row);

			setContent(content);

			connect(clear_btn_,  &QPushButton::clicked,
			        this, &QuickActionsSection::onClearClicked_);
			connect(invert_btn_, &QPushButton::clicked,
			        this, &QuickActionsSection::onInvertClicked_);
			connect(expand_btn_, &QPushButton::clicked,
			        this, &QuickActionsSection::onExpandToResiduesClicked_);
		}

		QuickActionsSection::~QuickActionsSection() = default;

		void QuickActionsSection::onClearClicked_()
		{
			if (main_control_ != nullptr) main_control_->clearSelection();
		}

		void QuickActionsSection::onInvertClicked_()
		{
			if (main_control_ != nullptr) main_control_->complementSelection();
		}

		void QuickActionsSection::onExpandToResiduesClicked_()
		{
			if (main_control_ != nullptr) main_control_->expandSelectionToResidues();
		}

	} // namespace VIEW
} // namespace BALL

