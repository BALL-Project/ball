// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: SectionHeader implementation.
// See `include/BALL/VIEW/WIDGETS/sectionHeader.h` for design.
//

#include <BALL/VIEW/WIDGETS/sectionHeader.h>
#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>

#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>

namespace BALL
{
	namespace VIEW
	{

		SectionHeader::SectionHeader(const QString& title, QWidget* parent)
			: QWidget(parent),
				expanded_(true),
				chevron_(nullptr),
				title_label_(nullptr)
		{
			setObjectName(QStringLiteral("sectionHeader"));

			chevron_ = new QToolButton(this);
			chevron_->setObjectName(QStringLiteral("sectionHeaderChevron"));
			chevron_->setAutoRaise(true);
			chevron_->setFocusPolicy(Qt::NoFocus);
			chevron_->setCursor(Qt::PointingHandCursor);
			chevron_->setIconSize(QSize(12, 12));
			updateChevronIcon_();
			connect(chevron_, &QToolButton::clicked, this, &SectionHeader::onChevronClicked);

			title_label_ = new QLabel(title, this);
			title_label_->setObjectName(QStringLiteral("sectionHeaderTitle"));
			QFont f = title_label_->font();
			f.setPointSize(11);
			f.setCapitalization(QFont::AllUppercase);
			f.setLetterSpacing(QFont::PercentageSpacing, 110);
			f.setBold(true);
			title_label_->setFont(f);

			QFrame* rule = new QFrame(this);
			rule->setObjectName(QStringLiteral("sectionHeaderRule"));
			rule->setFrameShape(QFrame::HLine);
			rule->setFrameShadow(QFrame::Plain);
			rule->setLineWidth(1);

			QHBoxLayout* layout = new QHBoxLayout(this);
			layout->setContentsMargins(0, 4, 0, 4);
			layout->setSpacing(8);
			layout->addWidget(chevron_);
			layout->addWidget(title_label_);
			layout->addWidget(rule, /*stretch=*/1);
		}

		SectionHeader::~SectionHeader() = default;

		QString SectionHeader::title() const
		{
			return title_label_ ? title_label_->text() : QString();
		}

		void SectionHeader::setTitle(const QString& t)
		{
			if (title_label_) title_label_->setText(t);
		}

		void SectionHeader::setExpanded(bool expanded)
		{
			if (expanded == expanded_) return;
			expanded_ = expanded;
			updateChevronIcon_();
			Q_EMIT expandedChanged(expanded_);
		}

		void SectionHeader::toggle()
		{
			setExpanded(!expanded_);
		}

		void SectionHeader::onChevronClicked()
		{
			toggle();
		}

		void SectionHeader::updateChevronIcon_()
		{
			if (!chevron_) return;
			// chevron-down when expanded (points "open"), chevron-right when
			// collapsed (points "click to open"). If neither icon is in the
			// theme bundle (BALL_UI_V2 OFF + no IconLoader entry), QIcon will
			// be null and the button shows a fallback arrow via Qt's style.
			QIcon icon = expanded_ ? Icons::get("actions/chevron-down")
			                       : Icons::get("actions/chevron-right");
			if (icon.isNull())
			{
				// Fallback to Qt's standard arrow shapes so the widget
				// stays functional on builds without the SVG bundle.
				chevron_->setArrowType(expanded_ ? Qt::DownArrow : Qt::RightArrow);
				chevron_->setIcon(QIcon());
			}
			else
			{
				chevron_->setArrowType(Qt::NoArrow);
				chevron_->setIcon(icon);
			}
		}

	} // namespace VIEW
} // namespace BALL
