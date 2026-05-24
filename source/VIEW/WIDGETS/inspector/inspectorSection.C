// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Phase 999.44 — InspectorSection implementation.
// Phase 999.48 — a11y: QAccessible::Pane (Section equivalent) role + name
//                per Handover §8.2.
//

#include <BALL/VIEW/WIDGETS/inspector/inspectorSection.h>


#include <BALL/VIEW/WIDGETS/sectionHeader.h>

#include <QtCore/QPropertyAnimation>
#include <QtCore/QEasingCurve>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>

namespace BALL
{
	namespace VIEW
	{

		InspectorSection::InspectorSection(const QString& title,
		                                   const QString& stateKey,
		                                   QWidget* parent)
			: QWidget(parent),
				state_key_(stateKey),
				header_(nullptr),
				content_(nullptr),
				root_layout_(nullptr),
				anim_(nullptr),
				expanded_max_height_(0),
				reset_confirm_text_()
		{
			setObjectName("inspectorSection");
			// Phase 999.62 — opaque-paint contract, set ONCE at the base so
			// every InspectorSection subclass inherits it. InspectorSection
			// runs a QPropertyAnimation on its content widget's maximumHeight
			// (the collapse/expand below); without an opaque background the
			// sibling sections sliding through this widget's region during
			// that animation leave transient text/glyph ghosts — the UFG-05 /
			// UFG-09 / UFG-10 defect family. WA_OpaquePaintEvent +
			// setAutoFillBackground + the paintEvent fillRect below guarantee
			// the rect is cleared on every paint pass. The `opaqueContainer`
			// dynamic property is the single machine-detectable marker the
			// opaque-paint lint and the centralized theme token key off.
			setAttribute(Qt::WA_OpaquePaintEvent, true);
			setAutoFillBackground(true);
			setProperty("opaqueContainer", true);
			root_layout_ = new QVBoxLayout(this);
			root_layout_->setContentsMargins(0, 0, 0, 0);
			root_layout_->setSpacing(0);

			header_ = new SectionHeader(title, this);
			root_layout_->addWidget(header_);

			connect(header_, &SectionHeader::expandedChanged,
			        this, &InspectorSection::onHeaderToggled_);
			// v1.7.x-18 — relay the header reset affordance.
			connect(header_, &SectionHeader::resetRequested,
			        this, &InspectorSection::onResetRequested_);

			// Phase 999.48 §8.2 — a11y. Per Handover, InspectorSection maps
			// to QAccessible::Section. Qt 6's role enumeration uses Pane for
			// generic group containers; combined with accessibleName, screen
			// readers announce the section title before recursing into
			// content. The SectionHeader child supplies the heading semantic.
			setAccessibleName(title);
			setAccessibleDescription(tr("Inspector section: %1.").arg(title));
		}

		InspectorSection::~InspectorSection() = default;

		void InspectorSection::paintEvent(QPaintEvent* event)
		{
			// Phase 999.62 — opaque-paint contract. WA_OpaquePaintEvent is
			// set in the ctor, so Qt expects us to cover the whole rect
			// ourselves; fill it with the Window palette colour.
			QPainter p(this);
			p.fillRect(event->rect(), palette().color(QPalette::Window));
		}

		QString InspectorSection::title() const
		{
			return header_ ? header_->title() : QString();
		}

		bool InspectorSection::isExpanded() const
		{
			return header_ ? header_->isExpanded() : false;
		}

		void InspectorSection::setContent(QWidget* content)
		{
			if (!content || content_)
				return;
			content_ = content;
			content_->setParent(this);
			root_layout_->addWidget(content_);

			// Lazy-create the animation now that we have a target.
			anim_ = new QPropertyAnimation(content_, "maximumHeight", this);
			anim_->setDuration(160);
			anim_->setEasingCurve(QEasingCurve::OutCubic);

			// Cache the natural height after first show. Use the size hint
			// as the initial expanded max — Qt expands it on first paint.
			expanded_max_height_ = content_->sizeHint().height();
			if (expanded_max_height_ <= 0)
				expanded_max_height_ = 1000;  // sane fallback for an as-yet-unrendered widget
		}

		void InspectorSection::setExpanded(bool expanded)
		{
			if (header_)
				header_->setExpanded(expanded);
		}

		void InspectorSection::setResettable(bool resettable,
		                                     const QString& confirm_text)
		{
			reset_confirm_text_ = confirm_text;
			if (header_) header_->setResetVisible(resettable);
		}

		void InspectorSection::onResetRequested_()
		{
			// v1.7.x-18 — confirm (unless confirm text is empty) then
			// emit resetRequested() for the subclass to wire to its
			// controller's revert().
			if (!reset_confirm_text_.isEmpty())
			{
				const QMessageBox::StandardButton answer =
					QMessageBox::question(this, tr("Reset section"),
						reset_confirm_text_,
						QMessageBox::Reset | QMessageBox::Cancel,
						QMessageBox::Cancel);
				if (answer != QMessageBox::Reset) return;
			}
			Q_EMIT resetRequested();
		}

		void InspectorSection::onHeaderToggled_(bool expanded)
		{
			if (!content_ || !anim_)
			{
				Q_EMIT expandedChanged(expanded);
				return;
			}

			// Refresh the natural height before each expand so dynamic
			// content (e.g. populated empty state) animates to the right
			// target.
			if (expanded)
				expanded_max_height_ = std::max(content_->sizeHint().height(), 1);

			anim_->stop();
			anim_->setStartValue(content_->maximumHeight());
			anim_->setEndValue(expanded ? expanded_max_height_ : 0);
			anim_->start();

			Q_EMIT expandedChanged(expanded);
		}

	} // namespace VIEW
} // namespace BALL

