// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 Plan 04 — RepHeaderSection implementation.
//

#include <BALL/VIEW/WIDGETS/inspector/sections/repHeaderSection.h>


#include <BALL/VIEW/WIDGETS/formRow.h>
#include <BALL/VIEW/KERNEL/representation.h>

#include <QtCore/QSignalBlocker>
#include <QtCore/QVariant>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		RepHeaderSection::RepHeaderSection(QWidget* parent)
			: InspectorSection(tr("Representation"),
			                   QStringLiteral("Representation/Header"),
			                   parent),
				combo_(nullptr), summary_(nullptr)
		{
			QWidget* content = new QWidget(this);
			QVBoxLayout* col = new QVBoxLayout(content);
			col->setContentsMargins(8, 4, 8, 8);
			col->setSpacing(4);

			combo_   = new QComboBox(content);
			summary_ = new QLabel(QStringLiteral("—"), content);

			col->addWidget(new FormRow(tr("Active"), combo_, content));
			col->addWidget(new FormRow(tr("Details"), summary_, content));

			setContent(content);

			connect(combo_,
			        QOverload<int>::of(&QComboBox::currentIndexChanged),
			        this, &RepHeaderSection::onComboChanged_);
		}

		RepHeaderSection::~RepHeaderSection() = default;

		void RepHeaderSection::updateRepresentations(const std::list<Representation*>& reps,
		                                             Representation* active)
		{
			last_reps_ = reps;
			QSignalBlocker block(combo_);
			combo_->clear();

			int active_index = -1;
			int i = 0;
			for (Representation* r : reps)
			{
				if (r == nullptr) continue;
				QString label = QString::fromUtf8(r->getCompositeName().c_str());
				if (label.isEmpty()) label = tr("Representation");
				label = QStringLiteral("#%1 %2").arg(i + 1).arg(label);
				combo_->addItem(label, QVariant::fromValue(static_cast<void*>(r)));
				if (r == active) active_index = i;
				++i;
			}
			if (active_index >= 0)
				combo_->setCurrentIndex(active_index);

			if (active != nullptr)
				summary_->setText(QString::fromUtf8(active->getProperties().c_str()));
			else
				summary_->setText(QStringLiteral("—"));
		}

		void RepHeaderSection::onComboChanged_(int index)
		{
			if (index < 0) return;
			void* ptr = combo_->itemData(index).value<void*>();
			Representation* rep = static_cast<Representation*>(ptr);
			Q_EMIT representationPicked(rep);
		}

	} // namespace VIEW
} // namespace BALL

