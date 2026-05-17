// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 03 — SelectionSummarySection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/selectionSummarySection.h>

#ifdef BALL_UI_V2

#include <BALL/VIEW/WIDGETS/formRow.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/CONCEPT/composite.h>

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		namespace
		{
			template <typename T>
			std::size_t countDescendants_(const Composite& root)
			{
				// Walk the recursive CompositeConstIterator and tally
				// nodes matching T. We treat each subtree given to us
				// (a selection entry) independently so users that
				// select two non-overlapping chains see the union.
				std::size_t count = 0;
				Composite::CompositeConstIterator it = root.beginComposite();
				for (; +it; ++it)
				{
					if (dynamic_cast<const T*>(&*it) != 0) ++count;
				}
				return count;
			}
		}

		SelectionSummarySection::SelectionSummarySection(QWidget* parent)
			: InspectorSection(tr("Summary"),
			                   QStringLiteral("Selection/Summary"),
			                   parent),
				atoms_label_(nullptr),
				residues_label_(nullptr),
				chains_label_(nullptr),
				molecules_label_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(4);

			atoms_label_     = new QLabel(QStringLiteral("0"), content);
			residues_label_  = new QLabel(QStringLiteral("0"), content);
			chains_label_    = new QLabel(QStringLiteral("0"), content);
			molecules_label_ = new QLabel(QStringLiteral("0"), content);

			col->addWidget(new FormRow(tr("Atoms"), atoms_label_, content));
			col->addWidget(new FormRow(tr("Residues"), residues_label_, content));
			col->addWidget(new FormRow(tr("Chains"), chains_label_, content));
			col->addWidget(new FormRow(tr("Molecules"), molecules_label_, content));

			setContent(content);
		}

		SelectionSummarySection::~SelectionSummarySection() = default;

		void SelectionSummarySection::updateForSelection(const std::list<Composite*>& selection)
		{
			std::size_t atoms = 0, residues = 0, chains = 0, molecules = 0;
			for (Composite* c : selection)
			{
				if (c == nullptr) continue;
				// CompositeConstIterator begins at the root itself so
				// each tally already includes the head node when it
				// matches T — no separate root check needed.
				atoms     += countDescendants_<Atom>(*c);
				residues  += countDescendants_<Residue>(*c);
				chains    += countDescendants_<Chain>(*c);
				molecules += countDescendants_<Molecule>(*c);
			}
			atoms_label_->setText(QString::number(atoms));
			residues_label_->setText(QString::number(residues));
			chains_label_->setText(QString::number(chains));
			molecules_label_->setText(QString::number(molecules));
		}

	} // namespace VIEW
} // namespace BALL

#endif // BALL_UI_V2
