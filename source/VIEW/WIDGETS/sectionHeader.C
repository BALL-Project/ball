// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.43 — BALLView Refresh: SectionHeader implementation.
// Phase 999.48 — a11y: QAccessible::Heading role + name per Handover §8.2.
// See `include/BALL/VIEW/WIDGETS/sectionHeader.h` for design.
//

#include <BALL/VIEW/WIDGETS/sectionHeader.h>
#include <BALL/VIEW/KERNEL/theme/iconRegistry.h>

#include <QtGui/QAccessible>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
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
			// v1.7.0-rc2 UFG-05 — paint a solid background.
			//
			// rc1 left SectionHeader as a plain QWidget with no
			// background — Qt's default for a QWidget is "transparent
			// where the child layout doesn't draw", so the gaps around
			// the chevron / label / rule passed pixels straight through
			// to whatever was painted underneath. When the parent
			// InspectorSection ran its expand/collapse QPropertyAnimation
			// on the content widget's maximumHeight, sibling sections
			// shifted up/down through the header's region and left
			// fragments of QComboBox / QLabel text overlapping the
			// header glyphs ("OPENG/SACCOUTH" in the bug screenshot,
			// fragments of "OpenGL" + "Saccharide" bleeding through
			// the "REPRESENTATION" title).
			//
			// Fix: opt into Qt::WA_OpaquePaintEvent + paint the window
			// palette colour ourselves in paintEvent. This guarantees
			// the entire header rect is cleared on every paint pass, so
			// stale pixels from underneath can never persist regardless
			// of the parent's animation state.
			setAttribute(Qt::WA_OpaquePaintEvent, true);
			setAutoFillBackground(true);
			// Phase 999.62 — uniform opaque-paint contract marker.
			setProperty("opaqueContainer", true);

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

			// v1.7.x-18 — optional reset-to-defaults affordance. Hidden by
			// default; sections that have a controller revert() call
			// setResetVisible(true). Sits after the rule so it right-aligns.
			reset_btn_ = new QToolButton(this);
			reset_btn_->setObjectName(QStringLiteral("sectionHeaderReset"));
			reset_btn_->setAutoRaise(true);
			reset_btn_->setFocusPolicy(Qt::StrongFocus);
			reset_btn_->setCursor(Qt::PointingHandCursor);
			reset_btn_->setIconSize(QSize(14, 14));
			reset_btn_->setToolTip(tr("Reset this section to defaults"));
			reset_btn_->setAccessibleName(tr("Reset %1 section to defaults").arg(title));
			{
				QIcon reset_icon = Icons::get("actions/reset");
				if (reset_icon.isNull())
					reset_btn_->setText(QStringLiteral("↺"));  // ↺ fallback
				else
					reset_btn_->setIcon(reset_icon);
			}
			reset_btn_->setVisible(false);
			connect(reset_btn_, &QToolButton::clicked, this, &SectionHeader::resetRequested);

			QHBoxLayout* layout = new QHBoxLayout(this);
			// UAT (v1.7.4): non-zero RIGHT margin so the right-aligned reset_btn_
			// is not flush against the section's right edge (where the scroll
			// gutter / overlay scrollbar clipped it). 6 px keeps it clear.
			layout->setContentsMargins(0, 4, 6, 4);
			layout->setSpacing(8);
			layout->addWidget(chevron_);
			layout->addWidget(title_label_);
			layout->addWidget(rule, /*stretch=*/1);
			layout->addWidget(reset_btn_);

			// Phase 999.48 §8.2 — a11y. The header is logically a
			// QAccessible::Heading at level 2. Qt's default factory exposes
			// the QLabel as text; setting accessibleName on the root widget
			// (the SectionHeader) gives the heading semantics, and the
			// chevron remains a button (focusable for keyboard collapse).
			setAccessibleName(title);
			setAccessibleDescription(tr("Section header. Activate the chevron to collapse or expand the section."));
			// Make the chevron tab-reachable for keyboard a11y (override
			// the NoFocus policy set above for the visual hover affordance).
			chevron_->setFocusPolicy(Qt::StrongFocus);
			chevron_->setAccessibleName(tr("Toggle %1 section").arg(title));
		}

		SectionHeader::~SectionHeader() = default;

		void SectionHeader::setResetVisible(bool visible)
		{
			if (reset_btn_) reset_btn_->setVisible(visible);
		}

		bool SectionHeader::isResetVisible() const
		{
			return reset_btn_ != nullptr && reset_btn_->isVisible();
		}

		QString SectionHeader::title() const
		{
			return title_label_ ? title_label_->text() : QString();
		}

		void SectionHeader::setTitle(const QString& t)
		{
			if (title_label_) title_label_->setText(t);
			// Phase 999.48 §8.2 — keep a11y name in sync.
			setAccessibleName(t);
			if (chevron_) chevron_->setAccessibleName(tr("Toggle %1 section").arg(t));
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

		void SectionHeader::paintEvent(QPaintEvent* event)
		{
			// v1.7.0-rc2 UFG-05 — explicit opaque fill.
			//
			// WA_OpaquePaintEvent makes Qt promise the widget will paint
			// every pixel of its area itself (and skip the parent-background
			// fill that would otherwise run before paintEvent). We honor
			// that promise here: fill the dirty rect with the Window
			// palette colour. setAutoFillBackground() alone is not enough
			// when WA_OpaquePaintEvent is set — Qt explicitly disables the
			// background-fill step on opaque widgets to avoid double-paint.
			QPainter p(this);
			p.fillRect(event->rect(), palette().color(QPalette::Window));
			// Don't call QWidget::paintEvent — the child widgets (chevron,
			// label, rule) paint themselves via the layout / repaint chain.
			// Calling the base would re-run the (now no-op) background
			// step but adds no glyphs.
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
