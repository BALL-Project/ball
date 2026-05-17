// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: FormRow implementation.
//

#include <BALL/VIEW/WIDGETS/formRow.h>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

namespace BALL
{
	namespace VIEW
	{

		FormRow::FormRow(const QString& label, QWidget* control, QWidget* parent)
			: QWidget(parent),
				label_(nullptr),
				control_(control),
				hint_label_(nullptr),
				root_layout_(nullptr)
		{
			setObjectName(QStringLiteral("formRow"));

			label_ = new QLabel(label, this);
			label_->setObjectName(QStringLiteral("formRowLabel"));
			label_->setMinimumWidth(LABEL_MIN_WIDTH);
			label_->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

			QHBoxLayout* row = new QHBoxLayout();
			row->setContentsMargins(0, 0, 0, 0);
			row->setSpacing(12);
			row->addWidget(label_);
			if (control_)
			{
				control_->setParent(this);
				row->addWidget(control_, /*stretch=*/1);
			}
			else
			{
				row->addStretch(1);
			}

			root_layout_ = new QVBoxLayout(this);
			root_layout_->setContentsMargins(0, 0, 0, 0);
			root_layout_->setSpacing(2);
			root_layout_->addLayout(row);
		}

		FormRow::~FormRow() = default;

		void FormRow::setHint(const QString& hint)
		{
			if (hint.isEmpty())
			{
				clearHint();
				return;
			}
			ensureHintLabel_();
			hint_label_->setText(hint);
			hint_label_->setVisible(true);
		}

		void FormRow::clearHint()
		{
			if (!hint_label_) return;
			hint_label_->setVisible(false);
			hint_label_->setText(QString());
		}

		QString FormRow::hint() const
		{
			return hint_label_ ? hint_label_->text() : QString();
		}

		void FormRow::ensureHintLabel_()
		{
			if (hint_label_) return;
			hint_label_ = new QLabel(this);
			hint_label_->setObjectName(QStringLiteral("formRowHint"));
			QFont f = hint_label_->font();
			f.setItalic(true);
			f.setPointSize(qMax(f.pointSize() - 1, 9));
			hint_label_->setFont(f);
			// Indent the hint to roughly line up with the control column.
			hint_label_->setContentsMargins(LABEL_MIN_WIDTH + 12, 0, 0, 0);
			root_layout_->addWidget(hint_label_);
		}

	} // namespace VIEW
} // namespace BALL
